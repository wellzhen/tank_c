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
	//清理全局数据（push_back的数据）
	clearGlobalData();

	if (gameType == 1) { //经典游戏
		initMapData();
		drawMap();

		//初始化玩家坦克: 默认一个
		initTank();
		drawTank(0, true);
		//初始化NPC坦克
		initNpcTank();
		drawTank(1, true);
	}
	else if (gameType == 2) {//自定义地图
		showNeedStaticObj();
		initOuterWall();
		drawMap();
		customMapData();

		//初始化玩家坦克: 默认一个
		initTank();
		drawTank(0, true);
		//初始化NPC坦克
		initNpcTank();
		drawTank(1, true);
	}
	else if (gameType == 3) {//读档
		readArchive();
		drawMap();

		//重新画一遍玩家坦克： 会自动更新g_pTankMap
		for (unsigned int i = 0; i < g_vecTank.size(); i++) {
			if ((!g_vecTank[i]->isNPC)&& g_vecTank[i]->isAlive) {
				drawTank(i, true);
			}
			
		}

	} 



	//控制坦克
	char chKey;
	int  nDirNum;
	bool hasPause = false;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 27) {//ESC键
				saveArchive();
				return;
			}
			else if (chKey == 32) { //空格
				hasPause = !hasPause; //取反
				continue;
			}
			else if (hasPause) {
				continue;
			}
			else if (chKey == 'j') {//发射炮弹
				shootBullet(0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //移动按键
				nDirNum = dirKey2DirNum(chKey);
				moveTank(nDirNum, 0);//暂时第一辆坦克
			}
			else {
				printf("非控制键： %c %d %d\n", chKey, chKey, (int)chKey);
			}
		}
		else if (hasPause) {
			continue;
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

void saveArchive()
{
	FILE * pFile;
	fopen_s(&pFile, "archive.db", "wb");
	//一、地图g_nMap数据
	fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//二、坦克
	//坦克数量
	int nTankSize = g_vecTank.size();
	fwrite(&nTankSize, sizeof(int), 1, pFile);
	//坦克数据
	for (int i = 0; i < nTankSize; i++) {
		fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
	}
	
	//三、子弹
	int nBulletSize = g_vecBullet.size();
	fwrite(&nBulletSize, sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
	}
	//五、受损植物地图
	//数量
	int nDamagedPlantSize = g_vecDamagedPlant.size();
	fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//数据
	for (int i = 0; i < nDamagedPlantSize; i++) {
		fwrite(&g_vecDamagedPlant[i], sizeof(POS), 1, pFile);
	}

	fclose(pFile);
}

void readArchive()
{

	FILE * pFile;
	fopen_s(&pFile, "archive.db", "rb");
	//一、地图g_nMap数据
	fread_s(g_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);	//fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//二、坦克
	//坦克数量
	int nTankSize;
	fread_s(&nTankSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nTankSize, sizeof(int), 1, pFile);
	//坦克数据
	for (int i = 0; i < nTankSize; i++) {
		TANK* pTank = (TANK*)malloc(sizeof(TANK));
		fread_s(pTank, sizeof(TANK), sizeof(TANK), 1, pFile);  //fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
		g_vecTank.push_back(pTank);
		pTank = NULL;
	}
	
	//三、子弹
	int nBulletSize;
	fread_s(&nBulletSize, sizeof(int), sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
		fread_s(pBullet, sizeof(BULLET), sizeof(BULLET), 1, pFile);//fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
		g_vecBullet.push_back(pBullet);
		pBullet = NULL;
	}
	//五、受损植物地图
	//数量
	int nDamagedPlantSize;
	fread_s(&nDamagedPlantSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//数据
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