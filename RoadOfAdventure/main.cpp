#include<iostream>
#include "begin.h"
#include "Debug.h"
#include "GameData.h"
#include "PageDisplay.h"

int main() {

	Begin();//初始化
	Debug("游戏开始==================");

	while (BoolTheGame) {//main
		ymxs();//页面显示
	}


	Debug("=============游戏正常退出");
}