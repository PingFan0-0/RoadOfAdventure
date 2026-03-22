#pragma once


#include<string>
#include<vector>


int UnitAdd(int Hand, float X = 0, float Y = 0);//指针添加单位
void UnitAdd(float X, float Y, int LifeMax, float SizeX, float SizeY, uint8_t Hand, float M, float F);//自定义添加单位

void UnitDataClear();//清空单位数据

struct SXY {// x y
	float X, Y;
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
	void Begin(float X, float Y, float LifeMax, float SizeX, float SizeY, uint8_t Hand, float M, float F);

	void SetA(float AX, float AY);//设置加速度
	void SetXY(float X, float Y);//设置位置
	void Move();//移动
	void ToF(int FX, int FY);//使用牵引力移动 输入方向
	void ToV();//计算速度
	void Tof();//阻力
	void ToM();//计算质量
	void ToLife(float X);//生命计算
	void UnitToDel();//删除单位

	void UnitTo();//单位运动计算

	bool IfOnSpace(float X, float Y, float DX, float DY);//判断是否在区内

	struct SXY GetXY();
	struct SXY GetSize();
	struct SXY GetV();
	struct SXY GetA();
	struct SNMfloat GetLife();
	int GetHand();

	bool BoolDEL;//是否存在
private:
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
		float X, Y;
		float LX, LY;
	}XY;
	struct StructV {//速度
		float X, Y;
	}V;
	struct StructA{//加速度
		float X, Y;
	}A;
	struct StructLife{//生命
		float now;
		float max;
	}Life;
	struct StructSize{//大小
		float X, Y;
	}Size;
};


extern std::vector <Unit> UnitData;//单位数据