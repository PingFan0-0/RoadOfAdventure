#include "Time.h"


#include"GameData.h"

void TimeMath() {//<------------------------------------------------------------------时间计算
	Time.LastTime = Time.NowTime;//赋值上次时间
	Time.NowTime = clock();//获取当前时间
	Time.JGTime = Time.NowTime - Time.LastTime;//计算间隔时间
}