#include "PlayerInput.h"


bool MousePlace(int MX, int MY, int X, int Y, int DX, int DY) {//<---------------------------------------------------------鼠标是否在位置
	if (MX >= X && MX <= DX) {
		if (MY >= Y && MY <= DY) return true;
		else return false;
	}
	else return false;
}