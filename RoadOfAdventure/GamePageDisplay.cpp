#include "GamePageDisplay.h"


#include"GameData.h"
#include<graphics.h>
#include"TypeConversion.h"

#include"GameUnit.h"

// 参数：左上角 (x,y)，宽 w 高 h，颜色 color，透明度 alpha (0~255)
void drawSemiRect(int x, int y, int w, int h, COLORREF color, int alpha){//透明矩形
	IMAGE img(w, h);
	SetWorkingImage(&img);
	setfillcolor(color);
	solidrectangle(0, 0, w, h);

	DWORD* pImg = GetImageBuffer(&img);
	DWORD* pWin = GetImageBuffer();
	int winW = getwidth();

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			int idxImg = i * w + j;
			int idxWin = (y + i) * winW + (x + j);
			COLORREF fg = pImg[idxImg];
			COLORREF bg = pWin[idxWin];

			int R = (GetRValue(fg) * alpha + GetRValue(bg) * (255 - alpha)) / 255;
			int G = (GetGValue(fg) * alpha + GetGValue(bg) * (255 - alpha)) / 255;
			int B = (GetBValue(fg) * alpha + GetBValue(bg) * (255 - alpha)) / 255;
			pWin[idxWin] = RGB(R, G, B);
		}

	SetWorkingImage(NULL);
}

void ImageDisplay(int x, int y, int size, IMAGE& img) {//<-------------------------------------------------------------图片显示
	putimage(x, y, size, size, &img, 0, 0);
	//putimage(100, 100, 300, 200, &img, 0, 0, img.getwidth(), img.getheight(), SRCCOPY);
}
void ImageDisplay(int x, int y, int size, SXY Size, IMAGE& img) {//<-------------------------------------------------------------图片显示
	putimage((int)(x - Size.X * size / 2), int(y - Size.Y * size / 2), (int)(Size.X * size), (int)(Size.Y * size), &img, 0, 0);
	//putimage(100, 100, 300, 200, &img, 0, 0, img.getwidth(), img.getheight(), SRCCOPY);
}


void pmsx() {//<---------------------------------------------------------------屏幕刷新 

	const int Hight = 32;//高度

	if (Win.LastSize != Win.Size) {//当缩放大小改变时
		Win.LastSize = Win.Size;//记录缩放大小
		for (int i = 0; i < CentralData.BackData.size(); i++) {//加载图片 BackData
			loadimage(&CentralData.ImageData[CentralData.BackData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.BackData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
		}
		for (int i = 0; i < CentralData.BuildingData.size(); i++) {//加载图片 BuildingData
			loadimage(&CentralData.ImageData[CentralData.BuildingData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.BuildingData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
		}
		for (int i = 0; i < CentralData.UnitData.size(); i++) {//加载图片 UnitData
			loadimage(&CentralData.ImageData[CentralData.UnitData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[i].ImageHand].imageway).c_str(), (int)(Win.Size * CentralData.UnitData[i].sizex), (int)(Win.Size * CentralData.UnitData[i].sizex));
		}
		for (int i = 0; i < CentralData.WallData.size(); i++) {//加载图片 WallData
			loadimage(&CentralData.ImageData[CentralData.WallData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.WallData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
		}
		//加载图片 IMAGENULL
		loadimage(&CentralData.ImageData[0].image, to_wstring(CentralData.ImageData[0].imageway).c_str(), Win.Size, Win.Size);
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
			int floor = Map.Data[to_MapHand(j, i)].floor;
			if(floor == 0)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.ImageData[CentralData.BackData[CentralData.Data[floor].Hand].ImageHand].image);//地板
			else ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.ImageData[CentralData.WallData[CentralData.Data[floor].Hand].ImageHand].image);//墙
		}
	}
	
	for (int i = 0; i < UnitData.size(); i++) {//单位显示 
		SXY a = UnitData[i].GetXY();
		if (UnitData[i].BoolDEL && UnitData[i].IfOnSpace((float)BX, (float)BY, (float)EX, (float)EY)) ImageDisplay((int)(a.X * Win.Size + GX), (int)(a.Y * Win.Size + GY), Win.Size, UnitData[i].GetSize(), CentralData.ImageData[CentralData.UnitData[UnitData[i].GetHand()].ImageHand].image);//绘制单位
	}


	int ULY = 0;//左上 ---------------------------
	{
		if (BoolFPS) {//=====FPS显示 
			settextstyle(15, 0, _T("宋体"));//打印FPS
			settextcolor(RGB(255, 255, 255));//设置文字颜色
			outtextxy(0, 15 * ULY, (L"FPS:" + std::to_wstring(FPS) + L" " + std::to_wstring(Time.JGTime) + L"ms Size:" + std::to_wstring(Win.Size)).c_str());
			ULY++;
		}
		if (BoolZbxs) {
			settextstyle(15, 0, _T("宋体"));//打印坐标
			settextcolor(RGB(255, 255, 255));//设置文字颜色
			SXY a = UnitData[Player.Player].GetXY();//XY
			outtextxy(0, 15 * ULY, (L"" + std::to_wstring(a.X) + L" " + std::to_wstring(a.Y)).c_str());
			ULY++;
		}
		if (BoolVxs) {
			settextstyle(15, 0, _T("宋体"));//打印坐标
			settextcolor(RGB(255, 255, 255));//设置文字颜色
			SXY a = UnitData[Player.Player].GetV();//V
			outtextxy(0, 15 * ULY, (L"" + std::to_wstring(a.X) + L" " + std::to_wstring(a.Y)).c_str());
			ULY++;
		}
	}

	int DLY = 0;//左下 ---------------------------
	{
		int L = Win.WinX / 50;
		int R = Win.WinX / 5;
		int U = Win.WinY - Win.WinY / 5;
		int D = Win.WinY - Win.WinY / 50;
		int X = R - L;
		int Y = D - U;

		setfillcolor(RGB(32, 32, 32) | (16 << 24));
		fillrectangle(L, U, R, D);
		// 
		//drawSemiRect(L, U, X, Y, RGB(255, 255, 255), 128);//实心矩形
		setlinecolor(RED);//线段颜色
		rectangle(L, U, R, D);//空心矩形
		{
			int L1 = L + X / 15;
			int R1 = R - X / 15;
			int U1 = U + Y / 8;
			int D1 = U + Y / 4;
			int X1 = R1 - L1;
			int Y1 = D1 - U1;
			//血条
			setfillcolor(GREEN);//线段颜色
			SNMfloat A = UnitData[Player.Player].GetLife();
			solidrectangle(L1, U1, L1 + (int)(X1 * A.Now / A.Max), D1);//实心矩形
			rectangle(L1, U1, R1, D1);//空心矩形
			//玩家贴图
			int Size1 = (int)(Y / 5);
			U1 = D1 + Y / 8;
			if (UnitData[Player.Player].BoolDEL) {
				loadimage(&CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].imageway).c_str(), Size1, Size1);
				ImageDisplay(L1, U1, Size1, CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image);
				loadimage(&CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].imageway).c_str(), (int)(Win.Size * CentralData.UnitData[UnitData[Player.Player].GetHand()].sizex), (int)(Win.Size * CentralData.UnitData[UnitData[Player.Player].GetHand()].sizex));
			}
			rectangle(L1, U1, L1 + Size1, U1 + Size1);//空心矩形

		}
		
	}

	


	FlushBatchDraw();//将绘图从缓存中绘制到屏幕
}