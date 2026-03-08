#pragma once

#include<iostream>
#include<string>
#include<graphics.h>
#include"json.hpp"



extern const std::wstring DataWayParent;//-----文件夹路径 游戏数据
extern const std::wstring SetWayParent;//------文件夹路径 Debug
extern const std::wstring DataParent;//--------文件夹路径 数据 

extern const std::wstring MapParent;//---------文件夹路径 地图
extern const std::wstring ImageParent;//-------文件夹路径 图片

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
extern bool BoolMapMessage;//------地图信息显示是否开启  
extern bool BoolWarn;//------------是否显示警告状态 

extern int FPS;//-------------------FPS数值 
extern int FPSWeek;//---------------FPS循环周期 
extern long long GameRunTime;//-----游戏时刻 


struct StructPlayer {//玩家信息
	int LastX, LastY;//---------玩家上次的位置
	int NextX, NextY;//---------玩家行动的位置
	int myx, myy;//-------------玩家位置
	float CameraX, CameraY;//---摄像机位置
	float PmX, PmY;//-----------显示的坐标
	float Life;//---------------玩家生命
	float MaxLife;//------------玩家最大生命
	float Speed;//--------------玩家速度
	std::string MapName;//------当前地图
};extern StructPlayer Player;

struct SM {//StructMap 的缩写 地图信息 
	int maxx, maxy;//-------地图大小
	std::string wjname;//---文件名字
	std::string name;//-----地图名字
	std::string Author = NULLTEXT;//地图作者
	int Data[MapMaxSize][MapMaxSize];//地图数据
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

struct StructImage {//图片信息 
	std::wstring Name;//图片名字 
	std::wstring Way;//图片路径
	IMAGE Img;//图片数据
}; extern StructImage Images[21];

struct StructTime {//时间信息
	clock_t LastTime;//---------------上次时间
	clock_t NowTime;//----------------现在时间
	int JGTime;//---------------------间隔的时间
}; extern StructTime Time;

struct StructInput {//输入信息 
	char up = 'W';//上
	char down = 'S';//下
	char left = 'A';//左
	char right = 'D';//右
	char returnn = 27;//返回
}; extern StructInput PlayerInput;

extern ExMessage Mouse;//定义鼠标数据
extern int MouseX;//鼠标坐标
extern int MouseY;

extern std::string YM;//当前页面

extern std::string MapName[201];//地图列表
extern int MapNum;//地图列表 的地图数量

extern std::string ErrorText;//错误信息
extern int ErrorNum;//错误数量
extern std::string WarnText;//警告信息
extern int WarnNum;//警告数量


void CDW(const std::wstring& str);//<---------------------------------------------------------------------创建文件夹
bool FFFW(const std::wstring& str, const std::string FindMapName);//<-------------------------------------获取文件

void czdata(std::string xz);//<---------------------------------------------------------------------------重置数据 
void GameData(std::string xz, int num);//<----------------------------------------------------------------保存/读取游戏数据

bool OpenJson(const std::string WJWay, const std::string WJName, nlohmann::json& jsin);//<----------------打开JSON 分析
bool GameMapData(std::string mapname);//<-----------------------------------------------------------------读取地图文件 