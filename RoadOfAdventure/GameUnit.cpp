#include "GameUnit.h"

#include"GameData.h"
#include"MapData.h"
#include<math.h>


bool IfTH(float x, float y) {//--------------------------------------------是同号
	return (x > 0 && y > 0) || (x < 0 && y < 0);
}

bool IfWC(float x, float y, float C) {//--------------------------------------------是否相等 误差
	float A = x - y;
	C = (C > 0) ? C : -C;
	if (A > C || A < -C)return false;
	return true;
}

bool Unit::IfMove(int x, int y, int z) {//<-----------------------------------------------------------------------是否可以移动
	//if (x < 0 || x >= Map.maxx)return false;
	//if (y < 0 || y >= Map.maxy)return false;
	BlockID num = G->MapData.GetBlock(int32_t(x),int32_t(y), int32_t(z));//获取地图数据
	//int num = Map.Data[to_MapHand(x, y)].floor;
	if (num == AIR) {//可以通过
		return true;
	}
	return false;
}


// ---------------- 是否移动过 X
bool Unit::BoolMoveX() {
	return !IfWC(XY.X, XY.LX, 0.0001f);
}
// ---------------- 是否移动过 Y
bool Unit::BoolMoveY() {
	return !IfWC(XY.Y, XY.LY, 0.0001f);
}
// ---------------- 是否移动过 Z
bool Unit::BoolMoveZ() {
	return !IfWC(XY.Z, XY.LZ, 0.0001f);
}
// ---------------- 是否移动过
bool Unit::BoolMove() {
	return BoolMoveX() || BoolMoveY() || BoolMoveZ();
}




int Game::UnitAdd(int Hand, float X, float Y, float Z) {
	UnitData.emplace_back();
	UnitData[UnitData.size() - 1].Begin(X, Y, Z, CentralData.UnitData[Hand].Life,//位置 血条 
		CentralData.UnitData[Hand].sizex, CentralData.UnitData[Hand].sizey, CentralData.UnitData[Hand].sizez,//尺寸
		Hand, CentralData.UnitData[Hand].M, CentralData.UnitData[Hand].F, CentralData.UnitData[Hand].WithMaxG);//指针 质量 牵引力 最大承受G
	UnitData[UnitData.size() - 1].AddGame(this);//添加游戏指针
	return (int)(UnitData.size() - 1);
}

void Game::UnitAdd(float X, float Y, float Z, float LifeMax, float SizeX, float SizeY, float SizeZ, uint8_t Hand, float M, float F, float WithMaxG) {
	UnitData.emplace_back();
	UnitData[UnitData.size() - 1].Begin(X, Y, Z, LifeMax, SizeX, SizeY, SizeZ, Hand, M, F, WithMaxG);//位置 血条 尺寸 指针 质量 牵引力 最大承受G
	UnitData[UnitData.size() - 1].AddGame(this);//添加游戏指针
}
//

void Game::UnitDataClear() {//
	while (UnitData.size())UnitData.pop_back();
}






//========================class

Unit::Unit() {};

//Unit::Unit(float X, float Y, int LifeMax, float SizeX, float SizeY, float MA, float MV, uint8_t Hand, float M, float F) {
//	UnitData.emplace_back();
//	UnitData[UnitData.size() - 1].Begin(X, Y, LifeMax, SizeX, SizeY, MA, MV, Hand, M, F);
//};

void Unit::Begin(float X, float Y, float Z, float LifeMax, float SizeX, float SizeY, float SizeZ, uint8_t Hand, float M, float F, float WithMaxG) {
	// ----- 位置 -----
	this->XY.X = X;
	this->XY.Y = Y;
	this->XY.Z = Z;
	this->XY.LX = X;
	this->XY.LY = Y;
	this->XY.LZ = Z;
	// ----- 速度 -----
	this->V.X = 0;
	this->V.Y = 0;
    this->V.Z = 0;
	this->A.X = 0;
	this->A.Y = 0;
	this->A.Z = 0;
	this->A.WithMaxG = WithMaxG;
	// ----- 生命 -----
	this->Life.now = LifeMax;
	this->Life.max = LifeMax;
	// ----- 大小 -----
	this->Size.X = SizeX;
	this->Size.Y = SizeY;
	this->Size.Z = SizeZ;
	// ----- 其他 -----
	this->hand = Hand;
	// ----- 质量 -----
	this->M.M = M;
	this->M.now = M;
	// ----- 牵引力 -----
	this->F.F = F;
    this->F.u = 0.0f;
	// ----- 旋转角度 -----
	this->Rotation.X = 0.0f;
	this->Rotation.Y = 0.0f;
	this->Rotation.Z = 0.0f;
	// ----- 存在 -----
	this->BoolDEL = true;
}

// ----- 添加游戏指针
void Unit::AddGame(Game* g) {
	this->G = g;
}



void Unit::Move() {//移动

	XY.LX = XY.X;//记录上次坐标
	XY.LY = XY.Y;
	XY.LZ = XY.Z;

	const float ENum = 0.0001f;//误差值 用于修正坐标
	float MYXR = Size.X / 2;//X的半径
	float MYYR = Size.Y / 2;//Y的半径
	float MYZR = Size.Z / 2;//Z的半径
	float T = 5;
	//=========== X ==========
	if (V.X != 0) {//当玩家 X轴 移动时
		float NowS = V.X * Time.JGTime;//位移
		float ToX = XY.X + NowS;//目标坐标 x
		if (NowS >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		if (NowS < 0) {//左移动
			
			if (IfMove((int)(ToX - MYXR), (int)(XY.Y - MYYR),        (int)(XY.Z - MYZR))        &&    // 左下后 顶点
				IfMove((int)(ToX - MYXR), (int)(XY.Y - MYYR),        (int)(XY.Z + MYZR - ENum)) &&    // 左下前 顶点
				IfMove((int)(ToX - MYXR), (int)(XY.Y + MYYR - ENum), (int)(XY.Z - MYZR))        &&    // 左上后 顶点
				IfMove((int)(ToX - MYXR), (int)(XY.Y + MYYR - ENum), (int)(XY.Z + MYZR - ENum))       // 左上前 顶点
				) {//判断能否行走
				//XY.LX = XY.X;
				XY.X = ToX;//行走
			}
			else {
				float CVX;//变化的速度
				if (V.X < -0.3f) {//如果接触到墙壁 就反弹
					CVX = V.X + V.X / T;//变化的速度
					V.X = -V.X / T; //反弹
				}
				else {
					CVX = V.X;//变化的速度
					V.X = 0;//停止
				}
				A.X = CVX * CVX / (2 * NowS);//计算加速度
				float G = abs(A.X / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
		else {//右移动
			if (IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y - MYYR),        (int)(XY.Z - MYZR))        &&    // 左下后 顶点
				IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y - MYYR),        (int)(XY.Z + MYZR - ENum)) &&    // 左下前 顶点
				IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y + MYYR - ENum), (int)(XY.Z - MYZR))        &&    // 左上后 顶点
				IfMove((int)(ToX + MYXR - ENum), (int)(XY.Y + MYYR - ENum), (int)(XY.Z + MYZR - ENum))       // 左上前 顶点
				) {//判断能否行走
				//XY.LX = XY.X;
				XY.X = ToX;//行走
			}
			else {
				float CVX;//变化的速度
				if (V.X > 0.3f) {//如果接触到墙壁 就反弹
					CVX = V.X + V.X / T;//变化的速度
					V.X = -V.X / T;//反弹
				}
				else {
					CVX = V.X;//变化的速度
					V.X = 0;//停止
				}
				A.X = CVX * CVX / (2 * NowS);//计算加速度
				float G = abs(A.X / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
	}
	//=========== Y ==========
	if (V.Y != 0) {//当玩家 Z轴 移动时
		float NowS = V.Y * Time.JGTime;//位移
		float ToY = XY.Y + NowS;//目标坐标 z
		if (NowS >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		if (NowS > 0) {//上移动
			if (IfMove((int)(XY.X - MYXR),        (int)(ToY + MYYR - ENum), (int)(XY.Z + MYZR - ENum)) &&  // 左上前 顶点
				IfMove((int)(XY.X - MYXR),        (int)(ToY + MYYR - ENum), (int)(XY.Z - MYZR ))       &&  // 左上后 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(ToY + MYYR - ENum), (int)(XY.Z + MYZR - ENum)) &&  // 右上前 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(ToY + MYYR - ENum), (int)(XY.Z - MYZR))            // 右上后 顶点
				) {//判断能否行走
				//XY.LY = XY.Y;
				XY.Y = ToY;//行走
			}
			else {
				float CVY;//变化的速度
				if (V.Y < -0.3f) {//如果接触到墙壁 就反弹
					CVY = V.Y + V.Y / T;//变化的速度
					V.Y = -V.Y / T;//反弹
				}
				else {
					CVY = V.Y;//变化的速度
					V.Y = 0; //停止
				}
				A.Y -= CVY * CVY / (2 * NowS);//计算加速度
				float G = abs(A.Y / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
		else {//下移动
			if (IfMove((int)(XY.X - MYXR),        (int)(ToY - MYYR), (int)(XY.Z + MYZR - ENum)) &&  // 左下前 顶点
				IfMove((int)(XY.X - MYXR),        (int)(ToY - MYYR), (int)(XY.Z - MYZR))        &&  // 左下后 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(ToY - MYYR), (int)(XY.Z + MYZR - ENum)) &&  // 右下前 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(ToY - MYYR), (int)(XY.Z - MYZR))            // 右下后 顶点
				) { //判断能否行走
				//XY.LY = XY.Y;
				XY.Y = ToY;//行走
			}
			else {
				float CVY;//变化的速度
				if (V.Y > 0.3f) {//如果接触到墙壁 就反弹
					CVY = V.Y + V.Y / T;//变化的速度
					V.Y = -V.Y / T;//反弹
				}
				else {
					CVY = V.Y;//变化的速度
					V.Y = 0;//停止
				}
				A.Y -= CVY * CVY / (2 * NowS);//计算加速度
				float G = abs(A.Y / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
	}
	//=========== Z ==========
	if (V.Z != 0) {//当玩家 Z轴 移动时
		float NowS = V.Z * Time.JGTime;//位移
		float ToZ = XY.Z + NowS;//目标坐标 z
		if (NowS >= 1) {//当移动量大于等于1时 可能会跨格子
			//Null
		}
		if (NowS < 0) {//后移动
			if (IfMove((int)(XY.X - MYXR),        (int)(XY.Y - MYYR),        (int)(ToZ - MYZR)) &&  // 左下后 顶点
				IfMove((int)(XY.X - MYXR),        (int)(XY.Y + MYYR - ENum), (int)(ToZ - MYZR)) &&  // 左上后 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(XY.Y - MYYR),        (int)(ToZ - MYZR)) &&  // 右下后 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(XY.Y + MYYR - ENum), (int)(ToZ - MYZR))     // 右上后 顶点
				) {//判断能否行走
				//XY.LZ = XY.Z;
				XY.Z = ToZ;//行走
			}
			else {
				float CVZ;//变化的速度
				if (V.Z < -0.3f) {//如果接触到墙壁 就反弹
					CVZ = V.Z + V.Z / T;//变化的速度
					V.Z = -V.Z / T;//反弹
				}
				else {
					CVZ = V.Z;//变化的速度
					V.Z = 0;//停止
				}
				A.Z = CVZ * CVZ / (2 * NowS);//计算加速度
				float G = abs(A.Z / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
		else {//前移动
			if (IfMove((int)(XY.X - MYXR),        (int)(XY.Y - MYYR),        (int)(ToZ + MYZR - ENum)) &&  // 左下前 顶点
				IfMove((int)(XY.X - MYXR),        (int)(XY.Y + MYYR - ENum), (int)(ToZ + MYZR - ENum)) &&  // 左上前 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(XY.Y - MYYR),        (int)(ToZ + MYZR - ENum)) &&  // 右下前 顶点
				IfMove((int)(XY.X + MYXR - ENum), (int)(XY.Y + MYYR - ENum), (int)(ToZ + MYZR - ENum))     // 右上前 顶点
				) { //判断能否行走
				//XY.LZ = XY.Z;
				XY.Z = ToZ;//行走
			}
			else {
				float CVZ;//变化的速度
				if (V.Z > 0.3f) {
					CVZ = V.Z + V.Z / T;//变化的速度
					V.Z = -V.Z / T;//反弹
				}
				else {
					CVZ = V.Z;//变化的速度
					V.Z = 0;//停止
				}
				A.Z = CVZ * CVZ / (2 * NowS);//计算加速度
				float G = abs(A.Z / this->G->MapData.g);//承受的加速度 相对 G
				if (G > A.WithMaxG) {//如果超过最大承受加速度
					ToLife(A.WithMaxG - G);//计算生命
				}
			}
		}
	}
}

void Unit::SetA(float AX, float AY, float AZ) {//设置加速度
	A.X = AX;
	A.Y = AY;
	A.Z = AZ;
}

void Unit::SetXYZ(float X, float Y ,float Z) {//设置位置速度
	XY.X = X;
	XY.Y = Y;
	XY.Z = Z;
}

void Unit::ToF(float FX, float FY, float FZ) {//使用牵引力移动 输入方向
	A.X += FX * F.F / M.now;
	A.Y += FY * F.F / M.now;
	A.Z += FZ * F.F / M.now;
}

void Unit::Jump() {//跳跃const float ENum = 0.0001f;//误差值 用于修正坐标
	if (BoolDEL) {
		const float ENum = 0.0001f;//误差值 用于修正坐标
		float MYXR = Size.X / 2;//X的半径
		float MYYR = Size.Y / 2;//Y的半径
		float MYZR = Size.Z / 2;//Z的半径
		float ToY = XY.Y - 0.01f;//目标坐标 z
		if (!IfMove((int)(XY.X - MYXR), (int)(ToY - MYYR), (int)(XY.Z + MYZR - ENum)) ||  // 左下前 顶点
			!IfMove((int)(XY.X - MYXR), (int)(ToY - MYYR), (int)(XY.Z - MYZR)) ||  // 左下后 顶点
			!IfMove((int)(XY.X + MYXR - ENum), (int)(ToY - MYYR), (int)(XY.Z + MYZR - ENum)) ||  // 右下前 顶点
			!IfMove((int)(XY.X + MYXR - ENum), (int)(ToY - MYYR), (int)(XY.Z - MYZR))            // 右下后 顶点
			) { //判断能否行走
			ToF(0.0f, 50.0f);
		}
	}
}

//
void Unit::ToVX() {//计算速度
	V.X += A.X * Time.JGTime;
}
void Unit::ToVY() {//计算速度
	V.Y += A.Y * Time.JGTime;
}
void Unit::ToVZ() {//计算速度
	V.Z += A.Z * Time.JGTime;
}
void Unit::ToV() {//计算速度
	ToVX();
	ToVY();
	ToVZ();
}


//
void Unit::ToG() {//重力计算
	A.Y -= G->MapData.g;
}

void Unit::Tof() {//阻力
	F.u = 50;
	A.X -= F.u * V.X / M.now;
	A.Y -= 0.5f * 1 * 1 * (Size.X * Size.Z) * V.Y * fabs(V.Y) / M.now ;//空气阻力 (0.5 * 空气密度 * 迎风面积 * 速度平方 / 质量)
	A.Z -= F.u * V.Z / M.now;
}

void Unit::ToM() {//质量
	M.now = M.M;
}

void Unit::ToLife(float X) {//生命计算
	Life.now += X;
	if (Life.now > Life.max)Life.now = Life.max;//如果生命超过最大值 就等于最大值
	else if (Life.now <= 0) {//如果生命小于等于0 就删除单位
		Life.now = 0;
		UnitToDel();//摧毁
	}
}

void Unit::UnitToDel() {//删除单位
	BoolDEL = false;
}
//

void Unit::UnitTo() {//单位运动计算
	if (BoolDEL) {
		ToM(); //质量计算
		ToG(); //重力计算
		Tof(); //阻力计算
		ToV(); //速度计算
		Move();//移动计算
	}
}

void Unit::UnitResetA() {//重置加速度
	SetA(0.0f, 0.0f, 0.0f);
}
//

bool Unit::IfOnSpace(float X, float Y, float DX, float DY) {//判断是否在区内
	return !(X > XY.X || DX < XY.X) || (Y > XY.Y || DY < XY.Y);
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

struct SXYZ Unit::GetV() {
	SXYZ a; a.X = V.X; a.Y = V.Y; a.Z = V.Z;
	return a;
}

struct SXYZ Unit::GetA() {
	SXYZ a; a.X = A.X; a.Y = A.Y; a.Z = A.Z;
	return a;
}

struct SNMfloat Unit::GetLife() {
	SNMfloat a; a.Max = Life.max; a.Now = Life.now;
	return a;
}

int Unit::GetHand() {
	return hand;
}

glm::vec3 Unit::Getglmvec3XYZ() {
	return glm::vec3(XY.X, XY.Y, XY.Z);//<-------
}
glm::vec3 Unit::Getglmvec3LXLYLZ() {
	return glm::vec3(XY.LX, XY.LY, XY.LZ);//<-------
}





// 添加一个面的顶点和索引
void addFace(std::vector<VertexTo>& vertices, std::vector<GLuint>& indices,
	const glm::vec3& pos, const glm::vec3& normal,
	const glm::vec2& uvMin, const glm::vec2& uvMax, float& X, float& Y, uint32_t& hand) {// 根据法线方向确定四个顶点的位置
	// 根据法线方向确定四个顶点的位置
	float x = pos.x, y = pos.y, z = pos.z;// 方块尺寸为1单位
	glm::vec3 v0, v1, v2, v3;// 四个顶点位置
	glm::vec2 uv0, uv1, uv2, uv3;// 四个顶点的纹理坐标
	uint8_t face = 0;
	if (normal.x == 1) { // 右面 (+x)
		v0 = { x + X, y,   z };   v1 = { x + X, y,   z + X };// 右面顶点位置
		v2 = { x + X, y + Y, z + X }; v3 = { x + X, y + Y, z };// 右面顶点位置
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };// 右面纹理坐标
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };// 右面纹理坐标
		face = 0;
	}
	else if (normal.x == -1) { // 左面 (-x)
		v0 = { x, y,   z + X }; v1 = { x, y,   z };
		v2 = { x, y + Y, z }; v3 = { x, y + Y, z + X };
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
		face = 1;
	}
	else if (normal.y == 1) { // 上面 (+y)
		v0 = { x,   y + Y, z };   v1 = { x + X, y + Y, z };
		v2 = { x + X, y + Y, z + X }; v3 = { x,   y + Y, z + X };
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
		face = 2;
	}
	else if (normal.y == -1) { // 下面 (-y)
		v0 = { x,   y, z + X }; v1 = { x + X, y, z + X };
		v2 = { x + X, y, z };   v3 = { x,   y, z };
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
		face = 3;
	}
	else if (normal.z == 1) { // 前面 (+z)
		v0 = { x + X, y,   z + X }; v1 = { x,   y,   z + X };
		v2 = { x,   y + Y, z + X }; v3 = { x + X, y + Y, z + X };
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
		face = 4;
	}
	else if (normal.z == -1) { // 后面 (-z)
		v0 = { x,   y,   z }; v1 = { x + X, y,   z };
		v2 = { x + X, y + Y, z }; v3 = { x,   y + Y, z };
		uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
		uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
		face = 5;
	}
	else {
		return;
	}
	GLuint base = (hand * 6 + face) * 4;
	//GLuint base = (GLint)vertices.size();//    当前顶点的起始索引
	if (base >= (GLint)vertices.size()) {// 如果当前顶点数量不足以容纳新的面，则添加
		vertices.push_back({ v0, uv0, normal });// 添加四个顶点数据
		vertices.push_back({ v1, uv1, normal });// 添加四个顶点数据
		vertices.push_back({ v2, uv2, normal });// 添加四个顶点数据
		vertices.push_back({ v3, uv3, normal });// 添加四个顶点数据
	}
	else {// 否则直接覆盖现有顶点数据
		vertices[base]     = { v0, uv0, normal };// 覆盖四个顶点数据
		vertices[base + 1] = { v1, uv1, normal };// 覆盖四个顶点数据
		vertices[base + 2] = { v2, uv2, normal };// 覆盖四个顶点数据
		vertices[base + 3] = { v3, uv3, normal };// 覆盖四个顶点数据
	}

	GLuint base2 = (hand * 6 + face) * 6;
	if (base2 >= (GLint)indices.size()) {// 如果当前索引数量不足以容纳新的面，则添加
		indices.push_back(base);    // 添加两个三角形的索引数据
		indices.push_back(base + 1);// 添加两个三角形的索引数据
		indices.push_back(base + 2);// 添加两个三角形的索引数据
		indices.push_back(base);    // 添加两个三角形的索引数据
		indices.push_back(base + 2);// 添加两个三角形的索引数据
		indices.push_back(base + 3);// 添加两个三角形的索引数据
	}
	else {// 否则直接覆盖现有索引数据
		indices[base2]     = base;    // 覆盖两个三角形的索引数据
		indices[base2 + 1] = base + 1;// 覆盖两个三角形的索引数据
		indices[base2 + 2] = base + 2;// 覆盖两个三角形的索引数据
		indices[base2 + 3] = base;    // 覆盖两个三角形的索引数据
		indices[base2 + 4] = base + 2;// 覆盖两个三角形的索引数据
		indices[base2 + 5] = base + 3;// 覆盖两个三角形的索引数据
	}
}




void Game::UnitRender() {
	glFrontFace(GL_CW);// 设置顺时针为正面
	static bool BoolOnce = true;
	for (uint32_t i = 0; i < UnitData.size(); i++) {
		if (BoolOnce || (UnitData[i].BoolDEL && UnitData[i].BoolMove())) {
			glm::vec2 uvMax = regions[CentralData.UnitData[UnitData[i].hand].ImageHand].uvMin;// 获取纹理坐标
			glm::vec2 uvMin = regions[CentralData.UnitData[UnitData[i].hand].ImageHand].uvMax;// 获取纹理坐标
			auto Size = UnitData[i].Size;
			auto Pos = UnitData[i].XY;
			glm::vec3 worldPos = glm::vec3(Pos.X - Size.X / 2, Pos.Y - Size.Y / 2, Pos.Z - Size.Z / 2);// 计算世界坐标（用于生成顶点位置）
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(1, 0, 0),  uvMin, uvMax, Size.X, Size.Y, i);// 右 (x+1)
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(-1, 0, 0), uvMin, uvMax, Size.X, Size.Y, i);// 左 (x-1)
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(0, 1, 0),  uvMin, uvMax, Size.X, Size.Y, i);// 上 (y+1)
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(0, -1, 0), uvMin, uvMax, Size.X, Size.Y, i);// 下 (y-1)
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(0, 0, 1),  uvMin, uvMax, Size.X, Size.Y, i);// 前 (z+1)
			addFace(unit_vertices, unit_indices, worldPos, glm::vec3(0, 0, -1), uvMin, uvMax, Size.X, Size.Y, i);// 后 (z-1)
		}
	}
	BoolOnce = false;
	// 存储顶点和索引数据到成员变量，用于上传
	unit_indexCount = (GLsizei)unit_indices.size();// 存储索引数量

	//========= 上传数据到 GPU

	if (unit_vertices.empty() || unit_indices.empty()) return;

	glGenVertexArrays(1, &unit_vao);// 生成顶点数组对象
	glGenBuffers(1, &unit_vbo);// 生成顶点缓冲对象
	glGenBuffers(1, &unit_ebo);// 生成索引缓冲对象

	glBindVertexArray(unit_vao);// 绑定顶点数组对象
	glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);// 绑定顶点缓冲对象
	glBufferData(GL_ARRAY_BUFFER, unit_vertices.size() * sizeof(VertexTo), unit_vertices.data(), GL_STATIC_DRAW);// 上传顶点数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unit_ebo);// 绑定索引缓冲对象
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, unit_indices.size() * sizeof(GLuint), unit_indices.data(), GL_STATIC_DRAW);// 上传索引数据

	// 位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, pos));// 定义位置属性指针
	glEnableVertexAttribArray(0);// 启用位置属性
	// 纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, uv));
	glEnableVertexAttribArray(1);
	// 法线
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, normal));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);// 解绑顶点数组对象
	// 清空CPU侧数据，释放内存
	//unit_vertices.clear();// 清空顶点数据，释放内存
	//unit_indices.clear();// 清空索引数据，释放内存


	// ===== 绘制 
	if (unit_indexCount == 0) return;
	glBindVertexArray(unit_vao);// 绑定顶点数组对象
	glDrawElements(GL_TRIANGLES, unit_indexCount, GL_UNSIGNED_INT, 0);// 绘制单位
	glBindVertexArray(0);// 解绑顶点数组对象

	// ===== 清理资源
	if (unit_vao) glDeleteVertexArrays(1, &unit_vao);// 析构函数，删除 OpenGL 相关资源
	if (unit_vbo) glDeleteBuffers(1, &unit_vbo);// 删除顶点缓冲对象
	if (unit_ebo) glDeleteBuffers(1, &unit_ebo);// 删除索引缓冲对象
	//unit_indexCount = 0;
}