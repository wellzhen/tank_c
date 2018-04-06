#include <Windows.h>
#include "map.h"
#include "data.h"

int g_nMap[MAPHEIGHT][MAPWIDTH] = { 0 };
BULLET *  g_pBulletMap[MAPHEIGHT][MAPWIDTH] = { NULL };
TANK * g_pTankMap[MAPHEIGHT][MAPWIDTH] = { NULL };
vector<POS> g_vecDamagedPlant;

void printChar(int posX, int posY, char* pszChar, WORD wArr)
{
	//1 设置光标属性
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	//2. 修改光标在控制台中的坐标位置
	COORD loc;
	loc.X = posX * 2;
	loc.Y = posY;
	// 设置颜色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wArr);
	//3. 设置光标在控制台中的坐标位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar);
}

void initOuterWall()
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
}
void initMapData()
{
	initOuterWall();
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


void showNeedStaticObj()
{
	int nDestPosX = MAPWIDTH + 2;
	printChar(nDestPosX, 2,  "□ 空地", COLOR_WHITE); //空地
	printChar(nDestPosX, 4,  "■ 石块", COLOR_GRAY);  //石块
	printChar(nDestPosX, 6,  "■ 砖块", COLOR_RED_LIGHT); // 砖块
	printChar(nDestPosX, 8,  "∷ 草地", COLOR_GREEN);   //草
	printChar(nDestPosX, 10, "※ 雪地", COLOR_WHITE);   //雪
	printChar(nDestPosX, 12, "↑ 树木", COLOR_GREEN_LIGHT); //树
	printChar(nDestPosX, 14, "≈ 河流", COLOR_GRAY);   //河流

	printChar(nDestPosX, 18, "Tip 左键编辑", COLOR_GRAY); 
	printChar(nDestPosX, 20, "    双击删除", COLOR_GRAY);
	printChar(nDestPosX, 22, "    右键游戏", COLOR_GRAY);  
}

void customMapData()
{
	initOuterWall();
	int nColorBoxPosX = MAPWIDTH + 1; //同showNeedStaticObj()保持一致

	int nBrushColorNum = 0; //鼠标的刷子颜色
	int mousePosX = 0;
	int mousePosY = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (1)
	{
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;
			switch (mer.dwEventFlags)
			{
			case 0:
				if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					//左键单击
					if (mousePosX >= nColorBoxPosX) { //点击了颜料区域
						switch (mousePosY) {
						case 2:
							nBrushColorNum =  MAP_SPACE;
							break;
						case 4:
							nBrushColorNum = MAP_STONE;
							break;
						case 6:
							nBrushColorNum = MAP_BRICK;
							break;
						case 8:
							nBrushColorNum = MAP_GRASS;
							break;
						case 10:
							nBrushColorNum = MAP_ICE;
							break;
						case 12:
							nBrushColorNum = MAP_TREE;
							break;
						case 14:
							nBrushColorNum = MAP_RIVER;
							break;
						default:
							//
							break;
						}
					}
					else if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 0 && mousePosY < MAPHEIGHT - 1) {
						g_nMap[mousePosY][mousePosX] = nBrushColorNum;
						reDrawMapPoint(mousePosY, mousePosX);
					}
				}
				else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				{
					//右键单击: 结束编辑编辑
					return;
				}
				break;
			case DOUBLE_CLICK:
				//双击 
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 0 && mousePosY < MAPHEIGHT - 1) {
					g_nMap[mousePosY][mousePosX] = 0;
					reDrawMapPoint(mousePosY, mousePosX);
				}
				break;
			case MOUSE_MOVED:
				mousePosX = mer.dwMousePosition.X /2;
				mousePosY = mer.dwMousePosition.Y;
				break;
			}
		}
	}

}

//参数（posX, posY）
void reDrawMapPoint(int row, int col)
{
	if (g_nMap[row][col] == MAP_STONE) {
		printChar(col, row, "■", COLOR_GRAY);
	}
	else if (g_nMap[row][col] == MAP_BRICK) {
		printChar(col, row, "■", COLOR_RED_LIGHT);
	}
	else if (g_nMap[row][col] == MAP_SPACE) {
		printChar(col, row, "  ", COLOR_BLACK);
	}
	else if (g_nMap[row][col] == MAP_GRASS) {
		printChar(col, row, "∷", COLOR_GREEN);
	}
	else if (g_nMap[row][col] == MAP_ICE) {
		printChar(col, row, "※", COLOR_WHITE);
	}
	else if (g_nMap[row][col] == MAP_TREE) {
		printChar(col, row, "↑", COLOR_GREEN_LIGHT);
	}
	else if (g_nMap[row][col] == MAP_RIVER) {
		printChar(col, row, "≈", COLOR_GRAY);
	}
	else {
		printf("未定义的地图物体\n");
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
			//没有坦克或子弹时才恢复植物
			reDrawMapPoint(posY, posX);
			it = g_vecDamagedPlant.erase(it);
		}
		else {
			++it;
		}
	}

}



