#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<conio.h>
#include<cstdlib>
#include<windows.h>
#include"json.hpp"


#pragma warning(disable:4996)
#pragma warning(disable:4267)

const std::wstring DataWayParent = L"GameData";//---文件夹路径 "GameData" 
const std::wstring SetWayParent = L"Set";//---------文件夹路径 "Set" 
const std::wstring DataParent = L"Data";//----------文件夹路径 "Data" 


const std::string GameDataWay = "GameData/GameData.json";//---文件路径及名称  
const std::string SetDataWay = "Set/SetData.txt";//----------文件路径及名称  
const std::string DebugWay = "Set/Debug.txt";//--------------文件路径及名称  

const std::string BeginMap = "冒险之路.json";//--------------初始的地图 

const std::string NullText = "无/Null";

const int MapMaxSize = 55;//定义地图内存的大小 

//-1.死亡页面 0.初始页面 1.游戏页面 2.设置页面  
int cz = 0;//显示的页面 

int BoolTheGame;//-----------游戏是否运行 
int BoolDebug = 1;//---------日志Debug是否开启
int BoolZbxs = 1;//----------坐标显示是否开启 
int BoolGameRunTime = 1;//---游戏时刻显示是否开启 
int BoolFPS = 1;//-----------FPS显示是否开启
int BoolMapMessage = 1;//----地图信息显示是否开启 

int FPS;//-------------------FPS数值 
int FPSWeek = 10;//----------FPS循环周期 
long GameRunTime = 0;//------游戏时刻 
clock_t LastTime;//----------上次时间 
int JGTime;//----------------间隔的时间

struct SP{//StructPlayer 的缩写   玩家信息 
	int LastX,LastY;//-------玩家上次的位置 
	int NextX,NextY;//-------玩家行动的位置 
	int myx,myy;//-----------玩家位置 
	int Life;//--------------玩家生命
	int MaxLife;//-----------玩家最大生命 
	std::string MapName;//---当前地图序号 
};
SP Player;

std::string ClsText;//清屏str 


//地图文件======================================================================
//0."  "空气   1."##"墙   2."门"门   9."%%"陷阱   10."敌"敌人   11."人"一个人 
struct SM {//StructMap 的缩写 地图信息 
	int maxx, maxy;//-------地图大小 
	std::string wjname;//---文件名字 
	std::string name;//-----地图名字 
	std::string Author = NullText;//地图作者

	int Data[MapMaxSize][MapMaxSize];//地图数据
	//	int SJData[20][40];//旧事件存储格式
	struct SJData {//事件数据
		int SJNum;//特殊事件数量 
		int CSNum;//传送事件数量 
		int DHNum;//对话事件数量 
		struct Data {//=====事件参数 
			int X, Y;//触发位置 
			int Type;//事件类型 
			int Hand;//事件序号 
		}Data[100];
		struct CSData {//=====传送数据 
			int X, Y;//传送目标位置
			std::string MapName;//目标名字
		}CSData[100];
		struct DHData {//=====对话数据 
			std::string ObjectName;//对话者名称 
			int DHNum;//对话数量 
			struct DH {//对话 
				std::string Text;//对话内容
				int HDNum;//回答数量 
				struct HD {//回答 
					std::string Text;//回答内容
					int JG;//回答结果 
				}HD[8];
			}DH[50];
		}DHData[100];
	}SJData;
}Map;


std::string MapName[201];//地图列表
int MapNum;//地图列表 的地图数量

std::string ErrorText;//错误信息
int ErrorNum;//错误数量

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

std::string wstring_string(std::wstring wstr){//------------------------------------------------ wstring --> string-8   (来自AI)
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string strr(len, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &strr[0], len, NULL, NULL);
	strr.pop_back();//去除最后的一个字符   在此是 ' '<---空格 
	return strr;
}

std::string to_utf8(const std::string& ansi) {//------------------------------------------------ String --> UFT-8   (来自AI)
	// 1. ANSI → UTF?16 (获取所需长度，不含 null)
	int wlen = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), nullptr, 0);
	if (wlen == 0) return "";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), &wstr[0], wlen);

	// 2. UTF?16 → UTF?8 (获取所需长度，不含 null)
	int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
	if (ulen == 0) return "";
	std::string utf8(ulen, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), &utf8[0], ulen, nullptr, nullptr);
	return utf8;   // utf8.size() == ulen，无多余 null
}

std::string to_Ansi(const std::string& utf8Str) {//------------------------------------------------ UFT-8 --> String   (来自AI)
	if (utf8Str.empty()) return {};

	// 1. UTF-8 -> UTF-16
	int wcharLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	if (wcharLen == 0) return {};
	std::wstring wstr(wcharLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wcharLen);

	// 2. UTF-16 -> ANSI (CP_ACP)
	int ansiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (ansiLen == 0) return {};
	std::string ansiStr(ansiLen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &ansiStr[0], ansiLen, nullptr, nullptr);

	// 返回时去掉末尾的 '\0'
	return ansiStr.substr(0, ansiLen - 1);
}

void Debug(std::string str){//<---------------------------------------------------------日志
	
	if(str == "sss"){//清空日志 
    	std::ofstream outfile(DebugWay);
		outfile<<"==================================日志==================================";
		if(!BoolDebug) outfile<<"\n日志以记录关闭 可以在设置中打开此功能";
		return;
	}
	
	if(!BoolDebug)return;//如果日志记录关闭 就退出函数 
	
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
	ct(0, 0, ClsText);
	ct(0, 0, "");
}

void CDW(const std::wstring& str){//<-------------------------------------------------------创建文件夹 
    CreateDirectoryW(str.c_str(),NULL);
}

bool FFFW(const std::wstring& str, const std::string FindMapName){//<-------------------------------------------------------获取文件 
	bool Find = true;
	Debug("搜索地图文件==="); 
	WIN32_FIND_DATAW num;//创建结构体以暂时保存地图名称 
	HANDLE Name = FindFirstFileW((str + L"\\*").c_str(), &num);// 来自AI 获取名称
	int i = 0;//定义记号 
	while(FindNextFileW(Name,&num)){//是否还有文件 
		std::string strr = wstring_string(num.cFileName);//获取名称
		MapName[i] = strr;//将地图名称写入数组
		if (Find && FindMapName == strr) Find = false;
		Debug("第"+std::to_string(i)+"个地图文件:"+strr);
		i++;//记号加一 
	}
    MapNum = i-1;//记录地图数量 
    
    FindClose(Name);//结束寻找 
    Debug("搜索了"+std::to_string(MapNum)+"个文件");
    Debug("===搜索地图文件");
	return !Find;
}

void Error(const std::string Text, const std::string Type) {;//<-------------------------------------------------------------------------错误显示
	if (Type == "W") {
		Debug("发生错误 " + std::to_string(ErrorNum) + ". " + Text);
		ErrorText +="| " + std::to_string(ErrorNum) + ". " + Text + "\n";
		ErrorNum++;
	}
	else if (Type == "R") {
		bool BoolError = false;
		while (1) {
			std::string str;
			str += "发生错误\n";
			str += "发现错误数量: " + std::to_string(ErrorNum) + "\n\n";
			if (BoolError)str += "错误信息:\n" + ErrorText + "\n";
			str += "请联系创作者修复\n";
			str += "\n\n";
			str += "空格.查看详细\n";
			str += "r.返回\n";
			cls();
			ct(0, 0, str);
			char input = _getch();
			if (input == 'r' || input == 'R')break;
			else if (input == ' ')BoolError = BoolError ? false : true;
		}
		ErrorText = "";
		ErrorNum = 0;
	}
}

bool OpenJson(const std::string WJWay, const std::string WJName, nlohmann::json& jsin) {//<--------------------------------------------------------------打开JSON 分析
	std::ifstream Open(WJName);
	if (!Open.is_open()) {//如果文件打开失败
		std::string T = FFFW(DataParent, WJName) ? "未知" : "未找到此文件";
		Error("无法打开文件: " + WJName + " 错误信息: " + T, "W");
		return false;//输出 假
	}
	try {//如果没有错误
		Open >> jsin;
		return true;//输出 真
	}
	catch (const nlohmann::json::parse_error& er) {//如果无法解析
		Error("无法解析JSON文件 " + WJName + " 错误信息: JSON文件格式错误 "+"错误位置: " + std::to_string(er.byte), "W");
		return false;//输出 假
	}
	return false;
}

void czdata(std::string xz){//<--------------------------------------------------------------重置数据 
	if(xz == "GD"){
		Debug("重置游戏数据====");

		Player.myx = 1;//X
		Player.myy = 1;//Y
		Player.LastX = 1;//LastX
		Player.LastY = 1;//LastY
		Player.NextX = 1;//NextX
		Player.NextY = 1;//NextY
		Player.Life = 100;//血条
		Player.MaxLife = 100;//最大血量
		
		GameRunTime = 0;//游戏时刻
		
		Player.MapName = BeginMap;//初始地图

		Debug("====重置游戏数据");
	}else if(xz == "SD"){
		
	}
}

bool GameMapData(std::string mapname){//<---------------------------------------------读取地图文件 
	Debug("读取地图=====");
	Debug("文件名称:"+mapname); 
	
	//std::string mapway = wstring_string(MapDataParent)+ "/" + mapname;//计算地图路径 
	//Debug("地图路径:"+mapway); 
	//std::ifstream PFMapData(mapway);
	//
	//PFMapData>>Map.Name;//读取地图名字
	//Debug("地图名称:"+Map.Name);
	//
	//PFMapData>>Map.MaxX>>Map.MaxY;//读取地图大小 
	//for(int i=0;i<Map.MaxY;i++){//读取地图数据 
	//	for(int j=0;j<Map.MaxX;j++){
	//		PFMapData>>Map.Data[i][j];
	//	}
	//}
	//
	//int num;
	//
	//PFMapData>>num;//读取地图特殊事件数量 
	//Debug("特殊事件个数:"+std::to_string(num));
	//Map.SjNum = num;//读取地图特殊事件数量 
	//for(int i=0;i<num;i++){
	//	PFMapData>>Map.TSSJ[i][0]>>Map.TSSJ[i][1];//X Y
	//	PFMapData>>Map.TSSJ[i][2];//事件类型
	//	if(Map.TSSJ[i][2] == 1){//1===传送 
	//		PFMapData>>Map.TSSJ[i][3]; 
	//		PFMapData>>Map.TSSJ[i][4]>>Map.TSSJ[i][5]; 
	//	}else if(Map.TSSJ[i][2] == 2){//2===对话 
	//		PFMapData>>Map.TSSJ[i][3];//对话人的名字
	//		int dhnum;//对话数量
	//		PFMapData>>dhnum;//对话数量
	//		Map.TSSJ[i][4] = dhnum;//对话数量
	//		int Hand = 5;
	//		for(int k=1;k<=dhnum;k++){
	//			PFMapData>>Map.TSSJ[i][Hand];//对话内容 
	//			Hand++;
	//			int hdnum;//回答数量 
	//			PFMapData>>hdnum;//回答数量 
	//			Map.TSSJ[i][Hand] = hdnum;//回答数量 
	//			Hand++;
	//			for(int j=1;j<=hdnum;j++){
	//				PFMapData>>Map.TSSJ[i][Hand];//回答内容
	//				Hand++;
	//				PFMapData>>Map.TSSJ[i][Hand];//此回答结果 
	//				Hand++;
	//			}
	//		}
	//	}
	//}
	//
	//int strnum;
	//PFMapData>>strnum;//获取地图文本数量
	//for(int i=0;i<strnum;i++){//读取地图文本
	//	PFMapData>>Map.Text[i];
	//}
	//

	//json
	std::string mapway = wstring_string(DataParent) + "/" + mapname;//计算地图路径 
	Debug("文件路径: " + mapway);
	nlohmann::json js;

	if (!OpenJson(mapname, mapway, js)) {//读取地图文件
		Error("", "R");
		return false;
	}
	
	if (js.contains("name"))Map.name = to_Ansi(js["name"]); else Map.name = NullText;//地图名称
	if (js.contains("author"))Map.Author = to_Ansi(js["author"]); else Map.Author = NullText;//作者名称
	if (js.contains("SizeX"))Map.maxx = js["SizeX"]; else Error("地图文件: " + mapway + " 缺失数据 SizeX", "W");//地图尺寸
	if (js.contains("SizeY"))Map.maxy = js["SizeY"]; else Error("地图文件: " + mapway + " 缺失数据 SizeY", "W");//地图尺寸
	
	if (js.contains("MapData")) {//地图数据
		auto MapData = js["MapData"];
		if (MapData.size() >= Map.maxy) {//检测数据是否完整 Y
			for (int i = 0; i < Map.maxy; i++) {
				if (MapData[i].size() >= Map.maxx) {//检测数据是否完整 X
					for (int j = 0; j < Map.maxx; j++) {
						Map.Data[i][j] = MapData[i][j];
					}
				}
				else Error("地图文件: " + mapway + " 地图数据部分缺失 第" + std::to_string(i) + "行 SizeX " + std::to_string(Map.maxy) + "   实际只有 " + std::to_string(MapData[i].size()), "W");
			}
		}
		else Error("地图文件: " + mapway + " 地图数据部分Y缺失 SizeY " + std::to_string(Map.maxy) + "   实际只有 " + std::to_string(MapData.size()), "W");
	}
	else Error("地图文件: " + mapway + " 缺失数据 MapData", "W");

	if (!js.contains("SJData"))Error("地图文件: " + mapway + " 缺失数据 SJData", "W");//键 SJData
	if (js["SJData"].contains("sjnum"))Map.SJData.SJNum = js["SJData"]["sjnum"]; else Error("地图文件: " + mapway + " 缺失数据 SJData - sjnum", "W");//事件数量
	for (int i = 0; i < Map.SJData.SJNum; i++) {
		std::string S1 = "SJ" + std::to_string(i);//---定义
		if (!js["SJData"].contains(S1)) {//键 SJData     SJi
			Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1, "W");
		}//不存在 ---上
		else {//存在 ---下
			if (!js["SJData"][S1].contains("x")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - x", "W");
			else Map.SJData.Data[i].X = js["SJData"][S1]["x"];//触发X
			if (!js["SJData"][S1].contains("y")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - y", "W");
			else Map.SJData.Data[i].Y = js["SJData"][S1]["y"];//触发Y
			if (!js["SJData"][S1].contains("type")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - type", "W");
			else {//事件的数据
				Map.SJData.Data[i].Type = js["SJData"][S1]["type"];//事件类型
				if (Map.SJData.Data[i].Type == 1) {//传送事件
					if(!js["SJData"][S1].contains("next")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - next", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].MapName = to_Ansi(js["SJData"][S1]["next"]);//传送的地图名字
					if (!js["SJData"][S1].contains("nextX")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextX", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].X = js["SJData"][S1]["nextX"];//传送的坐标 X
					if (!js["SJData"][S1].contains("nextY")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextY", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].Y = js["SJData"][S1]["nextY"];//传送的坐标 X
					Map.SJData.Data[i].Hand = Map.SJData.CSNum;
					Map.SJData.CSNum++;
				}
				else if (Map.SJData.Data[i].Type == 2) {//对话事件
					if(!js["SJData"][S1].contains("object")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - object", "W");
					else Map.SJData.DHData[Map.SJData.DHNum].ObjectName = to_Ansi(js["SJData"][S1]["object"]);//对话者
					if (!js["SJData"][S1].contains("dhnum")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - dhnum", "W");
					else {//对话的数据
						Map.SJData.DHData[Map.SJData.DHNum].DHNum = js["SJData"][S1]["dhnum"];//对话的数量
						for (int j = 0; j < Map.SJData.DHData[Map.SJData.DHNum].DHNum; j++) {
							std::string D2 = "DH" + std::to_string(j);//---定义
							if (!js["SJData"][S1].contains(D2)) {//键 SJData     SJi - DHj
								Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2, "W");
							}//不存在 ---上
							else {//存在 ---下
								if (!js["SJData"][S1][D2].contains("text")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - text", "W");
								else Map.SJData.DHData[Map.SJData.DHNum].DH[j].Text = to_Ansi(js["SJData"][S1][D2]["text"]);//对话内容
								if (!js["SJData"][S1][D2].contains("hdnum")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - hdnum", "W");
								else {//此对话 回答的数据
									Map.SJData.DHData[Map.SJData.DHNum].DH[j].HDNum = js["SJData"][S1][D2]["hdnum"];//此对话 回答的数量
									for (int k = 0; k < Map.SJData.DHData[Map.SJData.DHNum].DH[j].HDNum; k++) {
										std::string H3 = "HD" + std::to_string(k);//---定义
										if (!js["SJData"][S1][D2].contains(H3)) {//键 SJData     SJi - DHj - HDk
											Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3, "W");
										}//不存在 ---上
										else {
											if (!js["SJData"][S1][D2][H3].contains("text")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3 + " - text", "W");
											else Map.SJData.DHData[Map.SJData.DHNum].DH[j].HD[k].Text = to_Ansi(js["SJData"][S1][D2][H3]["text"]);//此回答的内容
											if (!js["SJData"][S1][D2][H3].contains("effect")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3 + " - effect", "W");
											else Map.SJData.DHData[Map.SJData.DHNum].DH[j].HD[k].JG = js["SJData"][S1][D2][H3]["effect"];//此回答的效果/结果
										}
									}
								}
							}
						}
					}
					Map.SJData.Data[i].Hand = Map.SJData.DHNum;
					Map.SJData.DHNum++;
				}
			}
		}
	}

	cls();
	if (ErrorNum != 0) {
		Error("", "R");
		return false;
	}
	Debug("=====读取地图");
	return true;
}

void GameData(std::string xz, int num){//<-------------------------------------------保存/读取游戏数据 
	if(xz == "GDW"){//GameData
		Debug("保存游戏数据==========");
		
		//std::string str;
		//
		//str += std::to_string(num)+"\n";
		////Player.MapHand myx   myy   Life   MaxLife
		//str += Player.MapName+" "+std::to_string(Player.myx)+" "+std::to_string(Player.myy)+" "+std::to_string(Player.Life)+" "+std::to_string(Player.MaxLife)+"\n";
		//str += std::to_string(GameRunTime)+"\n";
		//
		//std::ofstream PFR(GameDataWay);//写入文件
		//PFR<<str;

		nlohmann::json js;
		js["x"] = Player.myx;
		js["y"] = Player.myy;
		js["lastx"] = Player.LastX;
		js["lasty"] = Player.LastY;
		js["nextx"] = Player.NextX;
		js["nexty"] = Player.NextY;

		js["life"] = Player.Life;
		js["maxlife"] = Player.MaxLife;

		js["mapname"] = to_utf8(Player.MapName);

		js["gameruntime"] = GameRunTime;

		std::ofstream outjs(GameDataWay);
		outjs << js.dump(4);
		outjs.close();
		Debug("==========保存游戏数据");
	}else if(xz == "GDR"){//GameData
		
		Debug("读取游戏数据==========");
		
		std::ifstream jsin(GameDataWay);//用PFW表示Debug.txt文件 
		if (jsin.is_open()) {
			nlohmann::json js;
			jsin >> js;
			Player.myx = js["x"];//获取位置
			Player.myy = js["y"];
			Player.LastX = js["lastx"];
			Player.LastY = js["lasty"];
			Player.NextX = js["nextx"];
			Player.NextY = js["nexty"];
			Player.Life = js["life"];//获取生命
			Player.MaxLife = js["maxlife"];
			Player.MapName = to_Ansi(js["mapname"]);//获取地图文件名
			GameRunTime = js["gameruntime"];//获取游戏时刻
			jsin.close();
		}
		else {
			Debug("文件打开失败: "+ GameDataWay);
			czdata("GD");
			GameData("GDW", 1);
		}
		//int num;
		//PFW>>num;
		//if(num){
		//	PFW>>Player.MapName>>Player.myx >>Player.myy >>Player.Life >>Player.MaxLife;
		//	PFW>>GameRunTime;
		//}else{
		//	czdata("GD");
		//}
		Debug("==========读取游戏数据");

		
	}else if(xz == "SDW"){//SetData
		
		Debug("保存设置数据");
		
		std::string str;
		
		str += "1\n";
		str += std::to_string(BoolDebug)+" "+std::to_string(BoolZbxs)+" "+std::to_string(BoolGameRunTime)+" "+std::to_string(BoolFPS)+" "+std::to_string(BoolMapMessage)+"\n";
		str += std::to_string(FPSWeek)+"\n";
		
		std::ofstream PFR(SetDataWay);//写入文件
		PFR<<str;
		
	}else if(xz == "SDR"){//SetData
		
		Debug("读取设置数据");
		
		std::ifstream PFW(SetDataWay);//用file表示SetData.txt文件 
		
		int num;
		PFW>>num;
		if(num){
			PFW>>BoolDebug;
			PFW>>BoolZbxs >>BoolGameRunTime >>BoolFPS >>BoolMapMessage;
			PFW>>FPSWeek;
		}else{
			czdata("SD");
		}
	}
	
}


bool gamestart(){//<----------------------------------------------------------游戏初始化
	GameData("GDR",-1);//读取游戏数据
	return GameMapData(Player.MapName);//加载对应地图 返回false说明地图打开失败
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
	
	
	if(BoolZbxs){//=====坐标显示 
		str += "位置:" + std::to_string(Player.myx) + " " + std::to_string(Player.myy) + "   ";
		str += "Last:" + std::to_string(Player.LastX) + " " + std::to_string(Player.LastY) + "   ";
		str += "Next:" + std::to_string(Player.NextX) + " " + std::to_string(Player.NextY) + "          \n";
	}
	
	if(BoolGameRunTime){//游戏时刻显示 
		str +="游戏时刻:"+std::to_string(GameRunTime)+"\n";
	}
	
	if(BoolFPS){//=====FPS显示 
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
	
	if(BoolMapMessage){//=====地图文件信息显示 
		str+="文件名:"+Player.MapName+" 大小XY:"+std::to_string(Map.maxx)+"*"+std::to_string(Map.maxy)+" 事件:"+std::to_string(Map.SJData.SJNum);
		str+="\n";
	}
	
	
	for(int i=0;i<Map.maxy;i++){//地图显示 
		for(int j=0;j<Map.maxx;j++){
			int num = Map.Data[i][j];
			
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
	
	for(int i=0;i<=Map.maxx;i++) str+="  ";
	str+="\n";
	
	str+=Map.name;//地图名称 
	str+="\n";
	
	ct(0,0,str);
}




void playerinput(char input){//<---------------------------------------------------玩家输入 
	
	if(input == 'r' || input == 'R'){
		GameData("GDW",1);
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
	
	
	int num = Map.Data[Player.myy + yy][Player.myx + xx];
	
	if(num == 0 || num == 2 || num == 9){
		
		Player.LastX = Player.myx;
		Player.LastY = Player.myy;
		
		Player.myx += xx;
		Player.myy += yy;
	}
	
}

void shpd(){//<---------------------------------------------------------------------伤害判定 
	
	if(Map.Data[Player.myy][Player.myx] == 9){//陷阱判定 
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
				if(Map.Data[numY+i][numX+j] == 10){
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
	int num = Map.SJData.SJNum; //事件数量 
	for(int i=0;i<num;i++){
		int SJX,SJY,SJLX;//事件X 事件Y 事件类型 
		SJX = Map.SJData.Data[i].X;//获取发生位置X 
		SJY = Map.SJData.Data[i].Y;//获取发生位置Y 
		if (SJX == Player.NextX && SJY == Player.NextY && (Player.NextX != Player.LastX || Player.NextY != Player.LastY)) {//当玩家位置符合时 并且不等于上次的坐标
			Player.LastX = Player.NextX;//将玩家(上次的坐标)设置为玩家的行动坐标 以防止重复触发 
			Player.LastY = Player.NextY;//将玩家(上次的坐标)设置为玩家的行动坐标 以防止重复触发 
			SJLX = Map.SJData.Data[i].Type;
			if(SJLX == 1){//===================================(1)传送事件 
				Player.myx = Map.SJData.CSData[Map.SJData.Data[i].Hand].X;//X
				Player.myy = Map.SJData.CSData[Map.SJData.Data[i].Hand].Y;//Y
				Player.MapName = Map.SJData.CSData[Map.SJData.Data[i].Hand].MapName;//传送的目标
				GameMapData(Player.MapName);//更新地图内存 
				cls();//清屏
			}else if(SJLX == 2){//=============================(2)对话事件
				std::string DHR;//对话人
				DHR = Map.SJData.DHData[Map.SJData.Data[i].Hand].ObjectName;//获取对话人
				int DHnum;//对话数量
				DHnum = Map.SJData.DHData[Map.SJData.Data[i].Hand].DHNum;//获取对话数量
				int DHHand = 0;//正在显示对话的指针
				while(1){//显示对话 
					std::string str;
					str+="\n"+DHR+":\n";//打印对话者的名字 
					str+="|   "+Map.SJData.DHData[Map.SJData.Data[i].Hand].DH[DHHand].Text + "\n\n";//获取对话的文本 
					int HDnum = Map.SJData.DHData[Map.SJData.Data[i].Hand].DH[DHHand].HDNum;//获取回答的数量
					if(HDnum == 0){//当没有回答时 
						str+="f.继续";
					}else{//当有回答时 
						str+="你的回答:\n";
						for(int j=0;j<HDnum;j++){//展示回答 
							str+=std::to_string(j+1)+".";//打印序号 
							str+=Map.SJData.DHData[Map.SJData.Data[i].Hand].DH[DHHand].HD[j].Text;//打印回答内容
							str+="\n"; 
						}
					}
					cls();//清空屏幕 
					ct(0,2,str);//打印缓存
					char input =_getch();//获取玩家输入 
					if(HDnum == 0){//当没有回答时 
						if(input == 'f' || input =='F'){//当玩家输入F时 
							DHHand++;//切换下一个对话 
						}
					}else{//当有回答时
						if(input >= '1' && input <= '9'){//检查玩家输入是否为数字 
							int INPUT = input-'0';//将 char 转换为 int
							if(INPUT <= HDnum){//检查玩家输入是否在 回答数量 之内
								int GOHand = Map.SJData.DHData[Map.SJData.Data[i].Hand].DH[DHHand].HD[INPUT-1].JG;//获取跳转对话的序号
								if(GOHand == 0) DHHand++;//当 跳转对话的序号 为(0) 将 下一页 设置为显示 
								else if(GOHand == -1) break;//当 跳转对话的序号 为(-1) 退出对话 
								else DHHand = GOHand; //都不是 将 指定的对话 设置为显示 
							} 
						}
					}
					if(DHHand >= DHnum) break;//当 当前对话超过对话数量时 退出对话 
				}
			}
			cls();//清空屏幕 
			GameData("GDW", 1);
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
		GameData("GDW",1);
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
	if(cz == 1) gameon();//游戏页面====================(1) 
	if(cz == 0){//起始页面====================(0) 
    	while(1) {//input
			std::string str;
			str+= "冒险之路\n1.继续游戏\n2.设置\n3.关于\n\nr.退出游戏";
			
			cls();
			ct(0,0,str);
	        char input=_getch();
			if(input == '1') {
				if (gamestart())cz = 1;//如果初始化成功 切换到游戏界面
	            break;
	        }else if(input == '2') {
				cz=2;
				GameData("SDR",-1);
	            break;
	        }else if(input == '3') {
				cz=3;
	            break;
	        }else if(input == 'r' || input == 'R') {
	        	BoolTheGame=0;
	            return;
	        }
		}
	}else if(cz == 2){//设置页面====================(2) 
		while(1){
			std::string str;
			
			str+="设置\n";
			str+="1.日志记录          当前:";if(BoolDebug)       str+="开\n";else str+="关\n";
			str+="2.FPS显示           当前:";if(BoolFPS)         str+="开\n";else str+="关\n";
			str+="3.FPS刷新间隔       当前:";                    str+="每"+std::to_string(FPSWeek)+"游戏时刻更新\n";
			str+="4.游戏时刻显示      当前:";if(BoolGameRunTime) str+="开\n";else str+="关\n";
			str+="5.坐标显示          当前:";if(BoolZbxs)        str+="开\n";else str+="关\n";
			str+="6.地图信息显示      当前:";if(BoolMapMessage)      str+="开\n";else str+="关\n";
			
			str+="\n\n\n";
			str+="r.返回";

			cls();
			ct(0,0,str);
			
	        char input=_getch();
	        if(input == '1'){//选项 日志记录-----(1) 
				BoolDebug = BoolDebug ? false : true;
			}else if(input == '2'){//选项 FPS显示-----(2)
	        	BoolFPS = BoolFPS ? false : true;
			}else if(input == '3'){//选项 FPS显示-----(3)
	        	while(1){
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

					cls();
	        		ct(0,0,str);
	        		
	        		char input1=_getch();
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
	        				str+="FPS刷新间隔 自定义\n";
	        				str+="请输入 数值\n";
	        				str+="范围 数值 >= 1\n";
	        				cls();
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
									char input2 = _getch();
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
	        	BoolGameRunTime = BoolGameRunTime ? false : true;
			}else if(input == '5'){//选项 坐标显示-----(5)
	        	BoolZbxs = BoolZbxs ? false : true;
			}else if(input == '6'){//选项 地图信息显示-----(6)
	        	BoolMapMessage = BoolMapMessage ? false : true;
			}else if(input == 'r' || input == 'R') {//选项 返回-----(r) 
				GameData("SDW",-1);
	        	cz=0;
	            break;
	        }
		}
	}else if(cz == 3){//关于====================(3) 
		while(1){
			std::string str;
			str+="关于\n"; 
			str+="WASD--控制角色\n";
			str+="R-----返回\n";
			
	        str+="\n\n\n";
			str+="r.返回";
			
			cls();
			ct(0,0,str);
			
			char input=_getch();
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
			char input=_getch();
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
	for(int i=1;i<=100;i++)ClsText+="                                                                                                                                    \n";//补充清屏 
}

int main() {//----------------------------------------------------------------------main 
	
	GameData("SDR",-1);
	
	Debug("sss");
	Debug("初始化");
	Begin();
	
	Debug("游戏开始==================");
    
	BoolTheGame = 1;
    while(BoolTheGame) {//main
    	ymxs();
    }
	Debug("=============游戏正常退出");
    return 0;
}
//
//
//
//
//
//
//
//
//void ksvoid(){//<--------------------------------------------------------------快速定位函数 
//	ct(0,0,"");//<-----------------------------------------指定打印
//	Debug("");//<------------------------------------------日志
//	cls();//<----------------------------------------------清屏函数 
//	CDW(L"");//<-------------------------------------------创建文件夹 
//	FFFW(L"");//<------------------------------------------获取文件 
//	czdata("");//<-----------------------------------------重置数据 
//	GameMapData("");//<------------------------------------读取地图文件
//	GameData("",-1);//<------------------------------------保存/读取游戏数据 
//	gamestart();//<----------------------------------------游戏初始化
//	pmsx();//<---------------------------------------------屏幕刷新 
//	playerinput('0');//<-----------------------------------玩家输入 
//	shpd();//<---------------------------------------------伤害判定 
//	swpd();//<---------------------------------------------死亡判定
//	MapSJ();//<--------------------------------------------特殊事件 
//	Lifeup();//<-------------------------------------------生命自然回复 
//	SaveData(0);//<----------------------------------------游戏自动保存 
//	gameon();//============================================游戏主控 
//	ymxs();//<---------------------------------------------显示页面 
//	Begin();//---------------------------------------------初始化
//	main();//----------------------------------------------main 
//}
//
//
