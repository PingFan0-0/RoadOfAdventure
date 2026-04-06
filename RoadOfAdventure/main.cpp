#include "begin.h"
#include "Debug.h"
#include "GameData.h"
#include "PageDisplay.h"
#include "OpenGL.h"





int main() {
	std::cout << "RoadOfAdventure(冒险之路)\n\n" << PF << "\n\n";

	Debug("游戏程序启动-------------->");
	if (Begin()) {//初始化
		Debug("游戏开始==================");
		while (BoolTheGame) {//main
			ymxs();//页面显示
		}
		Debug("=============游戏正常退出");
	}
	else {//失败
		Debug("初始化失败");
		Error("=============游戏意外退出", "W");
		return -1;
	}
	GLEnd();//关闭窗口
	Debug("<-------------游戏程序退出");
}
