#include <conio.h>
#include "data.h"
#include "map.h"
#include "tank.h"
#include "bullet.h"


extern  vector<TANK *>  g_vecTank;
extern  vector<BULLET*> g_vecBullet;

int dirKey2DirNum(char keyWord);


void playTank()
{
	system("cls");
	//初始化地图
	initMapData();
	drawMap();
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

