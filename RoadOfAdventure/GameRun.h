#pragma once


#include"GameData.h"
#include"GamePageDisplay.h"
#include"GameLife.h"
#include"Time.h"
#include"GameInput.h"
#include"TypeConversion.h"
#include"Debug.h"


#include"GameUnit.h"




bool gamestart();//<----------------------------------------------------------游戏初始化
void SaveData(int Week);//<---------------------------------------------------游戏自动保存

void GameOnRun();//<----------------------------------------------------------游戏循环