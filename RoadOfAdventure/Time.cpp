#include "Time.h"


#include"GameData.h"


void TimeMath() {//<------------------------------------------------------------------时间计算

	

	Time.FPS++;
	Time.NowTime = clock();//获取当前时间
	if (Time.LastTime + 1000 <= Time.NowTime) {
		Time.LastTime = Time.NowTime;//赋值上次时间
		FPS = Time.FPS * 1;
		Time.JGTime = 1000.0f / FPS;//计算间隔时间
		Time.FPS = 0;
	}
}