/*=========================================
--- 项目名称: 冒险之路 --------------------
--- 项目环境: windows C++11 ---------------
--- 编译软件: Visual Studio ---------------
-------------------------------------------
--- 项目作者: 平凡0_0 ---------------------
--- 哔哩哔哩: https://b23.tv/mGuvqJi ------
--- 项目初创时间: 2026/1/23 ---------------
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

/*------ 版本概述 --------------------------------------------------------

EasyX 库的使用
	1.从 "graphics.h" 中引入 EasyX 库 
	2.使用 EasyX 库创建窗口 绘制图像 显示文本 
	3.使用 EasyX 库获取鼠标键盘输入 
	4.使用 EasyX 库进行图像处理 






	
	
	
	
	
	
	/*------ 版本概述 --------------------------------------------------------
体验层面
	1.玩家可以制作属于自己的地图 
	2.多数时间格式 更为 json
	3.代码环境变更 "Dev-C++" ---> "Visual Studio"
	4.代码编码变更 "ANSI" ---> "UTF-8 BOM"
	5.新添显示内容 位置 Last 与 next
	6.新增错误与警告提示
	7.新增错误与警告日志功能
	8.新增错误与警告显示功能
	9.地图数据缺失提示 避免游戏崩溃
代码层面
	1.存储地图数据结构改变
	2.代码长度增加     <---废话
	3.添加大量注释
	4.代码结构改变     <---又是废话
其他层面
	1.地图编辑器(基本完工) 
------------------------------------------------------------------------*/









#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<conio.h>
#include<cstdlib>
#include<windows.h>
#include"json.hpp"
#include<graphics.h>


#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:26495)

const std::wstring DataWayParent = L"GameData";//----文件夹路径
const std::wstring SetWayParent  = L"Debug";//-------文件夹路径
const std::wstring DataParent    = L"Data";//--------文件夹路径 


const std::string GameDataWay = "GameData/GameData.json";//-----文件路径及名称  
const std::string SetDataWay  = "GameData/SetData.json";//------文件路径及名称  
const std::string DebugWay    = "Debug/Debug.txt";//------------文件路径及名称   
const std::string ErrorWay    = "Debug/Error.txt";//------------文件路径及名称   
const std::string WarnWay     = "Debug/Warn.txt";//-------------文件路径及名称  

const std::string BeginMap = "冒险之路.json";//---------------初始的地图 

const std::string NULLTEXT = "无/Null";

const int MapMaxSize = 105;//定义地图内存的大小 

std::string YM;//显示的页面 

bool BoolTheGame     = true;//------游戏是否运行  
bool BoolDebug       = true;//------日志Debug是否开启 
bool BoolZbxs        = true;//------坐标显示是否开启  
bool BoolGameRunTime = true;//------游戏时刻显示是否开启  
bool BoolFPS         = true;//------FPS显示是否开启 
bool BoolMapMessage  = true;//------地图信息显示是否开启  
bool BoolWarn        = false;//-----是否显示警告状态 

int FPS;//------------------------FPS数值 
int FPSWeek = 10;//---------------FPS循环周期 
long long GameRunTime = 0;//------游戏时刻 

struct StructPlayer {//玩家信息
	int LastX,LastY;//---------玩家上次的位置
	int NextX,NextY;//---------玩家行动的位置
	int myx,myy;//-------------玩家位置
	float CameraX,CameraY;//---摄像机位置
	float PmX,PmY;//-----------显示的坐标
	float Life;//--------------玩家生命
	float MaxLife;//-----------玩家最大生命
	float Speed = 10;//--------玩家速度
	std::string MapName;//-----当前地图序号
}Player;

struct StructTime {//时间信息
	clock_t LastTime;//---------------上次时间
	clock_t NowTime;//----------------现在时间
	int JGTime;//---------------------间隔的时间
}Time;

struct StructInput {//输入信息 
	char up    = 'W';//上
	char down  = 'S';//下
	char left  = 'A';//左
	char right = 'D';//右
	char returnn= 27;//返回
}PlayerInput;

struct SrructWin {//窗口信息 
	int WinX = 800;//窗口宽度 
	int WinY = 600;//窗口高度
	int Size = 20;//每个格子的像素大小
	int LastSize = 0;//上次每个格子的像素大小
}Win;

ExMessage Mouse;//定义鼠标数据
int MouseX;//鼠标坐标
int MouseY;

std::string ClsText;//清屏str 

//地图文件======================================================================
//0."  "空气   1."##"墙   2."门"门   9."%%"陷阱   10."敌"敌人   11."人"一个人 
struct SM {//StructMap 的缩写 地图信息 
	int maxx, maxy;//-------地图大小 
	std::string wjname;//---文件名字 
	std::string name;//-----地图名字 
	std::string Author = NULLTEXT;//地图作者
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

std::string WarnText;//警告信息
int WarnNum;//警告数量

//======================================================================地图文件


IMAGE ImgWell;
IMAGE ImgPlayer;


//===========
void ct(int x, int y, std::string str){//<-------------------------------------------------------指定打印 增加FPS 
    COORD zb;zb.X = x;zb.Y = y;//定义坐标
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), zb);//设置光标位置
	std::cout << str;//定义文本
	std::cout.flush(); //使文本立即显示
}
//==========
//==========
//==========
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
//==========
//==========
//==========
void Debug(std::string str){//<---------------------------------------------------------日志
	if(!BoolDebug)return;//如果日志记录关闭 就退出函数  
	if(str == "sss"){//清空日志  
    	std::ofstream DubugOut(DebugWay);//打开文件 
		DubugOut <<"==================================日志==================================\n";
		if(!BoolDebug) DubugOut <<"\n日志以记录关闭 可以在设置中打开此功能\n";
		return;//退出 
	}
	std::string strr; //定义文本 
	strr += "\n";//写入换行 
	double NowTime = clock();//获取程序运行时间 
	strr += "[" + std::to_string(NowTime / 1000) + "] ";//写入时间 
    strr+=str;//写入Debug  
    std::ofstream DubugOut(DebugWay,std::ios::app);
	DubugOut<<strr;//写入文件 
}
 
void DebugError(std::string str) {//<---------------------------------------------------------错误日志 
	if (!BoolDebug)return;//如果日志记录关闭 就退出函数  
	if (str == "sss") {//清空错误日志  
		std::ofstream DubugOut(ErrorWay);//打开文件
		DubugOut << "==================================错误日志==================================";
		if (!BoolDebug) DubugOut << "\n错误日志以记录关闭 可以在设置中打开此功能";
		return;//退出 
	}
	std::string strr;//定义文本
	strr += "\n";//写入换行
	double NowTime = clock();//获取程序运行时间
	strr += "[" + std::to_string(NowTime / 1000) + "] ";//写入时间
	strr += str;//写入DebugError
	std::ofstream DubugOut(ErrorWay, std::ios::app);//打开文件
	DubugOut << strr;//写入文件
}

void DebugWarn(std::string str) {//<---------------------------------------------------------警告日志
	if (!BoolDebug)return;//如果日志记录关闭 就退出函数 
	if (str == "sss") {//清空警告日志 
		std::ofstream DubugOut(WarnWay);//打开文件
		DubugOut << "==================================警告日志==================================";
		if (!BoolDebug) DubugOut << "\n警告日志以记录关闭 可以在设置中打开此功能";
		return;//退出
	}
	std::string strr;//定义文本
	strr += "\n";////写入换行
	double NowTime = clock();//获取程序运行时间
	strr += "[" + std::to_string(NowTime / 1000) + "] ";//写入时间
	strr += str;//写入DebugWarn
	std::ofstream DubugOut(WarnWay, std::ios::app);//打开文件
	DubugOut << strr;//写入文件
}
//==========
//==========
//==========
void cls(){//<-------------------------------------------------------------------------------清屏函数  "极大"的减少了屏幕刷新时的闪烁 
	ct(0, 0, ClsText);//打印大量空格 
	ct(0, 0, "");//定位关闭至左上角 
}
//==========
//==========
//==========
void CDW(const std::wstring& str){//<-------------------------------------------------------创建文件夹 
    CreateDirectoryW(str.c_str(),NULL);//创建文件夹 
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
//==========
//==========
//==========
void Error(const std::string Text, const std::string Type) {
	;//<--------------------------------------------------------------------------------------------------错误显示
	if (Type == "W") {//写入错误
		DebugError("发生错误 " + std::to_string(ErrorNum) + ". " + Text);
		ErrorText += "| " + std::to_string(ErrorNum) + ". " + Text + "\n";
		ErrorNum++;//错误数量加一
	}
	else if (Type == "R") {//显示错误
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
			break;//            <  ---------
			char input = _getch();
			if (input == 'r' || input == 'R') break;
			else if (input == ' ')BoolError = BoolError ? false : true;
		}
		Error("", "sss");
	}
	else if (Type == "sss") {//清空错误
		Debug("发现错误数:  " + std::to_string(ErrorNum));
		DebugError("目前发现错误数:  " + std::to_string(ErrorNum));
		DebugError("======================================================");
		ErrorText = "";
		ErrorNum = 0;
	}
	cls();//清屏
}

void Warn(const std::string Text, const std::string Type) {//<-------------------------------------------------------------------------警告显示
	if (Type == "W") {//写入警告
		DebugWarn("发生警告 " + std::to_string(WarnNum) + ". " + Text);
		WarnText += "| " + std::to_string(WarnNum) + ". " + Text + "\n";
		WarnNum++;//警告数量加一
	}
	else if (Type == "R") {//显示警告
		bool Bool = false;
		while (1) {
			std::string str;
			str += "发生警告\n";
			str += "发现警告数量: " + std::to_string(WarnNum) + "\n\n";
			if (Bool)str += "警告信息:\n" + WarnText + "\n";
			str += "请联系创作者修复\n\n";
			str += "警告不影响游戏运行\n";
			str += "\n\n";
			str += "空格.查看详细\n";
			str += "r.返回\n";
			cls();
			ct(0, 0, str);
			char input = _getch();
			if (input == 'r' || input == 'R')break;
			else if (input == ' ')Bool = Bool ? false : true;
		}
	}
	else if (Type == "sss") {//清空警告
		Debug("发现警告数:  " + std::to_string(WarnNum));
		DebugWarn("目前发现警告数:  " + std::to_string(WarnNum));
		DebugWarn("======================================================");
		WarnText = "";
		WarnNum = 0;
	}
	cls();//清屏
}
//==========
//==========
//==========
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
		Error("无法解析JSON文件 " + WJName + " 错误信息: JSON文件格式错误 " + "错误位置: " + std::to_string(er.byte), "W");
		return false;//输出 假
	}
	return false;
}

bool MousePlace(int MX, int MY, int X, int Y, int DX, int DY) {
	if (MX >= X && MX <= DX) {
		if (MY >= Y && MY <= DY) return true;
		else return false;
	}
	else return false;
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
	//json
	std::string mapway = wstring_string(DataParent) + "/" + mapname;//计算地图路径 
	Debug("文件路径: " + mapway);
	nlohmann::json js;

	if (!OpenJson(mapname, mapway, js)) {//读取地图文件
		Error("", "R");
		return false;
	}
	if (js.contains("name"))Map.name = to_Ansi(js["name"]); else Map.name = NULLTEXT;//地图名称
	if (js.contains("author"))Map.Author = to_Ansi(js["author"]); else Map.Author = NULLTEXT;//作者名称
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
					if (MapData[i].size() > Map.maxx) Warn("地图文件: " + mapway + " 地图数据与尺寸不符 第" + std::to_string(i) + "行 SizeX " + std::to_string(Map.maxx) + "   实际有 " + std::to_string(MapData[i].size()), "W");//警告 数据过多X
				}
				else Error("地图文件: " + mapway + " 地图数据部分缺失 第" + std::to_string(i) + "行 SizeX " + std::to_string(Map.maxx) + "   实际只有 " + std::to_string(MapData[i].size()), "W");//错误 数据缺失X
			}
			if (MapData.size() > Map.maxy) Warn("地图文件: " + mapway + " 地图数据与尺寸不符 SizeY " + std::to_string(Map.maxy) + "   实际有 " + std::to_string(MapData.size()), "W");//警告 数据过多Y
		}
		else Error("地图文件: " + mapway + " 地图数据部分Y缺失 SizeY " + std::to_string(Map.maxy) + "   实际只有 " + std::to_string(MapData.size()), "W");//错误 数据缺失Y
	}
	else Error("地图文件: " + mapway + " 缺失数据 MapData", "W");//错误 数据缺失 MapData

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
					if (!js["SJData"][S1].contains("next")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - next", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].MapName = to_Ansi(js["SJData"][S1]["next"]);//传送的地图名字
					if (!js["SJData"][S1].contains("nextX")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextX", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].X = js["SJData"][S1]["nextX"];//传送的坐标 X
					if (!js["SJData"][S1].contains("nextY")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextY", "W");
					else Map.SJData.CSData[Map.SJData.CSNum].Y = js["SJData"][S1]["nextY"];//传送的坐标 X
					Map.SJData.Data[i].Hand = Map.SJData.CSNum;//获取传送地址
					Map.SJData.CSNum++;//传送事件数量加一
				}
				else if (Map.SJData.Data[i].Type == 2) {//对话事件
					if (!js["SJData"][S1].contains("object")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - object", "W");
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
					Map.SJData.Data[i].Hand = Map.SJData.DHNum;//获取对话地址
					Map.SJData.DHNum++;//对话事件数量加一
				}
			}
		}
	}
	cls();
	if (ErrorNum != 0) {//错误显示
		Error("", "R");
		return false;
	}
	if (WarnNum != 0) {
		if (BoolWarn) {//警告显示
			Warn("", "R");
		}
		Warn("", "sss");
	}
	Debug("=====读取地图");
	return true;
}

void GameData(std::string xz, int num){//<-------------------------------------------保存/读取游戏数据 
	if(xz == "GDW"){//GameData ====================
		Debug("保存游戏数据");
		nlohmann::json js;
		js["x"] = Player.myx;//位置
		js["y"] = Player.myy;
		js["lastx"] = Player.LastX;
		js["lasty"] = Player.LastY;
		js["nextx"] = Player.NextX;
		js["nexty"] = Player.NextY;
		js["life"] = Player.Life;//生命
		js["maxlife"] = Player.MaxLife;//最大生命
		js["mapname"] = to_utf8(Player.MapName);//当前地图名字
		js["gameruntime"] = GameRunTime;//游戏时刻
		std::ofstream outjs(GameDataWay);//打开文件
		outjs << js.dump(4);//写入文件
		outjs.close();//关闭文件
	}else if(xz == "GDR"){//GameData ====================
		Debug("读取游戏数据");
		std::ifstream jsin(GameDataWay);//用PFW表示Debug.txt文件 
		if (jsin.is_open()) {//如果文件打开成功
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
		else {//如果文件打开失败
			Debug("文件打开失败: " + GameDataWay);
			czdata("GD");//重置数据
			GameData("GDW", 1);//保存数据
		}
	}else if(xz == "SDW"){//SetData ====================
		Debug("保存设置数据");
		nlohmann::json js;
		js["BoolDebug"] = BoolDebug;
		js["BoolZbxs"] = BoolZbxs;
		js["BoolGameRunTime"] = BoolGameRunTime;
		js["BoolFPS"] = BoolFPS;
		js["BoolMapMessage"] = BoolMapMessage;
		js["BoolWarn"] = BoolWarn;
		js["FPSWeek"] = FPSWeek;
		std::ofstream outjs(SetDataWay);//打开文件
		outjs << js.dump(4);//写入文件
		outjs.close();//关闭文件
	}else if(xz == "SDR"){//SetData ====================
		Debug("读取设置数据");
		std::ifstream jsin(SetDataWay);//用PFW表示Debug.txt文件 
		if (jsin.is_open()) {//如果文件打开成功
			nlohmann::json js;
			jsin >> js;
			if (js.contains("BoolDebug"))       BoolDebug = js["BoolDebug"];
			if (js.contains("BoolZbxs"))        BoolZbxs = js["BoolZbxs"];
			if (js.contains("BoolGameRunTime")) BoolGameRunTime = js["BoolGameRunTime"];
			if (js.contains("BoolFPS"))         BoolFPS = js["BoolFPS"];
			if (js.contains("BoolMapMessage"))  BoolMapMessage = js["BoolMapMessage"];
			if (js.contains("BoolWarn"))        BoolWarn = js["BoolWarn"];
			if (js.contains("FPSWeek"))         FPSWeek = js["FPSWeek"];
			jsin.close();//关闭文件
		}
		else {//如果文件打开失败
			Debug("文件打开失败: " + SetDataWay);
			GameData("SDW", 1);//保存数据
		}
	}
}


bool gamestart(){//<----------------------------------------------------------游戏初始化
	GameData("GDR",-1);//读取游戏数据
	Player.PmX = (float)Player.myx;//玩家坐标
	Player.PmY = (float)Player.myy;
	const int Size = 1000;
	loadimage(&ImgWell, _T("Data/墙.jpg"), Size, Size);//加载图片
	loadimage(&ImgPlayer, _T("Data/玩家.jpg"), Size, Size);//加载图片
	loadimage(&ImgWell, _T("Data/墙.jpg"), Win.Size, Win.Size);//加载图片
	loadimage(&ImgPlayer, _T("Data/玩家.jpg"), Win.Size, Win.Size);//加载图片
	return GameMapData(Player.MapName);//加载对应地图 返回false说明地图打开失败
}


void pmsx(){//<---------------------------------------------------------------屏幕刷新 
//	cls();
	//std::string str;
	//str += "主角信息:\n";
	//
	////=====血条显示 
	//int num1 = Player.Life / 10;
	//int num2 = Player.Life % 10;
	//str += "生命值:";
	//str += std::to_string(Player.Life)+" ";
	//for(int i=1;i<=num1;i++) str+="##";
	//if(num2>=5) str+="# ";
	//else str+="  ";
	//for(int i=1;i<=10-num1;i++) str+="  ";
	//str += "\n";
	//
	//if(BoolZbxs){//=====坐标显示 
	//	str += "位置:" + std::to_string(Player.myx) + " " + std::to_string(Player.myy) + "   ";
	//	str += "Last:" + std::to_string(Player.LastX) + " " + std::to_string(Player.LastY) + "   ";
	//	str += "Next:" + std::to_string(Player.NextX) + " " + std::to_string(Player.NextY) + "          \n";
	//}
	//
	//if(BoolGameRunTime){//游戏时刻显示 
	//	str +="游戏时刻:"+std::to_string(GameRunTime)+"\n";
	//}
	//
	
	//
	//if(BoolMapMessage){//=====地图文件信息显示 
	//	str+="文件名:"+Player.MapName+" 大小XY:"+std::to_string(Map.maxx)+"*"+std::to_string(Map.maxy)+" 事件:"+std::to_string(Map.SJData.SJNum);
	//	str+="\n";
	//}
	//
	//
	//for(int i=0;i<Map.maxy;i++){//地图显示 
	//	for(int j=0;j<Map.maxx;j++){
	//		int num = Map.Data[i][j];
	//		
	//		if(j == Player.myx && i == Player.myy) str+="MY";
	//		else if(num ==  0) str+="  ";
	//		else if(num ==  1) str+="##"; 
	//		else if(num ==  2) str+="门"; 
	//		else if(num ==  9) str+="%%"; 
	//		else if(num == 10) str+="敌"; 
	//		else if(num == 11) str+="人"; 	
	//	}
	//	str+="  ";
	//	str+="\n"; 
	//}
	//
	//for(int i=0;i<=Map.maxx;i++) str+="  ";
	//str+="\n";
	//
	//str+=Map.name;//地图名称 
	//str+="\n";
	//
	//ct(0, 0, str);
	//ct(0, 0, "");//定位光标至左上角

	const int Hight = 32;//高度

	if (Win.LastSize != Win.Size) {//当缩放大小改变时
		Win.LastSize = Win.Size;//记录缩放大小
		loadimage(&ImgWell, _T("Data/墙.jpg"), Win.Size, Win.Size);//加载图片
		loadimage(&ImgPlayer, _T("Data/玩家.jpg"), Win.Size, Win.Size);//加载图片
	}

	BeginBatchDraw();//将绘图保存在缓存中

	int BY = Player.CameraY - (Win.WinY / 2 / Win.Size + 2) >= 0       ? (int)(Player.CameraY - (Win.WinY / 2 / Win.Size + 2)) : 0;//BeginY
	int EY = Player.CameraY + (Win.WinY / 2 / Win.Size + 2) < Map.maxy ? (int)(Player.CameraY + (Win.WinY / 2 / Win.Size + 2)) : Map.maxy;//EndY
	int BX = Player.CameraX - (Win.WinX / 2 / Win.Size + 2) >= 0       ? (int)(Player.CameraX - (Win.WinX / 2 / Win.Size + 2)) : 0;//BeginX
	int EX = Player.CameraX + (Win.WinX / 2 / Win.Size + 2) < Map.maxx ? (int)(Player.CameraX + (Win.WinX / 2 / Win.Size + 2)) : Map.maxx;//EndX
	int GY = (int)(Win.WinY / 2 - Player.CameraY * Win.Size);
	int GX = (int)(Win.WinX / 2 - Player.CameraX * Win.Size);

	for (int i = BY; i < EY; i++) {//地图显示 
		for (int j = BX; j < EX; j++) {
			int num = Map.Data[i][j];
			if (num == 1)putimage(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Win.Size, &ImgWell, 0, 0);
			//if (j == Player.myx && i == Player.myy) str += "MY";
			//else if (num == 0) str += "  ";
			//else if (num == 1) str += "##";
			//else if (num == 2) str += "门";
			//else if (num == 9) str += "%%";
			//else if (num == 10) str += "敌";
			//else if (num == 11) str += "人";

		}
	}
	putimage((int)(Player.PmX * Win.Size + GX), (int)(Player.PmY * Win.Size + GY), Win.Size, Win.Size, &ImgPlayer, 0, 0);//绘制玩家

	if(BoolFPS){//=====FPS显示 
		if(!(GameRunTime % FPSWeek)){
			FPS = 1000 / Time.JGTime;
		}
		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		settextcolor(RGB(255, 255, 255));//设置文字颜色
		outtextxy(0, 0, (L"FPS:" + std::to_wstring(FPS) + L" " + std::to_wstring(Time.JGTime) + L"ms ").c_str());
	}

	settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
	settextcolor(RGB(255, 255, 255));//设置文字颜色
	outtextxy(0, 15, (L"" + std::to_wstring(Player.PmX) + L" " + std::to_wstring(Player.PmY)).c_str());

	FlushBatchDraw();//将绘图从缓存中绘制到屏幕
	cleardevice();// 清屏
}

bool IfMove(int x, int y) {//<-----------------------------------------------------------------------是否可以移动 
	int num = Map.Data[y][x];//获取地图数据
	if(num == 0 || num == 2 || num == 9){//可以通过
		return true;
	}
	return false;
}

void PlayerMove(int xx, int yy, float Speed) {//<-----------------------------------------------------------------------玩家移动
	const float ENum = 0.99999f;//误差值 用于修正坐标
	//=========== X ==========
	if (xx != 0) {//当玩家 X轴 移动时
		float NowX = Player.PmX;//当前坐标 x
		float ToX = Player.PmX + xx * Speed;//目标坐标 x
		float NowY = Player.PmY;//当前坐标 y
		if (Speed >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		Player.PmX = ToX;//移动
		if (xx < 0) {//左移动
			if (!IfMove((int)ToX, (int)NowY) || !IfMove((int)ToX, (int)(NowY + ENum))) Player.PmX = (float)(int)(ToX + 1);//修正坐标
		}
		else {//右移动
			if (!IfMove((int)(ToX + 1), (int)NowY) || !IfMove((int)(ToX + 1), (int)(NowY + ENum))) Player.PmX = (float)(int)ToX;//修正坐标
		}
		Player.LastX = Player.NextX;//经过坐标
		Player.myx = (int)Player.PmX;//当前坐标
		Player.CameraX = Player.PmX;//摄像机坐标
		//if (xx > 0) Player.NextX = (int)Player.PmX + xx;//计算玩家的行动坐标
		//else Player.NextX = (int)Player.PmX;
		//settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		//settextcolor(RGB(255, 255, 255));//设置文字颜色
		//outtextxy(0, 30, (L"" + std::to_wstring((int)ToX) + L" " + std::to_wstring((int)NowY) + L" " + std::to_wstring((int)ToX) + L" " + std::to_wstring((int)(NowY + 0.99))).c_str());
	}
	//=========== Y ==========
	if (yy != 0) {//当玩家 X轴 移动时
		float NowY = Player.PmY;//当前坐标 x
		float ToY = Player.PmY + yy * Speed;//目标坐标 x
		float NowX = Player.PmX;//当前坐标 y
		if (Speed >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		Player.PmY = ToY;//移动
		if (yy < 0) {//上移动
			if (!IfMove((int)NowX, (int)ToY) || !IfMove((int)(NowX + ENum), (int)ToY)) Player.PmY = (float)(int)(ToY + 1);//修正坐标
		}
		else {//下移动
			if (!IfMove((int)NowX, (int)(ToY + 1)) || !IfMove((int)(NowX + ENum), (int)(ToY + 1))) Player.PmY = (float)(int)ToY;//修正坐标
		}
		Player.LastY = Player.NextY;//经过坐标
		Player.myy = (int)Player.PmY;//当前坐标
		Player.CameraY = Player.PmY;//摄像机坐标
		//if (yy > 0) Player.NextY = (int)Player.PmY + yy;
		//else Player.NextY = (int)Player.PmY;
		//settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		//settextcolor(RGB(255, 255, 255));//设置文字颜色
		//outtextxy(0, 30, (L"" + std::to_wstring((int)ToX) + L" " + std::to_wstring((int)NowY) + L" " + std::to_wstring((int)ToX) + L" " + std::to_wstring((int)(NowY + 0.99))).c_str());
	}
	//int num = Map.Data[Player.NextY][Player.NextX];//获取玩家行动坐标的地图数据
	//if(num == 0 || num == 2 || num == 9){//当玩家行动坐标是否可以通过
	//	Player.PmX += xx * Speed;//移动
	//	Player.PmY += yy * Speed;
	//	Player.LastX = Player.NextX;//经过坐标
	//	Player.LastY = Player.NextY;
	//	Player.myx = (int)Player.PmX;//当前坐标
	//	Player.myy = (int)Player.PmY;

	//	Player.CameraX = Player.PmX;//摄像机坐标
	//	Player.CameraY = Player.PmY;
	//}
}

void playerinput(){//<---------------------------------------------------玩家输入 
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {//返回
		GameData("GDW",1);//保存游戏数据
		YM = "begin";//返回开始菜单
	}
	int xx=0,yy=0;//定义坐标变化量
	
	if (GetAsyncKeyState(PlayerInput.left)  & 0x8000)xx--;//左
	if (GetAsyncKeyState(PlayerInput.right) & 0x8000)xx++;//右
	if (GetAsyncKeyState(PlayerInput.up)    & 0x8000)yy--;//上
	if (GetAsyncKeyState(PlayerInput.down)  & 0x8000)yy++;//下
	if (xx != 0 || yy != 0) PlayerMove(xx, yy, Player.Speed * Time.JGTime / 1000);//玩家移动

	if (peekmessage(&Mouse, EX_MOUSE)) {//获取鼠标消息
		if (Mouse.message == WM_MOUSEWHEEL) {//鼠标滚轮
			if (Mouse.wheel > 0) {
				Win.Size += (int)(Win.Size * 0.2);
			}
			else {
				Win.Size -= (int)(Win.Size * 0.2);
			}
			if (Win.Size > Win.WinY / 3) Win.Size = Win.WinY / 3;
			if (Win.Size < Win.WinY / 120) Win.Size = Win.WinY / 120;
		}
	}
}


void shpd(){//<---------------------------------------------------------------------伤害判定 
	if(Map.Data[Player.myy][Player.myx] == 9){//陷阱判定
		Player.Life -= 3;//造成伤害
	}
//	Debug(to_string(Player.myx)+" "+to_string(Player.LastX)+" "+to_string(Player.myy)+" "+to_string(Player.LastY),-1);
	if(Player.myx != Player.LastX || Player.myy != Player.LastY){//敌人判定 
		int drsl = 0;//敌人数量 
		int numX,numY;
		numX = Player.myx;
		numY = Player.myy;
		for(int i=-1;i<=1;i++){
			for(int j=-1;j<=1;j++){
				if(Map.Data[numY+i][numX+j] == 10){//当玩家周围有敌人时
					drsl++; //敌人数量加一
				}
			}
		}
//		Debug("敌人判定 ",drsl);
		Player.Life-=drsl;//造成伤害
	}
	if(Player.Life<0)Player.Life = 0;//防止生命小与零
}


void swpd(){//<---------------------------------------------------------------------死亡判定 
	if(Player.Life<=0){//当玩家生命小于等于零时
		//cz = -1;//切换到死亡界面------------------------------------------------------------------------------------------------------
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
			GameData("GDW", 1);//保存游戏
		}
	}
}


void Lifeup(){//<-------------------------------------------------------------------生命自然回复 
	if(GameRunTime %10 == 0 && Player.Life < Player.MaxLife){//条件判断 
		Player.Life++;//回复生命 
	}
	else if (Player.Life > Player.MaxLife) {//防止生命大于最大生命 
		Player.Life = Player.MaxLife; 
	} 
}


void SaveData(int Week){//<-----------------------------------------------------------游戏自动保存 
	if(GameRunTime %Week == 0){//条件判定 
		GameData("GDW",1);//保存游戏 
	}
}

void TimeMath() {//<------------------------------------------------------------------时间计算
	Time.LastTime = Time.NowTime;//赋值上次时间
	Time.NowTime = clock();//获取当前时间
	Time.JGTime = Time.NowTime - Time.LastTime;//计算间隔时间
}

void gameon(){//=======================================================================游戏主控 
	TimeMath();//时间计算
	pmsx();//屏幕刷新 
	shpd();//伤害判定 
	swpd();//死亡判定
	Lifeup();//生命自然回复 
	playerinput();//玩家输入 
	MapSJ();//地图特殊事件 
	SaveData(10000);//自动保存 10s
	GameRunTime++;//每循环一次游戏时刻加1
	Sleep(1);//游戏延迟
}

void YMBegin(){//<--------------------------------------------------------------------初始界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int ButtonX = Win.WinX / 8;
	int ButtonY = Win.WinY / 5;
	int ButtonSpaceY = Win.WinY / 10;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = ButtonX;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);

	bool BoolButtonEnd = false;
	bool BoolButton1 = false;
	bool BoolButton2 = false;
	bool BoolButton3 = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);

		peekmessage(&Mouse, EX_MOUSE);

		//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("冒险之路"));//输出文字
		// ===== Button
		//设置字体 =-= (开始游戏)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("开始游戏"));
		//设置字体 =-= (设置)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("设置"));
		//设置字体 =-= (关于)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("关于"));
		//设置字体 =-= (退出)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("退出"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 开始游戏
				if (gamestart()) {
					YM = "gameon";//切换到游戏界面
					return;
				}
				else {
					MessageBox(GetHWnd(), _T("地图文件打开失败！"), _T("错误"), MB_OK | MB_ICONERROR);
					Mouse.x = -1;
					Mouse.y = -1;
					//YM = "begin";//保持在初始界面
					return;
				}
			}
			if (BoolButton2) {//按钮2 设置
				YM = "set";//切换到设置界面
				return;
			}
			if (BoolButton3) {//按钮3 关于
				YM = "about";//切换到关于界面
				return;
			}
			if (BoolButtonEnd) {//按钮4 退出
				BoolTheGame = false;//结束游戏主循环
				return;
			}
		}

		Sleep(1); //延时
		cleardevice();// 清屏
	}
}

void YMSet(){//<--------------------------------------------------------------------设置界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int ButtonX = Win.WinX / 8;
	int ButtonY = Win.WinY / 5;
	int ButtonSpaceY = Win.WinY / 10;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = ButtonX;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);

	bool BoolButtonEnd = false;
	bool BoolButton1 = false;
	bool BoolButton2 = false;
	bool BoolButton3 = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);

		peekmessage(&Mouse, EX_MOUSE);

		//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("设置"));//输出文字
		// ===== Button
		//设置字体 =-= (开始游戏)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("设置"));
		//设置字体 =-= (设置)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("设置"));
		//设置字体 =-= (关于)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("设置"));
		//设置字体 =-= (返回)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 

			}
			if (BoolButton2) {//按钮2 
				YM = "set";//切换到设置界面
				return;
			}
			if (BoolButton3) {//按钮3
				YM = "about";//切换到关于界面
				return;
			}

			if (BoolButtonEnd) {//按钮 返回
				YM = "begin";//切换到初始界面
				return;
			}
		}
		Sleep(1); //延时
		cleardevice();// 清屏
	}
}

void YMAbout() {//<------------------------------------------------------------------关于界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int TextHigh = Win.WinY / 30 + 5;// ===== Text
	int TextX = Win.WinX / 8;
	int TextY = Win.WinY / 5;
	int TextSpaceY = TextHigh + Win.WinY / 50;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = TextX;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);
	COLORREF TextColor1 = RGB(255, 255, 255);

	bool BoolButtonEnd = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);
		peekmessage(&Mouse, EX_MOUSE);//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("关于"));//输出文字
		// ===== Button
		//文本 1
		Object.TextStyle1(TextColor1, TextHigh);
		outtextxy(TextX, TextY + TextSpaceY * 0, _T("本项目由 \"平凡0_0\" 创作"));
		//文本 2
		Object.TextStyle1(TextColor1, TextHigh);
		outtextxy(TextX, TextY + TextSpaceY * 1, _T("想不到还有什么可以写了"));

		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButtonEnd) {//按钮1 返回
				YM = "begin";//切换到初始界面
				return;
			}
		}

		Sleep(1); //延时
		cleardevice();// 清屏
	}
}

void ymxs(){//<----------------------------------------------------------------------页面显示
	if (YM == "begin")  YMBegin();//初始界面
	if (YM == "gameon") gameon();//游戏页面
	if (YM == "set")    YMSet();//设置页面
	if (YM == "about")  YMAbout();//关于页面
	//if(cz == 0){//起始页面====================(0) 
 //   	while(1) {//input
	//		std::string str;
	//		str+= "冒险之路\n1.继续游戏\n2.设置\n3.关于\n\nEsc.退出游戏";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//        char input=_getch();//获取玩家输入
	//		if(input == '1') {//选项 继续游戏-----(1)
	//			if (gamestart())cz = 1;//如果初始化成功 切换到游戏界面
	//            break;
	//        }else if(input == '2') {//选项 设置-----(2)
	//			cz=2;//切换到设置界面
	//			GameData("SDR",-1);//读取设置数据
	//            break;
	//        }else if(input == '3') {//选项 关于-----(3)
	//			cz=3;//切换到关于界面
	//            break;
	//        }else if(input == PlayerInput.returnn) {//选项 退出游戏-----(r)
	//        	BoolTheGame = false;//结束游戏主循环
	//            return;
	//        }
	//	}
	//}else if(cz == 2){//设置页面====================(2) 
	//	while(1){
	//		std::string str;
	//		str += "设置\n";
	//		str += "1.日志记录          当前:";if(BoolDebug)       str+="开\n";else str+="关\n";
	//		str += "2.FPS显示           当前:";if(BoolFPS)         str+="开\n";else str+="关\n";
	//		str += "3.FPS刷新间隔       当前:";                    str+="每"+std::to_string(FPSWeek)+"游戏时刻更新\n";
	//		str += "4.游戏时刻显示      当前:";if(BoolGameRunTime) str+="开\n";else str+="关\n";
	//		str += "5.坐标显示          当前:";if(BoolZbxs)        str+="开\n";else str+="关\n";
	//		str += "6.地图信息显示      当前:";if(BoolMapMessage)  str+="开\n";else str+="关\n";
	//		str += "7.警告显示          当前:";if(BoolWarn)        str+="开\n";else str+="关\n";
	//		str += "8.重置游戏数据      当前:无\n";
	//		str+="\n\n\n";
	//		str+="Esc.返回";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//        char input=_getch();//获取玩家输入
	//        if(input == '1'){//选项 日志记录-----(1) 
	//			BoolDebug = BoolDebug ? false : true;//切换状态
	//		}else if(input == '2'){//选项 FPS显示-----(2)
	//        	BoolFPS = BoolFPS ? false : true;//切换状态
	//		}else if(input == '3'){//选项 FPS显示-----(3)
	//        	while(1){
	//        		str+="FPS刷新间隔\n"; 
	//        		str+="1.每1时刻更新\n";
	//        		str+="2.每2时刻更新\n";
	//        		str+="3.每5时刻更新\n";
	//        		str+="4.每10时刻更新\n";
	//        		str+="5.每20时刻更新\n";
	//        		str+="6.自定义\n";
	//        		str+="\n";
	//        		str+="当前:每"+std::to_string(FPSWeek)+"游戏时刻更新";
	//        		str+="\n\n\n";
	//				str+="Esc.返回";
	//				cls();
	//        		ct(0,0,str);
	//        		char input1=_getch();
	//        		if(input1 == '1'){//选项  每1-(1) 
	//        			FPSWeek = 1;
	//				}else if(input1 == '2'){//选项  每2-(2) 
	//        			FPSWeek = 2;
	//				}else if(input1 == '3'){//选项  每5-(3) 
	//        			FPSWeek = 5;
	//				}else if(input1 == '4'){//选项  每10-(4) 
	//        			FPSWeek = 10;
	//				}else if(input1 == '5'){//选项  每20-(5) 
	//        			FPSWeek = 20;
	//				}else if(input1 == '6'){//选项  自定义-(6) 
	//        			while(1){
	//        				str+="FPS刷新间隔 自定义\n";
	//        				str+="请输入 数值\n";
	//        				str+="范围 数值 >= 1\n";
	//        				cls();
	//        				ct(0,0,str);
	//        				int num;
	//        				std::cin>>num;
	//        				if(num>=1){
	//        					FPSWeek = num;
	//							break; 
	//						}else{
	//							str+="\n范围错误 按 Esc 键以重新输入\n";
	//							ct(0,0,str);
	//							while(1){
	//								char input2 = _getch();
	//								if(input2 == PlayerInput.returnn){
	//									break;
	//								}
	//							}
	//						}
	//					}
	//				}else if(input1 == PlayerInput.returnn){
	//					GameData("SDW",-1);
	//		            break;
	//				}
	//			} 
	//		}
	//		else if(input == '4'){//选项 游戏时刻显示-----(4)
	//        	BoolGameRunTime = BoolGameRunTime ? false : true;//切换状态
	//		}
	//		else if(input == '5'){//选项 坐标显示-----(5)
	//        	BoolZbxs = BoolZbxs ? false : true;//切换状态
	//		}
	//		else if(input == '6'){//选项 地图信息显示-----(6)
	//        	BoolMapMessage = BoolMapMessage ? false : true;//切换状态
	//		}
	//		else if (input == '7') {//选项 警告显示-----(7)
	//			BoolWarn = BoolWarn ? false : true;//切换状态
	//		}
	//		else if (input == PlayerInput.returnn) {//选项 返回-----(r) 
	//			GameData("SDW",-1);//保存设置数据
	//        	cz=0;//切换到初始界面
	//            break;//退出循环
	//        }
	//	}
	//}else if(cz == 3){//关于====================(3) 
	//	while(1){
	//		std::string str;
	//		str+="关于\n";
	//		str+="\n";
	//		str+="Null\n";
	//        str+="\n\n\n";
	//		str+="Esc.返回";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//		char input=_getch();//获取玩家输入
	//        if(input == PlayerInput.returnn) {//选项 返回-----(r)
	//        	cz=0;//切换到初始界面
	//            break;//退出循环
	//        }
	//	}	
	//}else if(cz == -1){//死亡页面====================(-1) 
	//	pmsx();//屏幕刷新
	//	GameData("GDW",0);//保存游戏数据
	//	int X=5,Y=6;//定义文本显示位置
	//	std::string str;
	//	str=" ________________________ ";
	//	ct(X,Y+0,str);
	//	str="|                        |";
	//	ct(X,Y+1,str);
	//	str="|        GAME_OVER       |";
	//	ct(X,Y+2,str);
	//	str="|                        |";
	//	ct(X,Y+3,str);
	//	str="|                        |";
	//	ct(X,Y+4,str);
	//	str="|                        |";
	//	ct(X,Y+5,str);
	//	str="|                        |";
	//	ct(X,Y+6,str);
	//	str="|    ---按Esc键返回---   |";
	//	ct(X,Y+7,str);
	//	str="|________________________|";
	//	ct(X,Y+8,str);
	//	while(1){
	//		char input=_getch();//获取玩家输入
	//		if(input == PlayerInput.returnn){
	//			cz=0;//切换到起始界面
	//			break;//退出循环
	//		}
	//	}
	//}
}

void Begin(){//----------------------------------------------------------------------初始化
	Debug("sss");//清空日志
	DebugError("sss");//清空错误日志
	DebugWarn("sss");//清空警告日志
	
	GameData("SDR",-1);//读取设置数据
	Debug("初始化");
	CDW(DataParent);//创建文件夹 Data
	CDW(DataWayParent);//创建文件夹 GameData 
	CDW(SetWayParent);//创建文件夹 Set
	for(int i=1;i<=555;i++)ClsText+="                       ";//补充清屏
	initgraph(Win.WinX, Win.WinY);//初始化图形界面
	BoolTheGame = true;//游戏主循环开关
	YM = "begin";//设置页面为初始页面
	//IMAGE test;
	//loadimage(&test, _T("Data/墙.jpg")); // 例如 500x500
	//putimage(0, 0, 30, 300, &test, 55, 0); // 从(0,0)取到图片右下角，缩放到100x100绘制

	//Sleep(10000); //延时

}

void End(){//----------------------------------------------------------------------结束化
	closegraph();//关闭图形界面
}

int main() {//----------------------------------------------------------------------main 
	
	Begin();//初始化

	Debug("游戏开始==================");

    while(BoolTheGame) {//main
		ymxs();//页面显示
    }
	

	Debug("=============游戏正常退出");

    return 0;//结束进程
}