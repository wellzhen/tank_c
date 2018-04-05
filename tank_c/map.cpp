#include <Windows.h>
#include "map.h"
#include "data.h"

int g_nMap[MAPHEIGHT][MAPWIDTH] = { 0 };
BULLET *  g_pBulletMap[MAPHEIGHT][MAPWIDTH] = { NULL };
TANK * g_pTankMap[MAPHEIGHT][MAPWIDTH] = { NULL };
vector<POS> g_vecDamagedPlant;

void printChar(int posX, int posY, char* pszChar, WORD wArr)
{
	//1 ���ù������
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	//2. �޸Ĺ���ڿ���̨�е�����λ��
	COORD loc;
	loc.X = posX * 2;
	loc.Y = posY;
	// ������ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wArr);
	//3. ���ù���ڿ���̨�е�����λ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar);
}

void initMapData()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				g_nMap[row][col] = MAP_STONE;
			}
			else {
				g_nMap[row][col] = MAP_SPACE;
			}
		}
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		g_nMap[4][col] = MAP_GRASS;
		g_nMap[5][col] = MAP_GRASS;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		g_nMap[8][col] = MAP_TREE;
		g_nMap[9][col] = MAP_TREE;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		g_nMap[12][col] = MAP_ICE;
		g_nMap[13][col] = MAP_ICE;
	}


	for (int col = 0; col < 10; col++) {
		g_nMap[17][col] = MAP_STONE;
		g_nMap[18][col] = MAP_STONE;
	}

	for (int col = MAPWIDTH - 15; col < MAPWIDTH - 1; col++) {
		g_nMap[17][col] = MAP_RIVER;
		g_nMap[18][col] = MAP_RIVER;
	}
	for (int row = 20; row < 30; row++) {
		for (int col = 1; col < MAPWIDTH - 1; col++) {
			g_nMap[row][col] = MAP_BRICK;

		}
	}

}

//������posX, posY��
void reDrawMapPoint(int row, int col)
{
	if (g_nMap[row][col] == MAP_STONE) {
		printChar(col, row, "��", COLOR_GRAY);
	}
	else if (g_nMap[row][col] == MAP_BRICK) {
		printChar(col, row, "��", COLOR_RED_LIGHT);
	}
	else if (g_nMap[row][col] == MAP_SPACE) {
		printChar(col, row, "  ", COLOR_BLACK);
	}
	else if (g_nMap[row][col] == MAP_GRASS) {
		printChar(col, row, "��", COLOR_GREEN);
	}
	else if (g_nMap[row][col] == MAP_ICE) {
		printChar(col, row, "��", COLOR_WHITE);
	}
	else if (g_nMap[row][col] == MAP_TREE) {
		printChar(col, row, "��", COLOR_GREEN_LIGHT);
	}
	else if (g_nMap[row][col] == MAP_RIVER) {
		printChar(col, row, "��", COLOR_GRAY);
	}
	else {
		printf("δ����ĵ�ͼ����\n");
	}
}

void drawMap()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			reDrawMapPoint(row, col);
		}
	}

}

void recoverDamagedPlant()
{
	for (vector<POS>::iterator it = g_vecDamagedPlant.begin(); it != g_vecDamagedPlant.end(); ) {
		int posX = it->posX;
		int posY = it->posY;
		if (g_nMap[posY][posX] != 0 && g_pBulletMap[posY][posX] == NULL && g_pTankMap[posY][posX] == NULL) { 
			//û��̹�˻��ӵ�ʱ�Żָ�ֲ��
			reDrawMapPoint(posY, posX);
			it = g_vecDamagedPlant.erase(it);
		}
		else {
			++it;
		}
	}

}



