#pragma once


#include<string>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Game.h"


class Game;

//int UnitAdd(int Hand, float X = 0, float Y = 0);//指针添加单位
//void UnitAdd(float X, float Y, int LifeMax, float SizeX, float SizeY, uint8_t Hand, float M, float F);//自定义添加单位
//
//void UnitDataClear();//清空单位数据

struct SXY {// x y
	float X, Y;
};
struct SXYZ {// x y z
	float X, Y, Z;
};
struct SNMint {// now max
	int Now, Max;
};
struct SNMfloat {// now max
	float Now, Max;
};

class Unit {
public:
	Unit();//
	void Begin(float X, float Y, float Z, float LifeMax, float SizeX, float SizeY, float SizeZ, uint8_t Hand, float M, float F, float WithMaxG);
	void AddGame(Game* g);//添加游戏指针

	bool IfMove(int x, int y, int z);//是否可以移动
	bool BoolMoveX();//是否移动过 X
	bool BoolMoveY();//是否移动过 Y
	bool BoolMoveZ();//是否移动过 Z
	bool BoolMove();//是否移动过

	void SetA(float AX = 0.0f, float AY = 0.0f, float AZ = 0.0f);//设置加速度
	void SetXYZ(float X, float Y, float Z);//设置位置
	void Move();//移动
	void ToF(float FX = 0.0f, float FY = 0.0f, float FZ = 0.0f);//使用牵引力移动 输入方向

	void Jump();//跳跃

	void ToVX();//计算速度
	void ToVY();//计算速度
	void ToVZ();//计算速度
	void ToV(); //计算速度

	void ToG();//重力计算
	void Tof();//阻力
	void ToM();//计算质量
	void ToLife(float X);//生命计算
	void UnitToDel();//删除单位

	void UnitTo();//单位运动计算
	void UnitResetA();//重置加速度

	bool IfOnSpace(float X, float Y, float DX, float DY);//判断是否在区内

	struct SXY GetXY();
	struct SXY GetSize();
	struct SXYZ GetV();
	struct SXYZ GetA();
	struct SNMfloat GetLife();
	int GetHand();

	glm::vec3 Getglmvec3XYZ();
	glm::vec3 Getglmvec3LXLYLZ();




	bool BoolDEL;//是否存在
	Game* G = nullptr;//指向游戏对象的指针 用于访问地图数据等


	uint8_t hand;//相对 CentralData.UnitData
	struct StructF {//牵引力
		float F;
		float u;//线性摩擦因素
	}F;
	struct StructM {//质量
		float now;
		float M;
	}M;
	struct StructXY {//坐标
		float X, Y, Z;
		float LX, LY, LZ;
	}XY;
	struct StructV {//速度
		float X, Y, Z;
	}V;
	struct StructA{//加速度
		float X, Y, Z;
		float WithMaxG;//最大承受的加速度 相对 G
	}A;
	struct StructLife{//生命
		float now;
		float max;
	}Life;
	struct StructSize{//大小
		float X, Y, Z;
	}Size;
	struct StructRotation {//旋转角度
		float X, Y, Z;
	}Rotation;


private:

};




