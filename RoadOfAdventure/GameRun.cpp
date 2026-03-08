#include "GameRun.h"

#include"GameData.h"
#include"GamePageDisplay.h"
#include"GameLife.h"
#include"Time.h"
#include"GameInput.h"

bool gamestart() {//<----------------------------------------------------------游戏初始化
	GameData("GDR", -1);//读取游戏数据
	Player.PmX = (float)Player.myx; Player.PmY = (float)Player.myy;//玩家坐标
	Player.CameraX = Player.PmX + 0.5f; Player.CameraY = Player.PmY + 0.5f;//摄像机坐标
	Images[0].Name = L"墙"; Images[1].Name = L"玩家"; Images[2].Name = L"陷阱"; Images[3].Name = L"敌人"; Images[4].Name = L"NPC"; Images[5].Name = L"背景";
	const int Size = 1000;
	for (int i = 0; i < 6; i++) {
		Images[i].Way = DataParent + L"/" + ImageParent + L"/" + Images[i].Name + L".jpg";
		loadimage(&Images[i].Img, Images[i].Way.c_str(), Size, Size);//加载图片
		loadimage(&Images[i].Img, Images[i].Way.c_str(), Win.Size, Win.Size);//加载图片
	}
	return GameMapData(Player.MapName);//加载对应地图 返回false说明地图打开失败
}

void SaveData(int Week) {//<-----------------------------------------------------------游戏自动保存 
	if (GameRunTime % Week == 0) {//条件判定 
		GameData("GDW", 1);//保存游戏 
	}
}

void GameOnRun() {//<----------------------------------------------------------游戏循环
	TimeMath();//时间计算
	pmsx();//屏幕刷新
	shpd();//伤害判定
	swpd();//死亡判定
	Lifeup();//生命自然回复 
	playerinput();//玩家输入 
	//MapSJ();//地图特殊事件
	SaveData(10000);//自动保存 10s
	GameRunTime++;//每循环一次游戏时刻加1
	Sleep(1);//游戏延迟
}