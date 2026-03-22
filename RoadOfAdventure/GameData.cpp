#include "GameData.h"

#include<Windows.h>
#include"json.hpp"
#include"Debug.h"
#include"TypeConversion.h"
#include<fstream>

const std::wstring DataWayParent = L"GameData";//----文件夹路径 游戏数据
const std::wstring SetWayParent = L"Debug";//-------文件夹路径 Debug
const std::wstring DataParent = L"Data";//--------文件夹路径 数据 

const std::wstring MapParent = L"Map";//---------文件夹路径 地图
const std::wstring ImageParent = L"Image";//-------文件夹路径 图片
const std::wstring InformationParent = L"Information";//-------文件夹路径 图片

const std::string GameDataWay = "GameData/GameData.json";//-----文件名称 游戏数据
const std::string SetDataWay = "GameData/SetData.json";//------文件名称 设置数据
const std::string DebugWay = "Debug/Debug.txt";//------------文件名称 Debug
const std::string ErrorWay = "Debug/Error.txt";//------------文件名称 Error
const std::string WarnWay = "Debug/Warn.txt";//-------------文件名称 Warn

const std::string BeginMap = "冒险之路.json";//---------------初始的地图 

const std::string NULLTEXT = "无/Null";




bool BoolTheGame = true;//----------游戏是否运行  
bool BoolDebug = true;//------------日志Debug是否开启 
bool BoolZbxs = true;//-------------坐标显示是否开启  
bool BoolGameRunTime = true;//------游戏时刻显示是否开启  
bool BoolFPS = true;//--------------FPS显示是否开启 
bool BoolVxs = true;//--------------速度显示是否开启
bool BoolMapMessage = true;//-------地图信息显示是否开启  
bool BoolWarn = false;//------------是否显示警告状态 

int FPS;//-------------------FPS数值 
int FPSWeek = 10;//----------FPS循环周期 
long long GameRunTime;//-----游戏时刻 

StructPlayer Player;//玩家信息

SM Map = {//StructMap 的缩写 地图信息 
	0,0,//-------地图大小 
	"",//---文件名字 
	"",//-----地图名字 
	NULLTEXT,//地图作者
	{{}}//地图数据
};

StructWin Win = {//窗口信息 
	800,//窗口宽度
	600,//窗口高度
	20,//每个格子的像素大小
	0//上次每个格子的像素大小
};

StructCentralData CentralData;//中心数据

//std::vector <StructImage> ImageData;//图片信息

StructTime Time;//时间信息

StructInput PlayerInput = {//输入信息 
	'W',//上
	'S',//下
	'A',//左
	'D',//右
	27  //返回
};

ExMessage Mouse;//定义鼠标数据
int MouseX;//鼠标坐标
int MouseY;

std::string YM;//当前页面

std::string MapName[201];//地图列表
int MapNum;//地图列表 的地图数量

std::string ErrorText;//错误信息
int ErrorNum;//错误数量
std::string WarnText;//警告信息
int WarnNum;//警告数量


void CDW(const std::wstring& str) {//<-------------------------------------------------------创建文件夹 
	CreateDirectoryW(str.c_str(), NULL);//创建文件夹 
}

bool FFFW(const std::wstring& str, const std::string FindMapName) {//<-------------------------------------------------------获取文件 
	bool Find = true;
	WIN32_FIND_DATAW num;//创建结构体以暂时保存地图名称 
	HANDLE Name = FindFirstFileW((str + L"\\*").c_str(), &num);// 来自AI 获取名称
	int i = 0;//定义记号 
	while (FindNextFileW(Name, &num)) {//是否还有文件 
		std::string strr = wstring_string(num.cFileName);//获取名称
		MapName[i] = strr;//将地图名称写入数组
		if (Find && FindMapName == strr) Find = false;
		Debug("第" + std::to_string(i) + "个地图文件:" + strr);
		i++;//记号加一 
	}
	MapNum = i - 1;//记录地图数量 
	FindClose(Name);//结束寻找 
	return !Find;
}

std::string FindFile(const std::wstring& str, const std::string FindName) {//<-------------------------------------------------------查找文件
	std::string way = "null";
	// 遍历目录
	for (const auto& entry : std::filesystem::recursive_directory_iterator(str)) {
		if (entry.path().filename().string() == FindName) {
			way = entry.path().string();
			break;// 找到文件，返回路径
		}
	}
	return way;
}


std::vector <FileData> FindFileAll(const std::wstring& str) {//<-------------------------------------------------------查找所有文件
	std::vector<FileData> FData;
	// 遍历目录
	for (const auto& entry : std::filesystem::recursive_directory_iterator(str)) {
		if (entry.is_regular_file())
			FData.push_back({ entry.path().filename().string(), entry.path().string() });
	}
	return FData;//返回所有文件数据
}


std::vector <FileData> FindJsonAll(const std::wstring& str) {//<-------------------------------------------------------查找所有文件
	std::vector<FileData> FData;
	// 遍历目录
	for (const auto& entry : std::filesystem::recursive_directory_iterator(str)) {
		if (entry.is_regular_file() && entry.path().filename().string().ends_with(".json")) {
			FData.push_back({ entry.path().filename().string(), entry.path().string() });
		}
	}
	return FData;//返回所有文件数据
}

void czdata(std::string xz) {//<--------------------------------------------------------------重置数据 
	if (xz == "GD") {
		Debug("重置游戏数据====");
		//Player.myx = 1;//X 
		//Player.myy = 1;//Y 
		//Player.LastX = 1;//LastX 
		//Player.LastY = 1;//LastY 
		//Player.NextX = 1;//NextX 
		//Player.NextY = 1;//NextY 
		//Player.Life = 100;//血条 
		//Player.MaxLife = 100;//最大血量 

		GameRunTime = 0;//游戏时刻 

		Player.MapName = BeginMap;//初始地图 

		Debug("====重置游戏数据");
	}
	else if (xz == "SD") {

	}
}

void GameData(std::string xz, int num) {//<-------------------------------------------保存/读取游戏数据 
	if (xz == "GDW") {//GameData ====================
		Debug("保存游戏数据");
		nlohmann::json js;
		//js["x"] = Player.myx;//位置
		//js["y"] = Player.myy;
		//js["lastx"] = Player.LastX;
		//js["lasty"] = Player.LastY;
		//js["nextx"] = Player.NextX;
		//js["nexty"] = Player.NextY;
		//js["life"] = Player.Life;//生命
		//js["maxlife"] = Player.MaxLife;//最大生命
		js["mapname"] = to_utf8(Player.MapName);//当前地图名字
		js["gameruntime"] = GameRunTime;//游戏时刻
		std::ofstream outjs(GameDataWay);//打开文件
		outjs << js.dump(4);//写入文件
		outjs.close();//关闭文件
	}
	else if (xz == "GDR") {//GameData ====================
		Debug("读取游戏数据");
		std::ifstream jsin(GameDataWay);//用PFW表示Debug.txt文件 
		if (jsin.is_open()) {//如果文件打开成功
			nlohmann::json js;
			jsin >> js;
			//Player.myx = js["x"];//获取位置
			//Player.myy = js["y"];
			//Player.LastX = js["lastx"];
			//Player.LastY = js["lasty"];
			//Player.NextX = js["nextx"];
			//Player.NextY = js["nexty"];
			//Player.Life = js["life"];//获取生命
			//Player.MaxLife = js["maxlife"];
			Player.MapName = to_Ansi(js["mapname"]);//获取地图文件名
			GameRunTime = js["gameruntime"];//获取游戏时刻
			jsin.close();
		}
		else {//如果文件打开失败
			Debug("文件打开失败: " + GameDataWay);
			czdata("GD");//重置数据
			GameData("GDW", 1);//保存数据
		}
	}
	else if (xz == "SDW") {//SetData ====================
		Debug("保存设置数据");
		nlohmann::json js;
		js["BoolDebug"] = BoolDebug;
		js["BoolZbxs"] = BoolZbxs;
		js["BoolGameRunTime"] = BoolGameRunTime;
		js["BoolFPS"] = BoolFPS;
		js["BoolVxs"] = BoolVxs;
		js["BoolMapMessage"] = BoolMapMessage;
		js["BoolWarn"] = BoolWarn;
		js["FPSWeek"] = FPSWeek;
		std::ofstream outjs(SetDataWay);//打开文件
		outjs << js.dump(4);//写入文件
		outjs.close();//关闭文件
	}
	else if (xz == "SDR") {//SetData ====================
		Debug("读取设置数据");
		std::ifstream jsin(SetDataWay);//用PFW表示Debug.txt文件 
		if (jsin.is_open()) {//如果文件打开成功
			nlohmann::json js;
			jsin >> js;
			if (js.contains("BoolDebug"))       BoolDebug = js["BoolDebug"];
			if (js.contains("BoolZbxs"))        BoolZbxs = js["BoolZbxs"];
			if (js.contains("BoolGameRunTime")) BoolGameRunTime = js["BoolGameRunTime"];
			if (js.contains("BoolFPS"))         BoolFPS = js["BoolFPS"];
			if (js.contains("BoolVxs"))         BoolVxs = js["BoolVxs"];
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

int to_MapHand(int x, int y) {//<--------------------------------------------------------------坐标转换为地图数据序号
	return (y * Map.maxx + x);
}

bool OpenJson(const std::string Way, const std::string WJName, nlohmann::json& jsin) {//<--------------------------------------------------------------打开JSON 分析
	std::ifstream Open(Way);
	if (!Open.is_open()) {//如果文件打开失败
		std::string T = (Way == "null") ? "未找到此文件" : "未知";
		Error("无法打开文件: " + WJName + " 错误信息: " + T, "W");
		Open.close();
		return false;//输出 假
	}
	try {//如果没有错误
		Open >> jsin;
		Open.close();
		return true;//输出 真
	}
	catch (const nlohmann::json::parse_error& er) {//如果无法解析
		Error("无法解析JSON文件 " + WJName + " 错误信息: JSON文件格式错误 " + "错误位置: " + std::to_string(er.byte), "W");
		Open.close();
		return false;//输出 假
	}
	Open.close();
	return false;
}

int FindData(std::string name) {//---------------------------------------------------寻找中心数据
	static int Hand = 0;
	if (Hand > CentralData.Data.size())Hand = 0;
	if (Hand != 0) {
		if (CentralData.Data[Hand].name == name)return Hand;
		else Hand = 0;
	}

	for (; Hand < CentralData.Data.size(); Hand++) {
		if (CentralData.Data[Hand].name == name)return Hand;
	}
	return -1;
}

int FindUnitData(std::string name) {//---------------------------------------------------寻找Unit数据
	static int Hand = 0;
	if (Hand > CentralData.UnitData.size())Hand = 0;
	if (Hand != 0) {
		if (CentralData.UnitData[Hand].Name == name)return Hand;
		else Hand = 0;
	}

	for (; Hand < CentralData.UnitData.size(); Hand++) {
		if (CentralData.UnitData[Hand].Name == name)return Hand;
	}
	return -1;
}


bool GameMapData(std::string mapname) {//<---------------------------------------------读取地图文件 
	Debug("读取地图=====");
	Debug("文件名称:" + mapname);
	//json
	std::string mapway = wstring_string(DataParent + L"/" + MapParent) + "/" + mapname;//计算地图路径 
	Debug("文件路径: " + mapway);
	nlohmann::json js;

	if (!OpenJson(FindFile(DataParent + L"/" + MapParent, mapname), mapname, js)) {//读取地图文件
		Error("", "R");
		return false;
	}
	if (js.contains("name"))Map.name = to_Ansi(js["name"]); else Map.name = NULLTEXT;//地图名称
	if (js.contains("author"))Map.Author = to_Ansi(js["author"]); else Map.Author = NULLTEXT;//作者名称
	if (js.contains("SizeX"))Map.maxx = js["SizeX"]; else Error("地图文件: " + mapway + " 缺失数据 SizeX", "W");//地图尺寸
	if (js.contains("SizeY"))Map.maxy = js["SizeY"]; else Error("地图文件: " + mapway + " 缺失数据 SizeY", "W");//地图尺寸

	if (js.contains("dictionary")) {//地图数据 字典
		std::vector<std::string> dictionary(65536);;//字典
		auto& dict = js["dictionary"];
		for (auto it = dict.begin(); it != dict.end(); it++) {//读取字典
			if (!it.value().is_number_integer()) {//检测字典
				Error("地图文件: " + mapway + " dictionary 值不是整数: " + it.key(), "W");
				continue;
			}
			int num = it.value().get<int>();
			dictionary[num] = to_Ansi(it.key());
		}

		GameMapDataClear();
		if (js.contains("MapData")) {//
			auto& MapData = js["MapData"];
			if (MapData.size() >= Map.maxy) {//检测数据是否完整 Y
				for (int i = 0; i < Map.maxy; i++) {
					if (MapData[i].size() >= Map.maxx) {//检测数据是否完整 X
						for (int j = 0; j < Map.maxx; j++) {
							Map.Data.push_back({});
							Map.Data[to_MapHand(j, i)].floor = FindData(dictionary[MapData[i][j]]);
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
	}
	else Error("地图文件: " + mapway + " 缺失数据 dictionary", "W");//错误 数据缺失 dictionary


	//if (!js.contains("SJData"))Error("地图文件: " + mapway + " 缺失数据 SJData", "W");//键 SJData
	//if (js["SJData"].contains("sjnum"))Map.SJData.SJNum = js["SJData"]["sjnum"]; else Error("地图文件: " + mapway + " 缺失数据 SJData - sjnum", "W");//事件数量
	//for (int i = 0; i < Map.SJData.SJNum; i++) {
	//	std::string S1 = "SJ" + std::to_string(i);//---定义
	//	if (!js["SJData"].contains(S1)) {//键 SJData     SJi
	//		Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1, "W");
	//	}//不存在 ---上
	//	else {//存在 ---下
	//		if (!js["SJData"][S1].contains("x")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - x", "W");
	//		else Map.SJData.Data[i].X = js["SJData"][S1]["x"];//触发X
	//		if (!js["SJData"][S1].contains("y")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - y", "W");
	//		else Map.SJData.Data[i].Y = js["SJData"][S1]["y"];//触发Y
	//		if (!js["SJData"][S1].contains("type")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - type", "W");
	//		else {//事件的数据
	//			Map.SJData.Data[i].Type = js["SJData"][S1]["type"];//事件类型
	//			if (Map.SJData.Data[i].Type == 1) {//传送事件
	//				if (!js["SJData"][S1].contains("next")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - next", "W");
	//				else Map.SJData.CSData[Map.SJData.CSNum].MapName = to_Ansi(js["SJData"][S1]["next"]);//传送的地图名字
	//				if (!js["SJData"][S1].contains("nextX")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextX", "W");
	//				else Map.SJData.CSData[Map.SJData.CSNum].X = js["SJData"][S1]["nextX"];//传送的坐标 X
	//				if (!js["SJData"][S1].contains("nextY")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - nextY", "W");
	//				else Map.SJData.CSData[Map.SJData.CSNum].Y = js["SJData"][S1]["nextY"];//传送的坐标 X
	//				Map.SJData.Data[i].Hand = Map.SJData.CSNum;//获取传送地址
	//				Map.SJData.CSNum++;//传送事件数量加一
	//			}
	//			else if (Map.SJData.Data[i].Type == 2) {//对话事件
	//				if (!js["SJData"][S1].contains("object")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - object", "W");
	//				else Map.SJData.DHData[Map.SJData.DHNum].ObjectName = to_Ansi(js["SJData"][S1]["object"]);//对话者
	//				if (!js["SJData"][S1].contains("dhnum")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - dhnum", "W");
	//				else {//对话的数据
	//					Map.SJData.DHData[Map.SJData.DHNum].DHNum = js["SJData"][S1]["dhnum"];//对话的数量
	//					for (int j = 0; j < Map.SJData.DHData[Map.SJData.DHNum].DHNum; j++) {
	//						std::string D2 = "DH" + std::to_string(j);//---定义
	//						if (!js["SJData"][S1].contains(D2)) {//键 SJData     SJi - DHj
	//							Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2, "W");
	//						}//不存在 ---上
	//						else {//存在 ---下
	//							if (!js["SJData"][S1][D2].contains("text")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - text", "W");
	//							else Map.SJData.DHData[Map.SJData.DHNum].DH[j].Text = to_Ansi(js["SJData"][S1][D2]["text"]);//对话内容
	//							if (!js["SJData"][S1][D2].contains("hdnum")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - hdnum", "W");
	//							else {//此对话 回答的数据
	//								Map.SJData.DHData[Map.SJData.DHNum].DH[j].HDNum = js["SJData"][S1][D2]["hdnum"];//此对话 回答的数量
	//								for (int k = 0; k < Map.SJData.DHData[Map.SJData.DHNum].DH[j].HDNum; k++) {
	//									std::string H3 = "HD" + std::to_string(k);//---定义
	//									if (!js["SJData"][S1][D2].contains(H3)) {//键 SJData     SJi - DHj - HDk
	//										Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3, "W");
	//									}//不存在 ---上
	//									else {
	//										if (!js["SJData"][S1][D2][H3].contains("text")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3 + " - text", "W");
	//										else Map.SJData.DHData[Map.SJData.DHNum].DH[j].HD[k].Text = to_Ansi(js["SJData"][S1][D2][H3]["text"]);//此回答的内容
	//										if (!js["SJData"][S1][D2][H3].contains("effect")) Error("地图文件: " + mapway + " 缺失数据 SJData - " + S1 + " - " + D2 + " - " + H3 + " - effect", "W");
	//										else Map.SJData.DHData[Map.SJData.DHNum].DH[j].HD[k].JG = js["SJData"][S1][D2][H3]["effect"];//此回答的效果/结果
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//				Map.SJData.Data[i].Hand = Map.SJData.DHNum;//获取对话地址
	//				Map.SJData.DHNum++;//对话事件数量加一
	//			}
	//		}
	//	}
	//}
	//cls();
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

void GameMapDataClear() {//<---------------------------------------------------------------------------------清空地图数据
	while (Map.Data.size())Map.Data.pop_back();
}