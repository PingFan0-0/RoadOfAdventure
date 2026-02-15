/*=========================================
--- 项目名称: 冒险之路 --------------------
--- 当前项目版本: 0.3 ---------------------
--- 项目环境: windows C++11 ---------------
--- 编译软件: Dev-C++ ---------------------
-------------------------------------------
--- 项目作者: 平凡0_0 ---------------------
--- 哔哩哔哩: https://b23.tv/mGuvqJi ------
--- 项目初创时间: 2026/1/23 ---------------
--- 项目更新时间: 2026/2/14 ----------------
--- 项目类型: 开源 ------------------------
=========================================*/


/*===============声明=========================================================================================
--------------------------------------------------------------------------------------------------------------
--- 本项目由 [平凡0_0] 原创制作 版权所有 ---------------------------------------------https://b23.tv/mGuvqJi--
-------------------------------------------------------------------------------------------------------^^^----
--- 任何个人或机构"可以"对其 复制 修改 用于商业盈利或通过信息网络进行传播 但使用时需完整注明作者及出处链接 ---
--- 此项目为开源项目 任何组织和个人不可将其占为己有 ----------------------------------------------------------
--- 此项目主要用于代码的交流与学习 --------------------------------------------------------------------------- 
--------------------------------------------------------------------------------------------------------------
============================================================================================================*/

/*------ 0.3 概述 --------------------------------------------------------
体验层面
	1.新增一个显示内容: 地图文件信息显示
	2.玩家可以制作属于自己的地图 
代码层面
	1.删除函数:
		1. GameMapChange(); 游戏地图切换    (已被GameMapData("");取代)
	2.新增函数:
		1. CDW(L""); 创建文件夹 
		2. FFFW(L""); 获取文件 
		3. GameMapData(""); 读取地图文件
		4. cls(); 清屏函数 "极大"的减少了屏幕刷新时的闪烁 
	3.将较多数据与代码分离 保存在 Data 文件夹当中
		优势:减少代码体积 减少运行时的内存占用(影响似乎不大)
	4.部分原有代码 添加注释 
	5.分离万能头文件
	6.取消 std 全局申明 
其他层面
	1.地图编辑器(未完工) 
------------------------------------------------------------------------*/









#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<conio.h>
#include<windows.h>
#include<cstdlib>



std::wstring DataWayParent = L"GameData";//文件夹路径 "GameData" 
std::wstring SetWayParent = L"Set";//文件夹路径 "Set" 
std::wstring MapDataParent = L"Data";//文件夹路径 "Data" 


std::string GameDataWay = "GameData/GameData.txt";//文件路径及名称  
std::string SetDataWay = "Set/SetData.txt";//文件路径及名称  
std::string DebugWay = "Set/Debug.txt";//文件路径及名称  

std::string BeginMap = "home";//初始的地图 

const int MapMaxSize = 55;//定义地图内存的大小 

//-1.死亡页面 0.初始页面 1.游戏页面 2.设置页面  
int cz = 0;//显示的页面 

int TheGameBool;//--------------游戏是否运行 
int DebugBool = 1;//------------日志Debug是否开启
int ZbxsBool = 1;//-------------坐标显示是否开启 
int GameRunTimeBool = 1;//------游戏时刻显示是否开启 
int FPSBool = 1;//--------------FPS显示是否开启
int MapMessage = 1;//-----------地图信息显示是否开启 

int FPS;//-----------------FPS数值 
int FPSWeek = 10;//--------FPS循环周期 
long GameRunTime = 0;//----游戏时刻 
clock_t LastTime;//--------上次时间 
int JGTime;//--------------间隔的时间

int maxx,maxy;//当前地图最大尺寸 

struct SP{//StructPlayer 的缩写   玩家信息 
	int LastX,LastY;//--玩家上次的位置 
	int NextX,NextY;//--玩家行动的位置 
	int myx,myy;//------玩家位置 
	int Life;//---------玩家生命
	int MaxLife;//------玩家最大生命 
	std::string MapHand;//---当前地图序号 
};
SP Player;

std::string ClsText;//清屏str 


//地图文件======================================================================
//0."  "空气   1."##"墙   2."门"门   9."%%"陷阱   10."敌"敌人   11."人"一个人 

std::string mapdataname;

int mapp[MapMaxSize][MapMaxSize];//游戏运行时的地图数组 

std::string GameText[15];//文本数组 

int mapsj[20][40];//特殊事件

std::string MapName[21];//地图列表  暂时作用不大 
int MapNum;//地图列表 的地图数量 
//======================================================================地图文件





int ct(int x, int y, std::string str){//<-------------------------------------------------------指定打印 增加FPS 
    COORD zb;
    zb.X = x;
    zb.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), zb);
    std::cout << str;
    std::cout.flush(); 
    return 0;
}

std::string wstring_string(std::wstring wstr){
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);//   来自AI 将 wstring 转换为 string 
    std::string strr(len, 0);//                                                               来自AI 将 wstring 转换为 string 
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &strr[0], len, NULL, NULL);//       来自AI 将 wstring 转换为 string 
	strr.pop_back();//去除最后的一个字符   在此是 ' '<---空格 
	return strr;
}

int Debug(std::string str){//<---------------------------------------------------------日志
	
	if(str == "sss"){//清空日志 
    	std::ofstream outfile(DebugWay);
		outfile<<"==================================日志==================================";
		if(!DebugBool) outfile<<"\n日志以记录关闭 可以在设置中打开此功能";
		return 0;
	}
	
	if(!DebugBool)return 0;//如果日志记录关闭 就退出函数 
	
	std::string strr; 
	strr+="\n";
    
    
    //写入程序运行时间 
    strr+="[";
	double NowTime = clock();
	strr += std::to_string(NowTime/1000); 
    strr+="] ";
    
    //写入Debug 
    strr+=str;
    std::ofstream outfile(DebugWay,std::ios::app);
    outfile<<strr;//写入文件
}

void cls(){//<-------------------------------------------------------------------------------清屏函数  "极大"的减少了屏幕刷新时的闪烁 
	ct(0,0,ClsText);
}

void CDW(const std::wstring& str){//<-------------------------------------------------------创建文件夹 
    CreateDirectoryW(str.c_str(),NULL);
}

void FFFW(const std::wstring& str){//<-------------------------------------------------------获取文件 
	Debug("搜索地图文件==="); 
	WIN32_FIND_DATAW num;//创建结构体以暂时保存地图名称 
	HANDLE Name = FindFirstFileW((str + L"\\*").c_str(), &num);// 来自AI 获取名称
	int i = 0;//定义记号 
	while(FindNextFileW(Name,&num)){//是否还有文件 
		std::string strr = wstring_string(num.cFileName);//获取名称
		MapName[i] = strr;//将地图名称写入数组
		Debug("第"+std::to_string(i)+"个地图文件:"+strr);
		i++;//记号加一 
	}
    MapNum = i-1;//记录地图数量 
    
    FindClose(Name);//结束寻找 
    Debug("搜索了"+std::to_string(MapNum)+"个文件");
    Debug("===搜索地图文件");
}

void czdata(std::string xz){//<--------------------------------------------------------------重置数据 
	if(xz == "GD"){
		
		Debug("重置游戏数据");
		
		Player.myx = 1;
		Player.myy = 1;
		Player.Life = 100;
		Player.MaxLife = 100;
		
		GameRunTime = 0;
		
		Player.MapHand = BeginMap;
		
	}else if(xz == "SD"){
		
	}
	
}

void GameMapData(std::string mapname){//<---------------------------------------------读取地图文件 
	Debug("读取地图=====");
	Debug("文件名称:"+mapname); 
	
	std::string mapway = wstring_string(MapDataParent)+ "/" + mapname + ".txt";//计算地图路径 
	Debug("地图路径:"+mapway); 
	std::ifstream PFMapData(mapway);
	
	PFMapData>>mapdataname;//读取地图名字
	Debug("地图名称:"+mapdataname);
	
	PFMapData>>maxx>>maxy;//读取地图大小 
	for(int i=0;i<maxy;i++){//读取地图数据 
		for(int j=0;j<maxx;j++){
			PFMapData>>mapp[i][j];
		}
	}
	
	int num;
	
	PFMapData>>num;//读取地图特殊事件数量 
	Debug("特殊事件个数:"+std::to_string(num));
	mapsj[0][0] = num;//读取地图特殊事件数量 
	for(int i=1;i<=num;i++){
		PFMapData>>mapsj[i][0]>>mapsj[i][1];//X Y
		PFMapData>>mapsj[i][2];//事件类型
		if(mapsj[i][2] == 1){//1===传送 
			PFMapData>>mapsj[i][3]; 
			PFMapData>>mapsj[i][4]>>mapsj[i][5]; 
		}else if(mapsj[i][2] == 2){//2===对话 
			PFMapData>>mapsj[i][3];//对话人的名字
			int dhnum;//对话数量
			PFMapData>>dhnum;//对话数量
			mapsj[i][4] = dhnum;//对话数量
			int Hand = 5;
			for(int k=1;k<=dhnum;k++){
				PFMapData>>mapsj[i][Hand];//对话内容 
				Hand++;
				int hdnum;//回答数量 
				PFMapData>>hdnum;//回答数量 
				mapsj[i][Hand] = hdnum;//回答数量 
				Hand++;
				for(int j=1;j<=hdnum;j++){
					PFMapData>>mapsj[i][Hand];//回答内容
					Hand++;
					PFMapData>>mapsj[i][Hand];//此回答结果 
					Hand++;
				}
			}
		}
	}
	
	int strnum;
	PFMapData>>strnum;//获取地图文本数量
	for(int i=0;i<strnum;i++){//读取地图文本
		PFMapData>>GameText[i];
	}
	
	cls();
	Debug("=====读取地图");
}

void GameData(std::string xz, int num){//<-------------------------------------------保存/读取游戏数据 
	if(xz == "GDR"){//GameData
		
		Debug("保存游戏数据");
		
		std::string str;
		
		str += std::to_string(num)+"\n";
		//Player.MapHand myx   myy   Life   MaxLife
		str += Player.MapHand+" "+std::to_string(Player.myx)+" "+std::to_string(Player.myy)+" "+std::to_string(Player.Life)+" "+std::to_string(Player.MaxLife)+"\n";
		str += std::to_string(GameRunTime)+"\n";
		
		std::ofstream PFR(GameDataWay);//写入文件
		PFR<<str;
		
	}else if(xz == "GDW"){//GameData
		
		Debug("读取游戏数据");
		
		std::ifstream PFW(GameDataWay);//用PFW表示Debug.txt文件 
		
		int num;
		PFW>>num;
		if(num){
			PFW>>Player.MapHand>>Player.myx >>Player.myy >>Player.Life >>Player.MaxLife;
			PFW>>GameRunTime;
		}else{
			czdata("GD");
		}
		
	}else if(xz == "SDW"){//SetData
		
		Debug("保存设置数据");
		
		std::string str;
		
		str += "1\n";
		str += std::to_string(DebugBool)+" "+std::to_string(ZbxsBool)+" "+std::to_string(GameRunTimeBool)+" "+std::to_string(FPSBool)+" "+std::to_string(MapMessage)+"\n";
		str += std::to_string(FPSWeek)+"\n";
		
		std::ofstream PFR(SetDataWay);//写入文件
		PFR<<str;
		
	}else if(xz == "SDR"){//SetData
		
		Debug("读取设置数据");
		
		std::ifstream PFW(SetDataWay);//用file表示SetData.txt文件 
		
		int num;
		PFW>>num;
		if(num){
			PFW>>DebugBool;
			PFW>>ZbxsBool >>GameRunTimeBool >>FPSBool >>MapMessage;
			PFW>>FPSWeek;
		}else{
			czdata("SD");
		}
	}
	
}


void gamestart(){//<----------------------------------------------------------游戏初始化
	
	GameData("GDW",-1);
	GameMapData(Player.MapHand);
}


void pmsx(){//<---------------------------------------------------------------屏幕刷新 
	
//	cls();
	std::string str;
	
	
	str += "主角信息:\n";
	
	//=====血条显示 
	int num1 = Player.Life / 10;
	int num2 = Player.Life % 10;
	str += "生命值:";
	str += std::to_string(Player.Life)+" ";
	for(int i=1;i<=num1;i++) str+="##";
	if(num2>=5) str+="# ";
	else str+="  ";
	for(int i=1;i<=10-num1;i++) str+="  ";
	str += "\n";
	
	
	if(ZbxsBool){//=====坐标显示 
		str +="位置:"+std::to_string(Player.myx)+" "+std::to_string(Player.myy)+"               \n"; 
	}
	
	if(GameRunTimeBool){//游戏时刻显示 
		str +="游戏时刻:"+std::to_string(GameRunTime)+"\n";
	}
	
	if(FPSBool){//=====FPS显示 
//		Debug(to_string(GameRunTime % FPSWeek),-1);
		if(!(GameRunTime % FPSWeek)){
			clock_t NowTime=clock();
			JGTime = (NowTime-LastTime)/FPSWeek;
			FPS = (1000*FPSWeek)/(NowTime-LastTime);
//			Debug("FPS "+to_string(FPS),-1);
//			Debug("FPS= "+to_string((1000*FPSWeek)/(NowTime-LastTime)),-1);
//			Debug("Week "+to_string(FPSWeek),-1);
//			Debug("Time "+to_string(NowTime-LastTime),-1); 
			LastTime = NowTime;
		}
		str +="FPS:"+std::to_string(FPS)+" "+std::to_string(JGTime)+"ms      \n";
	}
	
	if(MapMessage){//=====地图文件信息显示 
		str+="文件名:"+Player.MapHand+" 大小XY:"+std::to_string(maxx)+"*"+std::to_string(maxy)+" 事件:"+std::to_string(mapsj[0][0]);
		str+="\n";
	}
	
	
	for(int i=0;i<maxy;i++){//地图显示 
		for(int j=0;j<maxx;j++){
			int num = mapp[i][j];
			
			if(j == Player.myx && i == Player.myy) str+="MY";
			else if(num ==  0) str+="  ";
			else if(num ==  1) str+="##"; 
			else if(num ==  2) str+="门"; 
			else if(num ==  9) str+="%%"; 
			else if(num == 10) str+="敌"; 
			else if(num == 11) str+="人"; 	
		}
		str+="  ";
		str+="\n"; 
	}
	
	for(int i=0;i<=maxx;i++) str+="  ";
	str+="\n";
	
	str+=mapdataname;//地图名称 
	str+="\n";
	
	ct(0,0,str);
}




void playerinput(char input){//<---------------------------------------------------玩家输入 
	
	if(input == 'r' || input == 'R'){
		GameData("GDR",1);
		cz=0;
	}
	
	
	int xx=0,yy=0;
	
	
	if(input == 'w' || input == 'W'){
		yy--;
	}else if(input == 'a' || input == 'A'){
		xx--;
	}else if(input == 's' || input == 'S'){
		yy++;
	}else if(input == 'd' || input == 'D'){
		xx++;
	}
	
//	if(1){
//		Player.LastX = Player.myx;
//		Player.LastY = Player.myy;
//	}
	
	
	Player.NextX = Player.myx + xx;
	Player.NextY = Player.myy + yy;
	
	
	int num = mapp[Player.myy + yy][Player.myx + xx];
	
	if((xx != 0 || yy != 0) && (num == 0 || num == 2 || num == 9)){
		
		Player.LastX = Player.myx;
		Player.LastY = Player.myy;
		
		Player.myx += xx;
		Player.myy += yy;
	}
	
}

void shpd(){//<---------------------------------------------------------------------伤害判定 
	
	if(mapp[Player.myy][Player.myx] == 9){//陷阱判定 
		Player.Life-=3;	
	}
	
//	Debug(to_string(Player.myx)+" "+to_string(Player.LastX)+" "+to_string(Player.myy)+" "+to_string(Player.LastY),-1);
	if(Player.myx != Player.LastX || Player.myy != Player.LastY){//敌人判定 
		int drsl = 0;//敌人数量 
		int numX,numY;
		numX = Player.myx;
		numY = Player.myy;
		for(int i=-1;i<=1;i++){
			for(int j=-1;j<=1;j++){
				if(mapp[numY+i][numX+j] == 10){
					drsl++; 
				}
			}
		}
//		Debug("敌人判定 ",drsl);
		Player.Life-=drsl;//造成伤害 
	}
	
	if(Player.Life<0)Player.Life;
}


void swpd(){//<---------------------------------------------------------------------死亡判定 
	if(Player.Life<=0){
		cz=-1;
	}
}


void MapSJ(){//<---------------------------------------------------------------------特殊事件 
	int num = mapsj[0][0]; //事件数量 
	for(int i=1;i<=num;i++){
		int SJX,SJY,SJLX;//事件X 事件Y 事件类型 
		SJX = mapsj[i][0];//获取发生位置X 
		SJY = mapsj[i][1];//获取发生位置Y
		if(SJX == Player.NextX && SJY == Player.NextY){//当玩家位置符合时
			Player.NextX = Player.myx;//将玩家的行动坐标设置为玩家坐标(显示在控制台上的坐标) 以防止重复触发 
			Player.NextY = Player.myy;//将玩家的行动坐标设置为玩家坐标(显示在控制台上的坐标) 以防止重复触发 
			SJLX = mapsj[i][2];//获取事件类型 
			if(SJLX == 1){//===================================(1)传送事件 
				Player.myx = mapsj[i][4];//X
				Player.myy = mapsj[i][5];//Y
				if(mapsj[i][3] != -1) Player.MapHand = GameText[mapsj[i][3]];//Player.MapHand
				GameMapData(Player.MapHand);//更新地图内存 
			}else if(SJLX == 2){//=============================(2)对话事件
				std::string DHR;//对话人的名字
				int DHnum;//对话数量
				int DHHAND[20] = {-1,5};//记录每个对话对应的数组位置 
				DHR = GameText[mapsj[i][3]];//获取对话人的名字
				DHnum = mapsj[i][4];//获取对话数量
				int HAND = 1;//记录正在分析对话的序号 
				int SJHAND = 6;//记录正在分析对话对应的数组位置 
				while(1){//分析对话数据 
					HAND++;//为 正在分析对话的序号 加1 
					if(HAND > DHnum)break; //当 正在分析对话的序号 大于 对话个数时 退出分析 
					int numm = mapsj[i][SJHAND];//获取当前对话有多少个回答 
					DHHAND[HAND] = SJHAND+numm*2+1;//计算当前对话对应的数组位置 
					SJHAND += numm*2+2;//计算当前对话对应的数组位置 
				}
				int NOWHAND = 1;//记录正在显示对话的 序号 
				while(1){//显示对话 
					std::string str;
					str+="\n";
					str+=DHR+"\n";//打印对话者的名字 
					str+="    "+GameText[mapsj[i][DHHAND[NOWHAND]]]+"\n\n\n\n";//获取对话的文本 
					int HDnum = mapsj[i][DHHAND[NOWHAND]+1];//获取回答的数量
					if(HDnum == 0){//当没有回答时 
						str+="f.继续";
					}else{//当有回答时 
						str+="你的回答:\n";
						for(int j=1;j<=HDnum;j++){//展示回答 
							str+=std::to_string(j)+".";//打印序号 
							str+=GameText[mapsj[i][DHHAND[NOWHAND]+j*2]];//打印回答内容
							str+="\n"; 
						}
					}
					cls();//清空屏幕 
					ct(0,2,str);//打印缓存
					char input =_getch();//获取玩家输入 
					if(HDnum == 0){//当没有回答时 
						if(input == 'f' || input =='F'){//当玩家输入F时 
							NOWHAND++;//切换下一个对话 
						}
					}else{//当有回答时
						if(input >= '1' && input <= '9'){//检查玩家输入是否为数字 
							int INPUT = input-'0';//将 char 转换为 int
							if(INPUT <= HDnum){//检查玩家输入是否在 回答数量 之内 
								int TZHAND = mapsj[i][DHHAND[NOWHAND]+INPUT*2+1];//获取跳转对话的序号
								if(TZHAND == 0) NOWHAND++;//当 跳转对话的序号 为(0) 将 下一页 设置为显示 
								else if(TZHAND == -1) break;//当 跳转对话的序号 为(-1) 退出对话 
								else NOWHAND = TZHAND; //都不是 将 指定的对话 设置为显示 
							} 
						}
					}
					if(NOWHAND > DHnum) break;//当 当前对话超过对话数量时 退出对话 
				}
				cls();//清空屏幕 
			}
		}
	}
}


void Lifeup(){//<-------------------------------------------------------------------生命自然回复 
	if(GameRunTime %10 == 0 && Player.Life<Player.MaxLife){
		Player.Life++;
	}
}


void SaveData(int Week){//<-----------------------------------------------------------游戏自动保存 
	if(GameRunTime %Week == 0){
		GameData("GDR",1);
	}
} 


void gameon(){//=======================================================================游戏主控 
	while(cz == 1) {
		
		
		pmsx();//屏幕刷新 
		shpd();//伤害判定 
		swpd();//死亡判定
		Lifeup();//生命自然回复 
		if(_kbhit()) playerinput(_getch());//玩家输入 
		MapSJ();//地图特殊事件 
		SaveData(100);//自动保存 
		GameRunTime++;//每循环一次游戏时刻加1 
		Sleep(10);//游戏延迟 
	}
}


void ymxs(){//<----------------------------------------------------------------------页面显示
	//一些简单的if 
	if(cz == 1) gameon();
	if(cz == 0){//起始页面====================(0) 
    	while(1) {//input
			cls();
			std::string str;
			str+= "冒险之路\n1.继续游戏\n2.设置\n3.关于\n\nr.退出游戏";
			
			ct(0,0,str);
			
	        char input=getch();
			if(input == '1') {
				cz=1;
				gamestart();
	            break;
	        }else if(input == '2') {
				cz=2;
				GameData("SDR",-1);
	            break;
	        }else if(input == '3') {
				cz=3;
	            break;
	        }else if(input == 'r' || input == 'R') {
	        	TheGameBool=0;
	            return;
	        }
		}
	}else if(cz == 2){//设置页面====================(2) 
		while(1){
			cls();
			std::string str;
			
			str+="设置\n";
			str+="1.日志记录          当前:";if(DebugBool)       str+="开\n";else str+="关\n";
			str+="2.FPS显示           当前:";if(FPSBool)         str+="开\n";else str+="关\n";
			str+="3.FPS刷新间隔       当前:";                    str+="每"+std::to_string(FPSWeek)+"游戏时刻更新\n";
			str+="4.游戏时刻显示      当前:";if(GameRunTimeBool) str+="开\n";else str+="关\n";
			str+="5.坐标显示          当前:";if(ZbxsBool)        str+="开\n";else str+="关\n";
			str+="6.地图信息显示      当前:";if(MapMessage)      str+="开\n";else str+="关\n";
			
			str+="\n\n\n";
			str+="r.返回";
			
			ct(0,0,str);
			
	        char input=getch();
	        if(input == '1'){//选项 日志记录-----(1) 
	        	DebugBool++;
	        	DebugBool%=2;
			}else if(input == '2'){//选项 FPS显示-----(2)
	        	FPSBool++;
	        	FPSBool%=2;
			}else if(input == '3'){//选项 FPS显示-----(3)
	        	while(1){
	        		cls();
	        		str+="FPS刷新间隔\n"; 
	        		str+="1.每1时刻更新\n";
	        		str+="2.每2时刻更新\n";
	        		str+="3.每5时刻更新\n";
	        		str+="4.每10时刻更新\n";
	        		str+="5.每20时刻更新\n";
	        		str+="6.自定义\n";
	        		
	        		str+="\n";
	        		str+="当前:每"+std::to_string(FPSWeek)+"游戏时刻更新";
	        		
	        		str+="\n\n\n";
					str+="r.返回";
	        		
	        		ct(0,0,str);
	        		
	        		char input1=getch();
	        		if(input1 == '1'){//选项  每1-(1) 
	        			FPSWeek = 1;
					}else if(input1 == '2'){//选项  每2-(2) 
	        			FPSWeek = 2;
					}else if(input1 == '3'){//选项  每5-(3) 
	        			FPSWeek = 5;
					}else if(input1 == '4'){//选项  每10-(4) 
	        			FPSWeek = 10;
					}else if(input1 == '5'){//选项  每20-(5) 
	        			FPSWeek = 20;
					}else if(input1 == '6'){//选项  自定义-(6) 
	        			while(1){
	        				cls();
	        				str+="FPS刷新间隔 自定义\n";
	        				str+="请输入 数值\n";
	        				str+="范围 数值 >= 1\n";
	        				ct(0,0,str);
	        				int num;
	        				std::cin>>num;
	        				if(num>=1){
	        					FPSWeek = num;
								break; 
							}else{
								str+="\n范围错误 按 r 键以重新输入\n";
								ct(0,0,str);
								while(1){
									char input2 = getch();
									if(input2 == 'r' || input2 == 'R'){
										break;
									}
								}
							}
						}
					}else if(input1 == 'r' || input1 == 'R'){
						GameData("SDW",-1);
			            break;
					}
				} 
			}else if(input == '4'){//选项 游戏时刻显示-----(4)
	        	GameRunTimeBool++;
	        	GameRunTimeBool%=2;
			}else if(input == '5'){//选项 坐标显示-----(5)
	        	ZbxsBool++;
	        	ZbxsBool%=2;
			}else if(input == '6'){//选项 地图信息显示-----(6)
	        	MapMessage++;
	        	MapMessage%=2;
			}else if(input == 'r' || input == 'R') {//选项 返回-----(r) 
				GameData("SDW",-1);
	        	cz=0;
	            break;
	        }
		}
	}else if(cz == 3){//关于====================(3) 
		while(1){
			cls();
			std::string str;
			str+="关于\n"; 
			str+="WASD--控制角色\n";
			str+="R-----返回\n";
			
	        str+="\n\n\n";
			str+="r.返回";
			
			ct(0,0,str);
			
			char input=getch();
	        if(input == 'r' || input == 'R') {//选项 返回-----(r)
	        	cz=0;
	            break;
	        }
		}	
	}else if(cz == -1){//死亡页面====================(-1) 
	
		pmsx();//屏幕刷新
		GameData("GDR",0);
		int X=5,Y=6;
		std::string str;
		str=" ________________________ ";
		ct(X,Y+0,str);
		str="|                        |";
		ct(X,Y+1,str);
		str="|        GAME_OVER       |";
		ct(X,Y+2,str);
		str="|                        |";
		ct(X,Y+3,str);
		str="|                        |";
		ct(X,Y+4,str);
		str="|     !!!存档以删除!!!   |";
		ct(X,Y+5,str);
		str="|                        |";
		ct(X,Y+6,str);
		str="|     ---按R键返回---    |";
		ct(X,Y+7,str);
		str="|________________________|";
		ct(X,Y+8,str);
		while(1){
			char input=getch();
			if(input == 'r' || input == 'R'){
				cz=0;
				break;
			}
		}
	}
}

void Begin(){//----------------------------------------------------------------------初始化
	CDW(DataWayParent);//创建文件夹 GameData 
	CDW(SetWayParent);//创建文件夹 Set
	FFFW(MapDataParent);//获取地图文件名称 
	for(int i=1;i<=40;i++)ClsText+="                                                                                \n";//补充清屏 
}

int main() {//----------------------------------------------------------------------main 
	
	GameData("SDR",-1);
	
	Debug("sss");
	
	Debug("初始化");
	Begin();
	
	
	Debug("游戏开始==================");
    
	TheGameBool = 1;
    while(TheGameBool) {//main
    	ymxs();
    }
	Debug("=============游戏正常退出");
    return 0;
}








void ksvoid(){//<--------------------------------------------------------------快速定位函数 
	ct(0,0,"");//<-----------------------------------------指定打印
	Debug("");//<------------------------------------------日志
	cls();//<----------------------------------------------清屏函数 
	CDW(L"");//<-------------------------------------------创建文件夹 
	FFFW(L"");//<------------------------------------------获取文件 
	czdata("");//<-----------------------------------------重置数据 
	GameMapData("");//<------------------------------------读取地图文件
	GameData("",-1);//<------------------------------------保存/读取游戏数据 
	gamestart();//<----------------------------------------游戏初始化
	pmsx();//<---------------------------------------------屏幕刷新 
	playerinput('0');//<-----------------------------------玩家输入 
	shpd();//<---------------------------------------------伤害判定 
	swpd();//<---------------------------------------------死亡判定
	MapSJ();//<--------------------------------------------特殊事件 
	Lifeup();//<-------------------------------------------生命自然回复 
	SaveData(0);//<----------------------------------------游戏自动保存 
	gameon();//============================================游戏主控 
	ymxs();//<---------------------------------------------显示页面 
	Begin();//---------------------------------------------初始化
	main();//----------------------------------------------main 
}

