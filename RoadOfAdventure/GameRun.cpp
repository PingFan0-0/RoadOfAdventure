#include "GameRun.h"


#include "Shader.h"
#include "Image.h"
#include "Model.h"
#include "OpenGL.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Texture.h"
#include "UIClickableText.h"
#include "Vertex.h"

#include"Camera.h"
#include"Game.h"

extern UIClickableTextManager& uiManager;





bool gamestart() {//<----------------------------------------------------------游戏初始化
	GameData("GDR", -1);//读取游戏数据
	return true;
	return GameMapData(Player.MapName);//加载对应地图 返回false说明地图打开失败
}

void SaveData(int Week) {//<-----------------------------------------------------------游戏自动保存 
	if (GameRunTime % Week == 0) {//条件判定 
		GameData("GDW", 1);//保存游戏 
	}
}

uint8_t FindUnit(std::string name) {
	for (int i = 0; i < CentralData.Data.size(); i++) {
		if (CentralData.Data[i].type == "unit" && CentralData.Data[i].name == name)return i;
	}
	return 0;
}

void GameOnRun() {//<----------------------------------------------------------游戏循环

	Game engine;


	if (gamestart() && engine.init()) {
		Debug("开始游戏");
		engine.MapData.MapName = Player.MapName;
		engine.MapData.MapWay = wstring_string(DataParent + L"/" + MapParent);
		engine.MapData.MapSaveWay = wstring_string(DataWayParent + L"/" + MapParent);
		engine.createWorld(10);   // 加载区块区块
#pragma region OpenGL设置 =============
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标
		glEnable(GL_DEPTH_TEST);//开启深度测试
		if (BoolMod) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
		else  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//默认模式
		glEnable(GL_BLEND);// 启用混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 设置混合函数
#pragma endregion

		
		Player.Player = UnitAdd(FindUnitData("人"));
		UnitData[Player.Player].SetXYZ(1.5f, 1.5f, 1.5f);
		for (int i = 1; i <= 100; i++) UnitAdd(FindUnitData("人"), 1.5f, 1.5f);

		Debug("游戏开始 ----- >");//测试日志
		engine.run();
	}
	else {
		Error("地图文件打开失败！", "W");
		MouseX = -1;
		MouseY = -1;
		YM = "begin";//保持在初始界面
	}

	//StaticSpriteBatch staticBatch(atlas);//创建静态精灵批处理
	//SpriteRenderer spriteRenderer(Win.WinX, Win.WinY);//创建精灵渲染器=



	//while (YM == "gameon") {
	//	TimeMath();//时间计算
	//	pmsx(textRenderer, SH, spriteRenderer);//屏幕刷新
	//	shpd();//伤害判定
	//	swpd();//死亡判定
	//	Lifeup();//生命自然回复 
	//	playerinput();//玩家输入 
	//	//MapSJ();//地图特殊事件
	//	SaveData(60000);//自动保存 60s
	//	GameRunTime++;//每循环一次游戏时刻加1
	//	//Sleep(1);//游戏延迟
	//	
	//	for (int i = 0; i < UnitData.size(); i++)UnitData[i].UnitTo();//单位移动计算

	//	if (UnitData[Player.Player].BoolDEL) {
	//		SXY a = UnitData[Player.Player].GetXY();
	//		cameraPos.x = a.X;
	//		cameraPos.y = a.Y;
	//	}
	//}
	GameData("GDW", 1);//保存游戏数据
	UnitDataClear();//清空单位数据
	GameMapDataClear();//清空地图数据

	engine.MapData.clear();
	engine.cleanup();
}