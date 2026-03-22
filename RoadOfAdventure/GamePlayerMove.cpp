#include "GamePlayerMove.h"



#include"GameData.h"


//bool IfMove(int x, int y) {//<-----------------------------------------------------------------------是否可以移动 
//	int num = Map.Data[to_MapHand(x, y)].floor;//获取地图数据 
//	if (num == 0 || num == 2 || num == 9 || num == 11) {//可以通过
//		return true;
//	}
//	return false;
//}

void PlayerMove(int xx, int yy, float Speed) {//<-----------------------------------------------------------------------玩家移动
	//const float ENum = 0.99999f;//误差值 用于修正坐标
	////=========== X ==========
	//if (xx != 0) {//当玩家 X轴 移动时
	//	float NowX = Player.PmX;//当前坐标 x
	//	float ToX = Player.PmX + xx * Speed;//目标坐标 x
	//	float NowY = Player.PmY;//当前坐标 y
	//	if (Speed >= 1) {//当移动量大于等于1时 可能会跨格子
	//		//Null
	//	}
	//	Player.PmX = ToX;//移动
	//	if (xx < 0) {//左移动
	//		if (!IfMove((int)ToX, (int)NowY) || !IfMove((int)ToX, (int)(NowY + ENum))) Player.PmX = (float)(int)(ToX + 1);//修正坐标
	//	}
	//	else {//右移动
	//		if (!IfMove((int)(ToX + 1), (int)NowY) || !IfMove((int)(ToX + 1), (int)(NowY + ENum))) Player.PmX = (float)(int)ToX;//修正坐标
	//	}
	//	Player.myx = (int)Player.PmX;//当前坐标
	//	Player.CameraX = Player.PmX;//摄像机坐标
	//	if ((int)Player.LastX != Player.myx)Player.LastX = Player.NextX;//经过坐标
	//}
	////=========== Y ==========
	//if (yy != 0) {//当玩家 X轴 移动时
	//	float NowY = Player.PmY;//当前坐标 x
	//	float ToY = Player.PmY + yy * Speed;//目标坐标 x
	//	float NowX = Player.PmX;//当前坐标 y
	//	if (Speed >= 1) {//当移动量大于等于1时 可能会跨格子
	//		//Null
	//	}
	//	Player.PmY = ToY;//移动
	//	if (yy < 0) {//上移动
	//		if (!IfMove((int)NowX, (int)ToY) || !IfMove((int)(NowX + ENum), (int)ToY)) Player.PmY = (float)(int)(ToY + 1);//修正坐标
	//	}
	//	else {//下移动
	//		if (!IfMove((int)NowX, (int)(ToY + 1)) || !IfMove((int)(NowX + ENum), (int)(ToY + 1))) Player.PmY = (float)(int)ToY;//修正坐标
	//	}
	//	Player.myy = (int)Player.PmY;//当前坐标
	//	Player.CameraY = Player.PmY;//摄像机坐标
	//	if ((int)Player.LastY != Player.myy)Player.LastY = Player.NextY;//经过坐标
	//}
}