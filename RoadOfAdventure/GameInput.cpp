#include "GameInput.h"


#include"GameData.h"
#include"GamePlayerMove.h"


void playerinput() {//<---------------------------------------------------玩家输入 
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {//返回
		GameData("GDW", 1);//保存游戏数据
		YM = "begin";//返回开始菜单
	}
	int xx = 0, yy = 0;//定义坐标变化量

	if (GetAsyncKeyState(PlayerInput.left) & 0x8000)xx--;//左
	if (GetAsyncKeyState(PlayerInput.right) & 0x8000)xx++;//右
	if (GetAsyncKeyState(PlayerInput.up) & 0x8000)yy--;//上
	if (GetAsyncKeyState(PlayerInput.down) & 0x8000)yy++;//下
	if (xx != 0 || yy != 0) PlayerMove(xx, yy, Player.Speed * Time.JGTime / 1000);//玩家移动

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