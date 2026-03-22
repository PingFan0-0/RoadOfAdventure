#include "Debug.h"

#include<iostream>
#include<fstream>
#include"GameData.h"



void Debug(std::string str) {//<---------------------------------------------------------日志
	if (!BoolDebug)return;//如果日志记录关闭 就退出函数  
	if (str == "sss") {//清空日志  
		std::ofstream DubugOut(DebugWay);//打开文件 
		DubugOut << "==================================日志==================================\n";
		if (!BoolDebug) DubugOut << "\n日志以记录关闭 可以在设置中打开此功能\n";
		return;//退出 
	}
	std::ofstream DubugOut(DebugWay, std::ios::app);
	DubugOut << "\n[" + std::to_string((float)clock() / 1000) + "] " + str;//写入文件 
}

void DebugError(std::string str) {//<---------------------------------------------------------错误日志 
	if (!BoolDebug)return;//如果日志记录关闭 就退出函数  
	if (str == "sss") {//清空错误日志  
		std::ofstream DubugOut(ErrorWay);//打开文件
		DubugOut << "==================================错误日志==================================";
		if (!BoolDebug) DubugOut << "\n错误日志以记录关闭 可以在设置中打开此功能";
		return;//退出 
	}
	std::ofstream DubugOut(ErrorWay, std::ios::app);//打开文件
	DubugOut << "\n[" + std::to_string((float)clock() / 1000) + "] " + str;;//写入文件
}

void DebugWarn(std::string str) {//<---------------------------------------------------------警告日志
	if (!BoolDebug)return;//如果日志记录关闭 就退出函数 
	if (str == "sss") {//清空警告日志 
		std::ofstream DubugOut(WarnWay);//打开文件
		DubugOut << "==================================警告日志==================================";
		if (!BoolDebug) DubugOut << "\n警告日志以记录关闭 可以在设置中打开此功能";
		return;//退出
	}
	std::ofstream DubugOut(WarnWay, std::ios::app);//打开文件
	DubugOut << "\n[" + std::to_string((float)clock() / 1000) + "] " + str;;//写入文件
}

void Error(const std::string Text, const std::string Type) {//<----------------------------------------------------错误显示
	if (Type == "W") {//写入错误
		DebugError("发生错误 " + std::to_string(ErrorNum) + ". " + Text);
		ErrorText += "| " + std::to_string(ErrorNum) + ". " + Text + "\n";
		ErrorNum++;//错误数量加一
	}
	else if (Type == "R") {//显示错误
		Error("", "sss");
	}
	else if (Type == "sss") {//清空错误
		Debug("发现错误数:  " + std::to_string(ErrorNum));
		DebugError("目前发现错误数:  " + std::to_string(ErrorNum));
		DebugError("======================================================");
		ErrorText = "";
		ErrorNum = 0;
	}
}

void Warn(const std::string Text, const std::string Type) {//<----------------------------------------警告显示
	if (Type == "W") {//写入警告
		DebugWarn("发生警告 " + std::to_string(WarnNum) + ". " + Text);
		WarnText += "| " + std::to_string(WarnNum) + ". " + Text + "\n";
		WarnNum++;//警告数量加一
	}
	else if (Type == "R") {//显示警告
		Warn("", "sss");
	}
	else if (Type == "sss") {//清空警告
		Debug("发现警告数:  " + std::to_string(WarnNum));
		DebugWarn("目前发现警告数:  " + std::to_string(WarnNum));
		DebugWarn("======================================================");
		WarnText = "";
		WarnNum = 0;
	}
}