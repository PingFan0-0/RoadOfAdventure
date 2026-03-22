#include "GameInput.h"


#include"GameData.h"
#include"GamePlayerMove.h"

#include"GameUnit.h"

void playerinput() {//<---------------------------------------------------玩家输入 
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {//返回
		YM = "begin";//返回开始菜单
		return;
	}
	int xx = 0, yy = 0;//定义坐标变化量

	if (GetAsyncKeyState(PlayerInput.left) & 0x8000)xx--;//左
	if (GetAsyncKeyState(PlayerInput.right) & 0x8000)xx++;//右
	if (GetAsyncKeyState(PlayerInput.up) & 0x8000)yy--;//上
	if (GetAsyncKeyState(PlayerInput.down) & 0x8000)yy++;//下
	if (xx != 0 || yy != 0) {
		if (UnitData[Player.Player].BoolDEL) UnitData[Player.Player].ToF(xx, yy);//玩家加速度
		else {
			Player.CameraX += xx * 200 / Win.Size * Time.JGTime / 1000;
			Player.CameraY += yy * 200 / Win.Size * Time.JGTime / 1000;
		}
	}

	if (peekmessage(&Mouse, EX_MOUSE)) {//获取鼠标消息
		if (Mouse.message == WM_MOUSEWHEEL) {//鼠标滚轮
			if (Mouse.wheel > 0) {
				Win.Size += (int)(Win.Size * 0.2);
			}
			else {
				Win.Size -= (int)(Win.Size * 0.2);
			}
			if (Win.Size > Win.WinY / 3) Win.Size = Win.WinY / 3;
			if (Win.Size < Win.WinY / 120) Win.Size = Win.WinY / 120;
		}
	}
}