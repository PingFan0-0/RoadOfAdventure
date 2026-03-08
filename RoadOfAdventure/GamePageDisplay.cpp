#include "GamePageDisplay.h"


#include"GameData.h"
#include<graphics.h>




void ImageDisplay(int x, int y, int size, IMAGE& img) {//<-------------------------------------------------------------图片显示
	putimage(x, y, size, size, &img, 0, 0);
}


void pmsx() {//<---------------------------------------------------------------屏幕刷新 

	const int Hight = 32;//高度

	if (Win.LastSize != Win.Size) {//当缩放大小改变时
		Win.LastSize = Win.Size;//记录缩放大小
		for (int i = 0; i < 6; i++) {
			loadimage(&Images[i].Img, Images[i].Way.c_str(), Win.Size, Win.Size);//加载图片
		}
	}

	cleardevice();// 清屏
	BeginBatchDraw();//将绘图保存在缓存中

	int BY = Player.CameraY - (Win.WinY / 2 / Win.Size + 2) >= 0 ? (int)(Player.CameraY - (Win.WinY / 2 / Win.Size + 2)) : 0;//BeginY
	int EY = Player.CameraY + (Win.WinY / 2 / Win.Size + 2) < Map.maxy ? (int)(Player.CameraY + (Win.WinY / 2 / Win.Size + 2)) : Map.maxy;//EndY
	int BX = Player.CameraX - (Win.WinX / 2 / Win.Size + 2) >= 0 ? (int)(Player.CameraX - (Win.WinX / 2 / Win.Size + 2)) : 0;//BeginX
	int EX = Player.CameraX + (Win.WinX / 2 / Win.Size + 2) < Map.maxx ? (int)(Player.CameraX + (Win.WinX / 2 / Win.Size + 2)) : Map.maxx;//EndX
	int GY = (int)(Win.WinY / 2 - Player.CameraY * Win.Size);
	int GX = (int)(Win.WinX / 2 - Player.CameraX * Win.Size);

	for (int i = BY; i < EY; i++) {//地图显示 
		for (int j = BX; j < EX; j++) {
			int num = Map.Data[i][j];
			if (num == 0)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Images[5].Img);//地板
			if (num == 1)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Images[0].Img);//墙
			if (num == 9)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Images[2].Img);//陷阱
			if (num == 10)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Images[3].Img);//敌人
			if (num == 11)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, Images[4].Img);//NPC
			//else if (num == 1) str += "##";//
			//else if (num == 2) str += "门";
			//else if (num == 9) str += "%%";//
			//else if (num == 10) str += "敌";//
			//else if (num == 11) str += "人";//

		}
	}
	ImageDisplay((int)(Player.PmX * Win.Size + GX), (int)(Player.PmY * Win.Size + GY), Win.Size, Images[1].Img);//绘制玩家

	if (BoolFPS) {//=====FPS显示 
		if (!(GameRunTime % FPSWeek)) {
			FPS = 1000 / Time.JGTime;
		}
		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		settextcolor(RGB(255, 255, 255));//设置文字颜色
		outtextxy(0, 0, (L"FPS:" + std::to_wstring(FPS) + L" " + std::to_wstring(Time.JGTime) + L"ms Size:" + std::to_wstring(Win.Size)).c_str());
	}

	settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
	settextcolor(RGB(255, 255, 255));//设置文字颜色
	outtextxy(0, 15, (L"" + std::to_wstring(Player.PmX) + L" " + std::to_wstring(Player.PmY)).c_str());

	FlushBatchDraw();//将绘图从缓存中绘制到屏幕
}