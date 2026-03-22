#include"GameLife.h"


#include"GameData.h"

void shpd() {//<---------------------------------------------------------------------伤害判定 
	//if (Map.Data[to_MapHand(Player.myx, Player.myy)].floor == 9) {//陷阱判定
	//	Player.Life -= 3;//造成伤害
	//}
	////	Debug(to_string(Player.myx)+" "+to_string(Player.LastX)+" "+to_string(Player.myy)+" "+to_string(Player.LastY),-1);
	//if (Player.myx != Player.LastX || Player.myy != Player.LastY) {//敌人判定 
	//	int drsl = 0;//敌人数量 
	//	int numX, numY;
	//	numX = Player.myx;
	//	numY = Player.myy;
	//	for (int i = -1; i <= 1; i++) {
	//		for (int j = -1; j <= 1; j++) {
	//			//if (Map.Data[to_MapHand(numX + j, numY + i)].floor == 10) {//当玩家周围有敌人时
	//			//	drsl++; //敌人数量加一
	//			//}
	//		}
	//	}
	//	//		Debug("敌人判定 ",drsl);
	//	Player.Life -= drsl;//造成伤害
	//}
	//if (Player.Life < 0)Player.Life = 0;//防止生命小与零
}


void swpd() {//<---------------------------------------------------------------------死亡判定 
	//if (Player.Life <= 0) {//当玩家生命小于等于零时
	//	//cz = -1;//切换到死亡界面------------------------------------------------------------------------------------------------------
	//}
}


void Lifeup() {//<-------------------------------------------------------------------生命自然回复 
	//if (GameRunTime % 10 == 0 && Player.Life < Player.MaxLife) {//条件判断 
	//	Player.Life++;//回复生命 
	//}
	//else if (Player.Life > Player.MaxLife) {//防止生命大于最大生命 
	//	Player.Life = Player.MaxLife;
	//}
}