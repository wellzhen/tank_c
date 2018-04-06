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
	//初始化地图
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
	
	
	//初始化玩家坦克: 默认一个
	initTank();
	drawTank(0, true);
	//初始化NPC坦克
	initNpcTank();
	drawTank(1, true);
	//控制坦克
	char chKey;
	int  nDirNum;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 'j') {//发射炮弹
				shootBullet(0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //移动按键
				nDirNum = dirKey2DirNum(chKey);
				moveTank(nDirNum, 0);//暂时第一辆坦克
			}
			else {
				printf("错误的按键： %c\n", chKey);
			}
		}

		
		//Npc坦克自动运行
		autoRunNpcTank();
		//子弹前进
		runBullet();
		//判断坦克生存情况
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
		printf("尚未设置的按键映射： dirkey2DirNum\n");
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
				printChar(col, row, "■", COLOR_GRAY);
			}
		}
	}
}
void  showWelcomeWords()
{
	char title[] = "坦克大战";
	char tip[] = "Wlecome To War Of Tank";
	printChar((MAPWIDTH - 2 - strlen(title))/2, MAPHEIGHT / 2, title, COLOR_WHITE);
	printChar((MAPWIDTH - 2 - strlen(tip)) / 2, MAPHEIGHT / 2 + 2, tip, COLOR_RED);
}

void showMenu()
{

	char menu[][30] = { "1  经典游戏  ",
					     "2  自定义地图",
					     "3  读档游戏  ",
					     "4  退出游戏  " 
						};

	char tips[] = "请输入您的选择 \n";

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