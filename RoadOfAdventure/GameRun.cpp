#include "GameRun.h"

#include"GameData.h"
#include"GamePageDisplay.h"
#include"GameLife.h"
#include"Time.h"
#include"GameInput.h"

#include"GameUnit.h"

bool gamestart() {//<----------------------------------------------------------游戏初始化
	GameData("GDR", -1);//读取游戏数据
	//Player.PmX = (float)Player.myx; Player.PmY = (float)Player.myy;//玩家坐标
	//Player.CameraX = Player.PmX + 0.5f; Player.CameraY = Player.PmY + 0.5f;//摄像机坐标
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

	Player.Player = UnitAdd(FindUnitData("人"));
	UnitData[Player.Player].SetXY(1.5f, 1.5f);


	for (int i = 1; i <= 100; i++) UnitAdd(FindUnitData("人"), 1.5f, 1.5f);

	while (YM == "gameon") {

		TimeMath();//时间计算
		pmsx();//屏幕刷新
		shpd();//伤害判定
		swpd();//死亡判定
		Lifeup();//生命自然回复 
		playerinput();//玩家输入 
		//MapSJ();//地图特殊事件
		SaveData(60000);//自动保存 60s
		GameRunTime++;//每循环一次游戏时刻加1
		//Sleep(1);//游戏延迟
		
		for (int i = 0; i < UnitData.size(); i++)UnitData[i].UnitTo();//单位移动计算

		if (UnitData[Player.Player].BoolDEL) {
			SXY a = UnitData[Player.Player].GetXY();
			Player.CameraX = a.X;
			Player.CameraY = a.Y;
		}
	}

	GameData("GDW", 1);//保存游戏数据
	UnitDataClear();//清空单位数据
	GameMapDataClear();//清空地图数据
}