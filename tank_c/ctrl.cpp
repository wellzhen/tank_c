#include <conio.h>
#include "data.h"
#include "map.h"
#include "tank.h"
#include "bullet.h"


extern  vector<TANK *>  g_vecTank;
extern  vector<BULLET*> g_vecBullet;

int dirKey2DirNum(char keyWord);


void playTank(int gameType)
{
	system("cls");
	//��ʼ����ͼ
	if (gameType == 1) {
		initMapData();
		drawMap();
	}
	else if (gameType == 2) {
		showNeedStaticObj();
		initOuterWall();
		drawMap();
		customMapData();
	}
	else if (gameType == 3) {
		initMapData();
		drawMap();
	}
	
	
	//��ʼ�����̹��: Ĭ��һ��
	initTank();
	drawTank(0, true);
	//��ʼ��NPC̹��
	initNpcTank();
	drawTank(1, true);
	//����̹��
	char chKey;
	int  nDirNum;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 'j') {//�����ڵ�
				shootBullet(0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //�ƶ�����
				nDirNum = dirKey2DirNum(chKey);
				moveTank(nDirNum, 0);//��ʱ��һ��̹��
			}
			else {
				printf("����İ����� %c\n", chKey);
			}
		}

		
		//Npc̹���Զ�����
		autoRunNpcTank();
		//�ӵ�ǰ��
		runBullet();
		//�ж�̹���������
		judgeAlive();
		recoverDamagedPlant();


	}

}

void initConsoleWindow()
{
	system("mode con cols=140 lines=50");
}

void initInputShift()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}

int dirKey2DirNum(char keyWord)
{
	switch (keyWord) {
	case 'w':
		return DIR_UP;
		break;
	case 'a':
		return DIR_LEFT;
		break;
	case 's':
		return DIR_DOWN;
		break;
	case 'd':
		return DIR_RIGHT;
		break;
	default:
		printf("��δ���õİ���ӳ�䣺 dirkey2DirNum\n");
		return -1;
		break;
	}
}

void  showWelcomeWall()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				g_nMap[row][col] = MAP_STONE;
				printChar(col, row, "��", COLOR_GRAY);
			}
		}
	}
}
void  showWelcomeWords()
{
	char title[] = "̹�˴�ս";
	char tip[] = "Wlecome To War Of Tank";
	printChar((MAPWIDTH - 2 - strlen(title))/2, MAPHEIGHT / 2, title, COLOR_WHITE);
	printChar((MAPWIDTH - 2 - strlen(tip)) / 2, MAPHEIGHT / 2 + 2, tip, COLOR_RED);
}

void showMenu()
{

	char menu[][30] = { "1  ������Ϸ  ",
					     "2  �Զ����ͼ",
					     "3  ������Ϸ  ",
					     "4  �˳���Ϸ  " 
						};

	char tips[] = "����������ѡ�� \n";

	for (int i = 0;i<  _countof(menu); i++) {
		printChar((MAPWIDTH - 2 - strlen(menu[i])) / 2, MAPHEIGHT / 2 - 6  + i * 2, menu[i], COLOR_WHITE);
	}
	printChar((MAPWIDTH - 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 4 + _countof(menu) * 2 , tips, COLOR_RED);
	printChar((MAPWIDTH + 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 2 + _countof(menu) * 2, " ", COLOR_RED);
}

int getMenuChoice()
{
	int menuNum = 0;
	while (1) {
		if (menuNum == 1 || menuNum == 2 || menuNum == 3 || menuNum == 4 || menuNum == 5 || menuNum == 6) {
			return menuNum;
		}
		else {
			//menuNum = 0;
			scanf_s("%d", &menuNum);
		}
	}
}