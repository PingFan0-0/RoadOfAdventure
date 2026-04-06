#pragma once

#include<string>


std::string wstring_string(std::wstring wstr);//------------------------------------------------ wstring --> string-8 
std::wstring to_wstring(const std::string str);//----------------------------------------------- string  --> wstring  
std::string to_utf8(const std::string ansi);//-------------------------------------------------- String  --> UFT-8  
std::string to_Ansi(const std::string utf8Str);//----------------------------------------------- UFT-8   --> String   
//std::vector<unsigned long> utf8ToUnicode(const std::string& utf8Str);//------------------------- UTF-8   --> Unicode 