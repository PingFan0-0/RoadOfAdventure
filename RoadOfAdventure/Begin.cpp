#include "begin.h"

#include<iostream>
#include"Debug.h"
#include"GameData.h"
#include"PageDisplay.h"

void Begin() {
	Debug("sss");//清空日志
	DebugError("sss");//清空错误日志
	DebugWarn("sss");//清空警告日志

	GameData("SDR", -1);//读取设置数据
	Debug("初始化");
	CDW(DataParent);//创建文件夹 Data
	CDW(DataWayParent);//创建文件夹 GameData 
	CDW(SetWayParent);//创建文件夹 Set
	NewWindows(Win.WinX, Win.WinY);//创建窗口
	BoolTheGame = true;//游戏主循环开关
	YM = "begin";//设置页面为初始页面

	std::cout << "附件";
}