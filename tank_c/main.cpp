#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "ctrl.h"
#include "data.h"




int g_TankShape[4][3][3] = {
	{//up = 0
		{ 0, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 }
	},
	{//right = 1
		{ 1, 1, 0 },
		{ 0, 1, 1 },
		{ 1, 1, 0 }
	},
	{ //down = 2
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 1, 0 }
	},
	{ //left
		{ 0, 1, 1 },
		{ 1, 1, 0 },
		{ 0, 1, 1 }
	}
};
int main()
{
	initConsoleWindow();
	initInputShift();
	//菜单选项......

	//开始游戏
	playTank();

	return 0;
}

