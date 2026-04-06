#include "TypeConversion.h"

#include <string>
#include <vector>
#include<Windows.h>

std::string wstring_string(std::wstring wstr) {//------------------------------------------------ wstring --> string-8
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string strr(len, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &strr[0], len, NULL, NULL);
	strr.pop_back();//去除最后的一个字符   在此是 ' '<---空格 
	return strr;
}

std::wstring to_wstring(const std::string gbkstr) {//------------------------------------------- string --> wstring
	if (gbkstr.empty()) return std::wstring();
	// CP_ACP 表示当前系统默认 ANSI 代码页（简体中文即为 GBK）
	int size_needed = MultiByteToWideChar(CP_ACP, 0, gbkstr.data(), (int)gbkstr.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, gbkstr.data(), (int)gbkstr.size(), &wstr[0], size_needed);
	return wstr;
}

std::string to_utf8(const std::string ansi) {//------------------------------------------------ String --> UFT-8
	// 1. ANSI → UTF?16 (获取所需长度，不含 null)
	int wlen = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), (int)ansi.size(), nullptr, 0);
	if (wlen == 0) return "";
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), (int)ansi.size(), &wstr[0], wlen);
	// 2. UTF?16 → UTF?8 (获取所需长度，不含 null)
	int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	if (ulen == 0) return "";
	std::string utf8(ulen, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &utf8[0], ulen, nullptr, nullptr);
	return utf8;   // utf8.size() == ulen，无多余 null
}

std::string to_Ansi(const std::string utf8Str) {//------------------------------------------------ UFT-8 --> String
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