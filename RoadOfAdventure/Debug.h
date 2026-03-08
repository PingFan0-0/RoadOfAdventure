#pragma once

#include<string>

void Debug(std::string str);     //<---------------------------------------------------------日志
void DebugError(std::string str);//<---------------------------------------------------------错误日志 
void DebugWarn(std::string str); //<---------------------------------------------------------警告日志

void Error(const std::string Text, const std::string Type);//<-------------------------------错误显示
void Warn(const std::string Text, const std::string Type);//<--------------------------------警告显示