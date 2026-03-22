#include "GameUnit.h"

#include"GameData.h"



std::vector <Unit> UnitData;//单位数据



bool IfMove(int x, int y) {//<-----------------------------------------------------------------------是否可以移动
	if (x < 0 || x >= Map.maxx)return false;
	if (y < 0 || y >= Map.maxy)return false;
	int num = Map.Data[to_MapHand(x, y)].floor;//获取地图数据 
	if (num == 0 || num == 2 || num == 9 || num == 11) {//可以通过
		return true;
	}
	return false;
}

bool IfTH(float x, float y) {//--------------------------------------------是同号
	if (x > 0) {
		if (y > 0)return true;
		return false;
	}
	if (y > 0)return false;
	return true;
}

bool IfWC(float x, float y, float C) {//--------------------------------------------是否相等 误差
	float A = x - y;
	C = (C > 0) ? C : -C;
	if (A > C || A < -C)return false;
	return true;
}


int UnitAdd(int Hand, float X, float Y) {
	UnitData.emplace_back();
	UnitData[UnitData.size() - 1].Begin(X, Y, CentralData.UnitData[Hand].Life,//位置 血条 
		CentralData.UnitData[Hand].sizex, CentralData.UnitData[Hand].sizey,//尺寸
		Hand, CentralData.UnitData[Hand].M, CentralData.UnitData[Hand].F);//指针 质量 牵引力
	return (int)(UnitData.size() - 1);
}

void UnitAdd(float X, float Y, float LifeMax, float SizeX, float SizeY, uint8_t Hand, float M, float F) {
	UnitData.emplace_back();
	UnitData[UnitData.size() - 1].Begin(X, Y, LifeMax, SizeX, SizeY, Hand, M, F);
}
//

void UnitDataClear() {//
	while (UnitData.size())UnitData.pop_back();
}






//========================class

Unit::Unit() {};

//Unit::Unit(float X, float Y, int LifeMax, float SizeX, float SizeY, float MA, float MV, uint8_t Hand, float M, float F) {
//	UnitData.emplace_back();
//	UnitData[UnitData.size() - 1].Begin(X, Y, LifeMax, SizeX, SizeY, MA, MV, Hand, M, F);
//};

void Unit::Begin(float X, float Y, float LifeMax, float SizeX, float SizeY, uint8_t Hand, float M, float F) {
	this->XY.X = X;
	this->XY.Y = Y;
	this->V.X = 0;
	this->V.Y = 0;
	this->A.X = 0;
	this->A.Y = 0;
	this->Life.now = LifeMax;
	this->Life.max = LifeMax;
	this->Size.X = SizeX;
	this->Size.Y = SizeY;
	this->hand = Hand;
	this->M.M = M;
	this->F.F = F;
	this->BoolDEL = true;
}

void Unit::Move() {//移动
	const float ENum = 0.0001f;//误差值 用于修正坐标
	float MYXR = Size.X / 2;//X的半径
	float MYYR = Size.Y / 2;//X的半径
	float T = 5;
	//=========== X ==========
	if (V.X != 0) {//当玩家 X轴 移动时
		float NowS = V.X * Time.JGTime / 1000;//位移
		float ToX = XY.X + NowS;//目标坐标 x
		if (NowS >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		if (NowS < 0) {//左移动
			if (IfMove((int)(ToX - MYXR), (int)(XY.Y - MYYR)) && IfMove((int)(ToX - MYXR), (int)(XY.Y + MYYR - ENum))) {//判断能否行走
				XY.LX = XY.X;
				XY.X = ToX;//行走
			}
			else {
				if (!IfWC(XY.LX, XY.X, ENum))ToLife(V.X);
				if (V.X < -0.3f)V.X = -V.X / T; else V.X = 0;
			}
		}
		else {//右移动
			if (IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y - MYYR)) && IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y + MYYR - ENum))) {//判断能否行走
				XY.LX = XY.X;
				XY.X = ToX;//行走
			}
			else {
				if (!IfWC(XY.LX, XY.X, ENum))ToLife(-V.X);
				if (V.X > 0.3f)V.X = -V.X / T; else V.X = 0;
			}
		}
	}
	//=========== Y ==========
	if (V.Y != 0) {//当玩家 X轴 移动时
		float NowS = V.Y * Time.JGTime / 1000;//位移
		float ToY = XY.Y + NowS;//目标坐标 y
		if (NowS >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		if (NowS < 0) {//上移动
			if (IfMove((int)(XY.X - MYXR), (int)(ToY - MYYR)) && IfMove((int)(XY.X + MYXR - ENum), (int)(ToY - MYYR))) {//判断能否行走
				XY.LY = XY.Y;
				XY.Y = ToY;//行走
			}
			else {
				if (!IfWC(XY.LY, XY.Y, ENum))ToLife(V.Y);
				if (V.Y < -0.3f)V.Y = -V.Y / T; else V.Y = 0;
			}
		}
		else {//下移动
			if (IfMove((int)(XY.X - MYXR), (int)(ToY + MYYR - ENum)) && IfMove((int)(XY.X + MYXR - ENum), (int)(ToY + MYYR - ENum))) {//判断能否行走
				XY.LY = XY.Y;
				XY.Y = ToY;//行走
			}
			else {
				if (!IfWC(XY.LY, XY.Y, ENum))ToLife(-V.Y);
				if (V.Y > 0.3f)V.Y = -V.Y / T; else V.Y = 0;
			}
		}
	}
}

void Unit::SetA(float AX, float AY) {//设置加速度
	A.X = AX;
	A.Y = AY;
}

void Unit::SetXY(float X, float Y) {//设置加速度
	XY.X = X;
	XY.Y = Y;
}

void Unit::ToF(int FX, int FY) {//使用牵引力移动 输入方向
	A.X += FX * F.F / M.now;
	A.Y += FY * F.F / M.now;
}

void Unit::ToV() {//计算速度
	V.X += A.X * Time.JGTime / 1000;
	V.Y += A.Y * Time.JGTime / 1000;
}

void Unit::Tof() {//阻力
	F.u = 50;
	A.X -= F.u * V.X / M.now;
	A.Y -= F.u * V.Y / M.now;
}

void Unit::ToM() {//阻力
	M.now = M.M;
}

void Unit::ToLife(float X) {//生命计算
	Life.now += X;
	if (Life.now > Life.max)Life.now = Life.max;
	else if (Life.now <= 0) {
		Life.now = 0;
		UnitToDel();//摧毁
	}
}

void Unit::UnitToDel() {//删除单位
	BoolDEL = false;
}
//

void Unit::UnitTo() {//单位运动计算
	Tof();//阻力计算
	ToV();//速度计算
	Move();//移动计算
	ToM();//质量计算
	SetA(0, 0);//重置加速度
}
//

bool Unit::IfOnSpace(float X, float Y, float DX, float DY) {//判断是否在区内
	if (X > XY.X || DX < XY.X) return false;
	if (Y > XY.Y || DY < XY.Y) return false;
	return true;
}
//

struct SXY Unit::GetXY() {
	SXY a; a.X = XY.X; a.Y = XY.Y;
	return a;
}

struct SXY Unit::GetSize() {
	SXY a; a.X = Size.X; a.Y = Size.Y;
	return a;
}

struct SXY Unit::GetV() {
	SXY a; a.X = V.X; a.Y = V.Y;
	return a;
}

struct SXY Unit::GetA() {
	SXY a; a.X = A.X; a.Y = A.Y;
	return a;
}

struct SNMfloat Unit::GetLife() {
	SNMfloat a; a.Max = Life.max; a.Now = Life.now;
	return a;
}

int Unit::GetHand() {
	return hand;
}