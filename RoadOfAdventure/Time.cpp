#include "Time.h"


#include"GameData.h"
#include"OpenGL.h"

void TimeMath() {//<------------------------------------------------------------------时间计算

	Time.LastTime = Time.NowTime;//更新上次时间
	Time.NowTime = (float)glfwGetTime();//获取时间
	Time.JGTime = Time.NowTime - Time.LastTime;//计算间隔时间
	Time.FPS++;
	if (Time.FPS >= FPSWeek) {
		FPS = (int)(1 / Time.JGTime);//计算FPS
		Time.FPS = 0;
	}
}