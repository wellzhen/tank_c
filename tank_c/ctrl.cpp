#include <conio.h>
#include "data.h"
#include "map.h"
#include "tank.h"
#include "bullet.h"


extern  vector<TANK *>  g_vecTank;
extern  vector<BULLET*> g_vecBullet;

int dirKey2DirNum(char keyWord);
void saveArchive();
void readArchive();
void clearGlobalData();

void playTank(int gameType)
{
	system("cls");
	//����ȫ�����ݣ�push_back�����ݣ�
	clearGlobalData();

	if (gameType == 1) { //������Ϸ
		initMapData();
		drawMap();

		//��ʼ�����̹��: Ĭ��һ��
		initTank();
		drawTank(0, true);
		//��ʼ��NPC̹��
		initNpcTank();
		drawTank(1, true);
	}
	else if (gameType == 2) {//�Զ����ͼ
		showNeedStaticObj();
		initOuterWall();
		drawMap();
		customMapData();

		//��ʼ�����̹��: Ĭ��һ��
		initTank();
		drawTank(0, true);
		//��ʼ��NPC̹��
		initNpcTank();
		drawTank(1, true);
	}
	else if (gameType == 3) {//����
		readArchive();
		drawMap();

		//���»�һ�����̹�ˣ� ���Զ�����g_pTankMap
		for (unsigned int i = 0; i < g_vecTank.size(); i++) {
			if ((!g_vecTank[i]->isNPC)&& g_vecTank[i]->isAlive) {
				drawTank(i, true);
			}
			
		}

	} 



	//����̹��
	char chKey;
	int  nDirNum;
	bool hasPause = false;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 27) {//ESC��
				saveArchive();
				return;
			}
			else if (chKey == 32) { //�ո�
				hasPause = !hasPause; //ȡ��
				continue;
			}
			else if (hasPause) {
				continue;
			}
			else if (chKey == 'j') {//�����ڵ�
				shootBullet(0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //�ƶ�����
				nDirNum = dirKey2DirNum(chKey);
				moveTank(nDirNum, 0);//��ʱ��һ��̹��
			}
			else {
				printf("�ǿ��Ƽ��� %c %d %d\n", chKey, chKey, (int)chKey);
			}
		}
		else if (hasPause) {
			continue;
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

void saveArchive()
{
	FILE * pFile;
	fopen_s(&pFile, "archive.db", "wb");
	//һ����ͼg_nMap����
	fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//����̹��
	//̹������
	int nTankSize = g_vecTank.size();
	fwrite(&nTankSize, sizeof(int), 1, pFile);
	//̹������
	for (int i = 0; i < nTankSize; i++) {
		fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
	}
	
	//�����ӵ�
	int nBulletSize = g_vecBullet.size();
	fwrite(&nBulletSize, sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
	}
	//�塢����ֲ���ͼ
	//����
	int nDamagedPlantSize = g_vecDamagedPlant.size();
	fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//����
	for (int i = 0; i < nDamagedPlantSize; i++) {
		fwrite(&g_vecDamagedPlant[i], sizeof(POS), 1, pFile);
	}

	fclose(pFile);
}

void readArchive()
{

	FILE * pFile;
	fopen_s(&pFile, "archive.db", "rb");
	//һ����ͼg_nMap����
	fread_s(g_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);	//fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//����̹��
	//̹������
	int nTankSize;
	fread_s(&nTankSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nTankSize, sizeof(int), 1, pFile);
	//̹������
	for (int i = 0; i < nTankSize; i++) {
		TANK* pTank = (TANK*)malloc(sizeof(TANK));
		fread_s(pTank, sizeof(TANK), sizeof(TANK), 1, pFile);  //fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
		g_vecTank.push_back(pTank);
		pTank = NULL;
	}
	
	//�����ӵ�
	int nBulletSize;
	fread_s(&nBulletSize, sizeof(int), sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
		fread_s(pBullet, sizeof(BULLET), sizeof(BULLET), 1, pFile);//fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
		g_vecBullet.push_back(pBullet);
		pBullet = NULL;
	}
	//�塢����ֲ���ͼ
	//����
	int nDamagedPlantSize;
	fread_s(&nDamagedPlantSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//����
	for (int i = 0; i < nDamagedPlantSize; i++) {
		POS  damagedPlant;
		fread_s(&damagedPlant, sizeof(POS), sizeof(POS), 1, pFile);//fwrite(&g_vecDamagedPlant[i], sizeof(POS), 1, pFile);
		g_vecDamagedPlant.push_back(damagedPlant);
	}
	fclose(pFile);
}

void clearGlobalData()
{
	g_vecTank.clear();
	g_vecBullet.clear();
	g_vecDamagedPlant.clear();
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			g_pTankMap[row][col] = 0;
			g_pBulletMap[row][col] = 0;
			g_nMap[row][col] = 0;
		}
	}
}