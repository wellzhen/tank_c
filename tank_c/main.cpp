#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "ctrl.h"
#include "data.h"
#include <mmsyscom.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "WINMM.LIB")

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
	/************  显示欢迎界面***************/
	showWelcomeWall();
	showWelcomeWords();
	Sleep(1000);

	while (1) {
		/*菜单 选项*/
		system("cls");
		showWelcomeWall();
		showMenu();
		int optionNum = getMenuChoice();

		switch (optionNum) {
		case  1:
			playTank(1);//开始游戏
			break;
		case 2:
			playTank(2); //自定义游戏
			break;
		case 3: 
			playTank(3); //读档游戏
			break;
		case 4:
			return 0; //退出
		}
	}
		

	return 0;
}

