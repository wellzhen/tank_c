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
	//��ʼ����ͼ
	initMapData();
	drawMap();
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

