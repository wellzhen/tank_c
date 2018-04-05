#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "data.h"


extern int g_nMap[MAPHEIGHT][MAPWIDTH];
extern BULLET* g_pBulletMap[MAPHEIGHT][MAPWIDTH];
extern TANK* g_pTankMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;

//初始化地图
void initMapData();

//打印地图静态
void drawMap();

//恢复受损的植物
void recoverDamagedPlant();

//画静态物体点： 依据g_nMap
void reDrawMapPoint(int row, int col);

//打印一个字符  
void printChar(int posX, int posY, char* pszCha, WORD wArr);

