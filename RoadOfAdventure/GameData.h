#pragma once

#include<iostream>
#include<string>
//#include<graphics.h>
#include<json.hpp>
#include<vector>



extern const std::wstring DataWayParent;//-----文件夹路径 游戏数据
extern const std::wstring SetWayParent;//------文件夹路径 Debug
extern const std::wstring DataParent;//--------文件夹路径 数据 

extern const std::wstring MapParent;//---------文件夹路径 地图
extern const std::wstring ImageParent;//-------文件夹路径 图片
extern const std::wstring InformationParent;//-文件夹路径 json
extern const std::wstring ShaderParent;//------文件夹路径 着色器
extern const std::wstring FontsParent;//-------文件夹路径 字体

extern const std::string GameDataWay;//------文件名称 游戏数据
extern const std::string SetDataWay;//-------文件名称 设置数据
extern const std::string DebugWay;//---------文件名称 Debug
extern const std::string ErrorWay;//---------文件名称 Error
extern const std::string WarnWay;//----------文件名称 Warn

extern const std::string BeginMap;//---------初始的地图 

extern const std::string NULLTEXT;//---------无/Null

const int MapMaxSize = 105;//定义地图内存的大小 

extern bool BoolTheGame;//---------游戏是否运行  
extern bool BoolDebug;//-----------日志Debug是否开启 
extern bool BoolZbxs;//------------坐标显示是否开启  
extern bool BoolGameRunTime;//-----游戏时刻显示是否开启  
extern bool BoolFPS;//-------------FPS显示是否开启  
extern bool BoolVxs;//-------------速度显示是否开启 
extern bool BoolMapMessage;//------地图信息显示是否开启  
extern bool BoolWarn;//------------是否显示警告状态 

extern int FPS;//-------------------FPS数值 
extern int FPSWeek;//---------------FPS循环周期 
extern long long GameRunTime;//-----游戏时刻 


extern std::string FontMain;//字体信息


struct StructPlayer {//玩家信息
	float CameraX, CameraY;//---摄像机位置
	std::string MapName;//------当前地图
	int Player;//---------------角色
};extern StructPlayer Player;

struct MapData {//地图数据 
	uint16_t floor;//-----地面
	uint16_t wall;//------墙壁
	//std::string object;//----物体
};

struct SM {//StructMap 的缩写 地图信息 
	int maxx, maxy;//-------地图大小
	std::string wjname;//---文件名字
	std::string name;//-----地图名字
	std::string Author = NULLTEXT;//地图作者
	
	std::vector<MapData> Data;
	//int Data[MapMaxSize][MapMaxSize];//地图数据
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
	};SJData SJData;
}; extern SM Map;

struct StructWin {//窗口信息 
	int WinX;//窗口宽度
	int WinY;//窗口高度
	int Size;//每个格子的像素大小
	int LastSize;//上次每个格子的像素大小
};extern StructWin Win;




//struct StructImage {//图片信息 
//	std::string imageway;//图片路径
//	IMAGE image;//图片数据
//};
//extern std::vector <StructImage> ImageData;//建筑数据

struct StructCentralData {//中心数据
	struct CentralData {//数据-----
		std::string name;//名字
		std::string type;//类型
		uint32_t Hand;//指针
		std::string dataway;//数据路径
	};
	//struct StructImage {//图片信息-----
	//	std::string imageway;//图片路径
	//	IMAGE image;//图片数据
	//};
	struct StructBuilding {//建筑数据-----
		uint8_t sizex;//x
		uint8_t sizey;//y
		uint16_t ImageHand;//贴图指针
	};
	struct StructUnit {//单位数据-----
		std::string Name;//名字
		float sizex;//x
		float sizey;//y
		float sizez;//z
		float WithMaxG;//最大承受G
		float Life;//血条
		float F;//牵引力
		float M;//质量
		uint16_t ImageHand;//贴图指针
	};
	struct StructBlock {//方块数据-----
		std::string Name;
		uint8_t sizex;//x
		uint8_t sizey;//y
		uint16_t ImageHand;//贴图指针
	};
	struct StructItem {//物品数据-----
		std::string Name;
		std::string Introduction;
		uint16_t ImageHand;//贴图指针
	};
	std::vector <CentralData> Data;//数据
	//std::vector <StructImage> ImageData;//图像数据
	std::vector <StructBuilding> BuildingData;//建筑数据
	std::vector <StructUnit> UnitData;//单位数据
	std::vector <StructBlock> BlockData;//方块数据
	std::vector <StructItem> ItemData;//物品数据
}; extern StructCentralData CentralData;

extern std::unordered_map<uint16_t, uint16_t> BlockDictionary;  //方块字典数据


struct StructTime {//时间信息
	float LastTime;//---------------上次时间
	float NowTime;//----------------现在时间
	float JGTime;//---------------------间隔的时间
	int FPS;//
}; extern StructTime Time;

struct StructInput {//输入信息 
	char up = 'W';//上
	char down = 'S';//下
	char left = 'A';//左
	char right = 'D';//右
	char returnn = 27;//返回
}; extern StructInput PlayerInput;

//extern ExMessage Mouse;//定义鼠标数据s
extern double MouseX;//鼠标坐标
extern double MouseY;

extern std::string YM;//当前页面

extern std::string MapName[201];//地图列表
extern int MapNum;//地图列表 的地图数量


void CDW(const std::wstring& str);//<---------------------------------------------------------------------创建文件夹
bool FFFW(const std::wstring& str, const std::string FindMapName);//<-------------------------------------获取文件
std::string FindFile(const std::wstring& str, const std::string FindName);//<-----------------------------查找文件
/**/struct FileData {std::string name;std::string way;};//文件数据--->name:文件名字 way:文件路径--\|/
std::vector <FileData> FindFileAll(const std::wstring& str);//<-------------------------------------------查找所有文件
std::vector <FileData> FindJsonAll(const std::wstring& str);//<-------------------------------------------查找所有Json文件

void czdata(std::string xz);//<---------------------------------------------------------------------------重置数据 
void GameData(std::string xz, int num);//<----------------------------------------------------------------保存/读取游戏数据

int to_MapHand(int x, int y);//<--------------------------------------------------------------------------坐标转换为地图数据序号
bool OpenJson(const std::string Way, const std::string WJName, nlohmann::json& jsin);//<------------------打开JSON 分析
int FindData(std::string name);//-------------------------------------------------------------------------寻找中心数据
int FindUnitData(std::string name);//---------------------------------------------------------------------寻找Unit数据
int FindBlockData(std::string name);//--------------------------------------------------------------------寻找Block数据
bool GameMapData(std::string mapname);//<-----------------------------------------------------------------读取地图文件 
void GameMapDataClear();//<-------------------------------------------------------------------------------清空地图数据 