#include "Begin.h"

#include<iostream>
#include<string>
#include"Debug.h"
#include"GameData.h"
#include"PageDisplay.h"
#include<json.hpp>
#include<fstream>
#include"TypeConversion.h"
#include"OpenGL.h"
#include"Image.h"
#include"TextRenderer.h"
#include"Time.h"
#include"DrawShape.h"

#pragma region PF
const char* PF =
R"(
####   #####    ###          ###  
#   #  #       #   #        #   # 
#   #  #       #   #        #   # 
####   ####    #   #        #   # 
#      #       #   #        #   # 
#      #       #   #        #   # 
#      #        ###   #####  ###  
)";

#pragma endregion


std::vector<std::string> images;


// --------- 添加图片资源 --------------
int AddImage(std::string way) {
	if (way == "null")return 0;
	for (int i = 0; i < images.size(); i++) {
		if (images[i]== way) {
			return i;
		}
	}//找不到 设置贴图
	images.emplace_back();
	images[images.size() - 1] = way;
	return (int)images.size() - 1;
}
// --------- 分析图片资源 --------------
int ProcessImage() {
	atlas = createTextureAtlas(images, regions, 64, 2048);
	if (atlas == 0) {
		std::cerr << "图集创建失败" << std::endl;
		return -1;
	}

	// 输出每个子图的 UV 范围
	for (const auto& reg : regions) {
		Debug("图集区域: " + reg.name + " -> UV: (" +
			std::to_string(reg.uvMin.x) + "," + std::to_string(reg.uvMin.y) + ") to (" +
			std::to_string(reg.uvMax.x) + "," + std::to_string(reg.uvMax.y) + ")");
	}

	// 渲染时绑定图集
	glBindTexture(GL_TEXTURE_2D, atlas);

	return 0;
}

bool Begin() {
#pragma region 日志 ===============
	Debug("sss");//清空日志
	DebugError("sss");//清空错误日志
	DebugWarn("sss");//清空警告日志
#pragma endregion
#pragma region 文件 ===============
	GameData("SDR", -1);//读取设置数据
	Debug("初始化");

	CDW(DataParent);//创建文件夹 Data
	CDW(DataWayParent);//创建文件夹 GameData 
	CDW(SetWayParent);//创建文件夹 Set
	CDW(DataWayParent + L"/" + MapParent);//创建文件夹 Set
#pragma endregion
#pragma region 初始化 OpenGL ======
	Debug("创建游戏窗口 " + std::to_string(Win.WinX) + " " + std::to_string(Win.WinY));
	if (!GLBegin()) {//创建窗口
		return false;
	}

	{
		int maxUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
		Debug("您的的设备支持的纹理单元数量上限为: " + std::to_string(maxUnits));
	}

	DrawRectShape::GetInstance().Init(Win.WinX, Win.WinY);//全局矩形渲染器初始化
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
		DrawRectShape::GetInstance().UpdateProjection(w, h);
		});

#pragma endregion
#pragma region 游戏资源 ===========
	Debug("加载游戏资源=====");
	images.clear();//清除图片列表
	{
		std::string nullimage = "null.jpg";//null图片资源
		std::string Nway = FindFile(DataParent + L"/" + ImageParent, nullimage);//获取路径
		if (Nway != "null") {
			images.emplace_back();
			images[0] = Nway;
		}
		else Error("缺失文件: " + nullimage + " 图片数据缺失", "W");
	}

	int t1 = clock();
	std::vector <FileData> FileData = FindJsonAll(DataParent + L"/" + InformationParent);//<-----------------------------查找文件
	Debug("扫描了 "+ std::to_string(FileData.size()) + " 个json资源文件==  花费 " + std::to_string((float)(clock() - t1) / 1000)); t1 = clock();

	for (int i = 0; i < FileData.size(); i++) {
		nlohmann::json js;
		std::string suffix = ".jpg";
		if (OpenJson(FileData[i].way, FileData[i].name, js)) {//读取文件
			CentralData.Data.push_back({ FileData[i].name, js["type"],0 , FileData[i].way });//将数据存入中心数据
			if (js.contains("name")) CentralData.Data[i].name = to_Ansi(js["name"]);//name-----
			else Error("数据文件: " + FileData[i].name + " 缺失数据 name", "W");
			if (CentralData.Data[i].type == "back") {//如果是背景===============
				CentralData.Data[i].Hand = (int)CentralData.BackData.size();//写入指针
				CentralData.BackData.emplace_back();
				if (js.contains("sizex")) CentralData.BackData[CentralData.BackData.size() - 1].sizex = js["sizex"];//sizex-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizex", "W");
				if (js.contains("sizey")) CentralData.BackData[CentralData.BackData.size() - 1].sizey = js["sizey"];//sizey-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizey", "W");
				std::string imagename = CentralData.Data[i].name + suffix;//IMAGE-----
				if (js.contains("image")) imagename = to_Ansi(js["image"]);//获取贴图名称 
				CentralData.BackData[CentralData.BackData.size() - 1].ImageHand = AddImage(FindFile(DataParent + L"/" + ImageParent, imagename));//返回图片地址  根据名称获取路径
			}
			else if (CentralData.Data[i].type == "building") {//建筑===============
				CentralData.Data[i].Hand = (int)CentralData.BuildingData.size();//写入指针
				CentralData.BuildingData.emplace_back();
				if (js.contains("sizex")) CentralData.BuildingData[CentralData.BuildingData.size() - 1].sizex = js["sizex"];//sizex-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizex", "W");
				if (js.contains("sizey")) CentralData.BuildingData[CentralData.BuildingData.size() - 1].sizey = js["sizey"];//sizey-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizey", "W");
				std::string imagename = CentralData.Data[i].name + suffix;//IMAGE-----
				if (js.contains("image")) imagename = to_Ansi(js["image"]);//获取贴图名称 
				CentralData.BuildingData[CentralData.BuildingData.size() - 1].ImageHand = AddImage(FindFile(DataParent + L"/" + ImageParent, imagename));//返回图片地址  根据名称获取路径
			}
			else if (CentralData.Data[i].type == "unit") {//单位===============
				CentralData.Data[i].Hand = (int)CentralData.UnitData.size();//写入指针
				CentralData.UnitData.emplace_back();
				CentralData.UnitData[CentralData.UnitData.size() - 1].Name = CentralData.Data[i].name;//name-----
				if (js.contains("F")) CentralData.UnitData[CentralData.UnitData.size() - 1].F = js["F"];//牵引力-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 F牵引力 ", "W");
				if (js.contains("M")) CentralData.UnitData[CentralData.UnitData.size() - 1].M = js["M"];//质量M-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 M质量 ", "W");
				if (js.contains("life")) CentralData.UnitData[CentralData.UnitData.size() - 1].Life = js["life"];//Life-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 Life最大生命 ", "W");
				if (js.contains("sizex")) CentralData.UnitData[CentralData.UnitData.size() - 1].sizex = js["sizex"];//sizex-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizex宽 ", "W");
				if (js.contains("sizey")) CentralData.UnitData[CentralData.UnitData.size() - 1].sizey = js["sizey"];//sizey-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizey高 ", "W");
				std::string imagename = CentralData.Data[i].name + suffix;//IMAGE-----
				if (js.contains("image")) imagename = to_Ansi(js["image"]);//获取贴图名称 
				CentralData.UnitData[CentralData.UnitData.size() - 1].ImageHand = AddImage(FindFile(DataParent + L"/" + ImageParent, imagename));//返回图片地址  根据名称获取路径
			}
			else if (CentralData.Data[i].type == "wall") {//墙===============
				CentralData.Data[i].Hand = (int)CentralData.WallData.size();//写入指针
				CentralData.WallData.emplace_back();
				if (js.contains("sizex")) CentralData.WallData[CentralData.WallData.size() - 1].sizex = js["sizex"];//sizex-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizex", "W");
				if (js.contains("sizey")) CentralData.WallData[CentralData.WallData.size() - 1].sizey = js["sizey"];//sizey-----
				else Error("数据文件: " + FileData[i].name + " 缺失数据 sizey", "W");
				std::string imagename = CentralData.Data[i].name + suffix;//IMAGE-----
				if (js.contains("image")) imagename = to_Ansi(js["image"]);//获取贴图名称 
				CentralData.WallData[CentralData.WallData.size() - 1].ImageHand = AddImage(FindFile(DataParent + L"/" + ImageParent, imagename));//返回图片地址  根据名称获取路径
			}
			else if (CentralData.Data[i].type == "item") {//墙===============
				CentralData.Data[i].Hand = (int)CentralData.ItemData.size();//写入指针
				CentralData.ItemData.emplace_back();
				CentralData.ItemData[CentralData.ItemData.size() - 1].Name = CentralData.Data[i].name;//name-----
				std::string imagename = CentralData.Data[i].name + suffix;//IMAGE-----
				if (js.contains("image")) imagename = to_Ansi(js["image"]);//获取贴图名称
				if (js.contains("introduction")) CentralData.ItemData[CentralData.ItemData.size() - 1].Introduction = to_Ansi(js["introduction"]);//introduction-----
				else CentralData.ItemData[CentralData.ItemData.size() - 1].Introduction = "null";
				CentralData.ItemData[CentralData.ItemData.size() - 1].ImageHand = AddImage(FindFile(DataParent + L"/" + ImageParent, imagename));//返回图片地址  根据名称获取路径
			}
		}
		//js.clear();
	}
	Debug("分析了 " + std::to_string(CentralData.Data.size()) + " 个有效资源文件==  花费 " + std::to_string((float)(clock() - t1) / 1000)); t1 = clock();

	ProcessImage();//处理图片资源
	Debug("处理了 " + std::to_string(images.size()) + " 个 贴图资源== 花费 " + std::to_string((float)(clock() - t1) / 1000)); t1 = clock();
	Debug("=====加载游戏资源");
#pragma endregion

	BoolTheGame = true;//游戏主循环开关
	YM = "begin";//设置页面为初始页面
	Error("", "R");
	TimeMath();//时间计算

	return true;
}