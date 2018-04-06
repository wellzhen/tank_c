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
	/************  ��ʾ��ӭ����***************/
	showWelcomeWall();
	showWelcomeWords();
	Sleep(1000);

	while (1) {
		/*�˵� ѡ��*/
		system("cls");
		showWelcomeWall();
		showMenu();
		int optionNum = getMenuChoice();

		switch (optionNum) {
		case  1:
			playTank(1);//��ʼ��Ϸ
			break;
		case 2:
			playTank(2); //�Զ�����Ϸ
			break;
		case 3: 
			playTank(3); //������Ϸ
			break;
		case 4:
			return 0; //�˳�
		}
	}
		

	return 0;
}

