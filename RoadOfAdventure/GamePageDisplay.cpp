#include "GamePageDisplay.h"


#include"GameData.h"
#include"TypeConversion.h"
#include"Debug.h"
#include"DrawShape.h"


#include"Camera.h"

#include"GameUnit.h"


extern UIClickableTextManager& uiManager;

// 参数：左上角 (x,y)，宽 w 高 h，颜色 color，透明度 alpha (0~255)
//void drawSemiRect(int x, int y, int w, int h, COLORREF color, int alpha){//透明矩形
//	IMAGE img(w, h);
//	SetWorkingImage(&img);
//	setfillcolor(color);
//	solidrectangle(0, 0, w, h);
//
//	DWORD* pImg = GetImageBuffer(&img);
//	DWORD* pWin = GetImageBuffer();
//	int winW = getwidth();
//
//	for (int i = 0; i < h; i++)
//		for (int j = 0; j < w; j++)
//		{
//			int idxImg = i * w + j;
//			int idxWin = (y + i) * winW + (x + j);
//			COLORREF fg = pImg[idxImg];
//			COLORREF bg = pWin[idxWin];
//
//			int R = (GetRValue(fg) * alpha + GetRValue(bg) * (255 - alpha)) / 255;
//			int G = (GetGValue(fg) * alpha + GetGValue(bg) * (255 - alpha)) / 255;
//			int B = (GetBValue(fg) * alpha + GetBValue(bg) * (255 - alpha)) / 255;
//			pWin[idxWin] = RGB(R, G, B);
//		}
//
//	SetWorkingImage(NULL);
//}


float cameraSpeed = 3.0f;


void ImageDisplay(int x, int y, int size, int img, SpriteRenderer& spriteRenderer) {//<-------------------------------------------------------------图片显示
	spriteRenderer.DrawSprite(atlas, regions[img], glm::vec2(x, y), glm::vec2(size, size));
}
void ImageDisplay(int x, int y, int size, SXY Size, int img, SpriteRenderer& spriteRenderer) {//<-------------------------------------------------------------图片显示
	spriteRenderer.DrawSprite(atlas, regions[img], glm::vec2((x - Size.X * size / 2), (y - Size.Y * size / 2)), glm::vec2((Size.X * size), (Size.Y * size)));
}



void pmsx(TextRenderer& textRenderer, Shader& SH, SpriteRenderer& spriteRenderer) {//<---------------------------------------------------------------屏幕刷新 

	/*

	const int Hight = 32;//高度
#pragma region 旧的
	//if (Win.LastSize != Win.Size) {//当缩放大小改变时
	//	Win.LastSize = Win.Size;//记录缩放大小
	//	for (int i = 0; i < CentralData.BackData.size(); i++) {//加载图片 BackData
	//		loadimage(&CentralData.ImageData[CentralData.BackData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.BackData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
	//	}
	//	for (int i = 0; i < CentralData.BuildingData.size(); i++) {//加载图片 BuildingData
	//		loadimage(&CentralData.ImageData[CentralData.BuildingData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.BuildingData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
	//	}
	//	for (int i = 0; i < CentralData.UnitData.size(); i++) {//加载图片 UnitData
	//		loadimage(&CentralData.ImageData[CentralData.UnitData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[i].ImageHand].imageway).c_str(), (int)(Win.Size * CentralData.UnitData[i].sizex), (int)(Win.Size * CentralData.UnitData[i].sizex));
	//	}
	//	for (int i = 0; i < CentralData.WallData.size(); i++) {//加载图片 WallData
	//		loadimage(&CentralData.ImageData[CentralData.WallData[i].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.WallData[i].ImageHand].imageway).c_str(), Win.Size, Win.Size);
	//	}
	//	//加载图片 IMAGENULL
	//	loadimage(&CentralData.ImageData[0].image, to_wstring(CentralData.ImageData[0].imageway).c_str(), Win.Size, Win.Size);
	//}

	//cleardevice();// 清屏
	//BeginBatchDraw();//将绘图保存在缓存中

	//int BY = Player.CameraY - (Win.WinY / 2 / Win.Size + 2) >= 0 ? (int)(Player.CameraY - (Win.WinY / 2 / Win.Size + 2)) : 0;//BeginY
	//int EY = Player.CameraY + (Win.WinY / 2 / Win.Size + 2) < Map.maxy ? (int)(Player.CameraY + (Win.WinY / 2 / Win.Size + 2)) : Map.maxy;//EndY
	//int BX = Player.CameraX - (Win.WinX / 2 / Win.Size + 2) >= 0 ? (int)(Player.CameraX - (Win.WinX / 2 / Win.Size + 2)) : 0;//BeginX
	//int EX = Player.CameraX + (Win.WinX / 2 / Win.Size + 2) < Map.maxx ? (int)(Player.CameraX + (Win.WinX / 2 / Win.Size + 2)) : Map.maxx;//EndX
	//int GY = (int)(Win.WinY / 2 - Player.CameraY * Win.Size);
	//int GX = (int)(Win.WinX / 2 - Player.CameraX * Win.Size);

	//for (int i = BY; i < EY; i++) {//地图显示 
	//	for (int j = BX; j < EX; j++) {
	//		int floor = Map.Data[to_MapHand(j, i)].floor;
	//		if(floor == 0)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.ImageData[CentralData.BackData[CentralData.Data[floor].Hand].ImageHand].image);//地板
	//		else ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.ImageData[CentralData.WallData[CentralData.Data[floor].Hand].ImageHand].image);//墙
	//	}
	//}
	//
	//for (int i = 0; i < UnitData.size(); i++) {//单位显示 
	//	SXY a = UnitData[i].GetXY();
	//	if (UnitData[i].BoolDEL && UnitData[i].IfOnSpace((float)BX, (float)BY, (float)EX, (float)EY)) ImageDisplay((int)(a.X * Win.Size + GX), (int)(a.Y * Win.Size + GY), Win.Size, UnitData[i].GetSize(), CentralData.ImageData[CentralData.UnitData[UnitData[i].GetHand()].ImageHand].image);//绘制单位
	//}


	//int ULY = 0;//左上 ---------------------------
	//{
	//	if (BoolFPS) {//=====FPS显示 
	//		settextstyle(15, 0, _T("宋体"));//打印FPS
	//		settextcolor(RGB(255, 255, 255));//设置文字颜色
	//		outtextxy(0, 15 * ULY, (L"FPS:" + std::to_wstring(FPS) + L" " + std::to_wstring(Time.JGTime) + L"ms Size:" + std::to_wstring(Win.Size)).c_str());
	//		ULY++;
	//	}
	//	if (BoolZbxs) {
	//		settextstyle(15, 0, _T("宋体"));//打印坐标
	//		settextcolor(RGB(255, 255, 255));//设置文字颜色
	//		SXY a = UnitData[Player.Player].GetXY();//XY
	//		outtextxy(0, 15 * ULY, (L"" + std::to_wstring(a.X) + L" " + std::to_wstring(a.Y)).c_str());
	//		ULY++;
	//	}
	//	if (BoolVxs) {
	//		settextstyle(15, 0, _T("宋体"));//打印坐标
	//		settextcolor(RGB(255, 255, 255));//设置文字颜色
	//		SXY a = UnitData[Player.Player].GetV();//V
	//		outtextxy(0, 15 * ULY, (L"" + std::to_wstring(a.X) + L" " + std::to_wstring(a.Y)).c_str());
	//		ULY++;
	//	}
	//}

	//int DLY = 0;//左下 ---------------------------
	//{
	//	int L = Win.WinX / 50;
	//	int R = Win.WinX / 5;
	//	int U = Win.WinY - Win.WinY / 5;
	//	int D = Win.WinY - Win.WinY / 50;
	//	int X = R - L;
	//	int Y = D - U;

	//	setfillcolor(RGB(32, 32, 32) | (16 << 24));
	//	fillrectangle(L, U, R, D);
	//	// 
	//	//drawSemiRect(L, U, X, Y, RGB(255, 255, 255), 128);//实心矩形
	//	setlinecolor(RED);//线段颜色
	//	rectangle(L, U, R, D);//空心矩形
	//	{
	//		int L1 = L + X / 15;
	//		int R1 = R - X / 15;
	//		int U1 = U + Y / 8;
	//		int D1 = U + Y / 4;
	//		int X1 = R1 - L1;
	//		int Y1 = D1 - U1;
	//		//血条
	//		setfillcolor(GREEN);//线段颜色
	//		SNMfloat A = UnitData[Player.Player].GetLife();
	//		solidrectangle(L1, U1, L1 + (int)(X1 * A.Now / A.Max), D1);//实心矩形
	//		rectangle(L1, U1, R1, D1);//空心矩形
	//		//玩家贴图
	//		int Size1 = (int)(Y / 5);
	//		U1 = D1 + Y / 8;
	//		if (UnitData[Player.Player].BoolDEL) {
	//			loadimage(&CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].imageway).c_str(), Size1, Size1);
	//			ImageDisplay(L1, U1, Size1, CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image);
	//			loadimage(&CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].image, to_wstring(CentralData.ImageData[CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand].imageway).c_str(), (int)(Win.Size * CentralData.UnitData[UnitData[Player.Player].GetHand()].sizex), (int)(Win.Size * CentralData.UnitData[UnitData[Player.Player].GetHand()].sizex));
	//		}
	//		rectangle(L1, U1, L1 + Size1, U1 + Size1);//空心矩形

	//	}
	//	
	//}

	


	//FlushBatchDraw();//将绘图从缓存中绘制到屏幕

#pragma endregion
	glClearColor(0.15f, 0.2f, 0.2f, 0);//设置背景颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓冲区

#pragma region 左上 =================
	{
		GLfloat ULY = 0;
		GLfloat h = 5.0f;
		float size = 0.4f;
		if (BoolFPS) {//=====FPS显示
			textRenderer.RenderText(
				"FPS: " + std::to_string(FPS) + " " + std::to_string(Time.JGTime) + "ms Size:" + std::to_string(Win.Size)
				, 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
			ULY++;
		}
		if (BoolZbxs) {
			textRenderer.RenderText(//相机XY
				"POS: " + std::to_string((int)cameraPos.x) + " " + std::to_string((int)cameraPos.y)
				, 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
			ULY++;
		}
		if (BoolVxs) {
			SXY a = UnitData[Player.Player].GetV();//V
			textRenderer.RenderText(
				"V:   " + std::to_string(a.X) + " " + std::to_string(a.Y)
				, 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
			ULY++;
		}
	}
#pragma endregion
#pragma region 左下 =================
	{
		GLfloat DLY = 0.0f;
		float size = 0.4f;
		int L = Win.WinX / 50;
		int R = Win.WinX / 5;
		int U = Win.WinY - Win.WinY / 5;
		int D = Win.WinY - Win.WinY / 50;
		int X = R - L;
		int Y = D - U;

		SNMfloat A = UnitData[Player.Player].GetLife();//获取生命
		
		{
			int L1 = L + X / 15;
			int R1 = R - X / 15;
			int U1 = U + Y / 8;
			int D1 = U + Y / 4;
			int X1 = R1 - L1;
			int Y1 = D1 - U1;
			//血条
			SNMfloat A = UnitData[Player.Player].GetLife();
			DrawRectShape::GetInstance().DrawRect(L1, U1, L1 + (int)(X1 * A.Now / A.Max), D1,
				glm::vec4(0.0f, 0.8f, 0.0f, 0.5f), true);//实心矩形.
			DrawRectShape::GetInstance().DrawRect(L1, U1, R1, D1,
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//空心矩形
			//玩家贴图
			int Size1 = (int)(Y / 5);
			U1 = D1 + Y / 8;
			if (UnitData[Player.Player].BoolDEL) {
				ImageDisplay(L1, U1, Size1, CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand, spriteRenderer);
			}
			DrawRectShape::GetInstance().DrawRect(L1, U1, L1 + Size1, U1 + Size1,
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//空心矩形
		}
		DrawRectShape::GetInstance().DrawRect(L, U, R, D,
			glm::vec4(0.125f, 0.125f, 0.125f, 0.5f),true);//背景
		DrawRectShape::GetInstance().DrawRect(L, U, R, D,
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//边框
	}
#pragma endregion
#pragma region 地图显示 =============
	int BY = cameraPos.y - (Win.WinY / 2 / Win.Size + 2) >= 0 ? (int)(cameraPos.y - (Win.WinY / 2 / Win.Size + 2)) : 0;//BeginY
	int EY = cameraPos.y + (Win.WinY / 2 / Win.Size + 2) < Map.maxy ? (int)(cameraPos.y + (Win.WinY / 2 / Win.Size + 2)) : Map.maxy;//EndY
	int BX = cameraPos.x - (Win.WinX / 2 / Win.Size + 2) >= 0 ? (int)(cameraPos.x - (Win.WinX / 2 / Win.Size + 2)) : 0;//BeginX
	int EX = cameraPos.x + (Win.WinX / 2 / Win.Size + 2) < Map.maxx ? (int)(cameraPos.x + (Win.WinX / 2 / Win.Size + 2)) : Map.maxx;//EndX
	int GY = (int)(Win.WinY / 2 - cameraPos.y * Win.Size);
	int GX = (int)(Win.WinX / 2 - cameraPos.x * Win.Size);
	
	for (int i = 0; i < UnitData.size(); i++) {//单位显示 
		SXY a = UnitData[i].GetXY();
		if (UnitData[i].BoolDEL && UnitData[i].IfOnSpace((float)BX - 1, (float)BY - 1, (float)EX + 1, (float)EY + 1)) {
			ImageDisplay((int)(a.X * Win.Size + GX), (int)(a.Y * Win.Size + GY), Win.Size, UnitData[i].GetSize(), CentralData.UnitData[UnitData[i].GetHand()].ImageHand, spriteRenderer);//绘制单位
		}
	}

	for (int i = BY; i < EY; i++) {//地图显示 
		for (int j = BX; j < EX; j++) {
			int floor = Map.Data[to_MapHand(j, i)].floor;
			if(floor == 0)ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.BackData[CentralData.Data[floor].Hand].ImageHand, spriteRenderer);//地板
			else ImageDisplay(j * Win.Size + GX, i * Win.Size + GY, Win.Size, CentralData.BlockData[CentralData.Data[floor].Hand].ImageHand, spriteRenderer);//墙
		}
	}


#pragma endregion


	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//视图矩阵



	//透视
	SH.setMat4("model", model);
	SH.setMat4("view", view);
	SH.setMat4("projection", projection);
	
	//spriteRenderer.DrawSprite(atlas, regions[0], glm::vec2(100, 400), glm::vec2(100));

	//textRenderer.RenderText("冒险之路", 100.0f, 50.0f, 1.0f, glm::vec3(1.0f, 0.5f, 0.2f));
	//textRenderer.RenderText("PF", 600.0f, 500.0f, sin(Time.NowTime * 2), glm::vec3(0.3f, 0.7f, 0.9f));
	uiManager.Render();//渲染按钮
	
	
	//数据处理
	glfwSwapBuffers(window);//交换缓冲区
	glfwPollEvents();//获取事件



	*/
}