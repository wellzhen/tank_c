#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "data.h"


extern int g_nMap[MAPHEIGHT][MAPWIDTH];
extern BULLET* g_pBulletMap[MAPHEIGHT][MAPWIDTH];
extern TANK* g_pTankMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;

//��ʼ����ͼ
void initMapData();

//��ӡ��ͼ��̬
void drawMap();

//�ָ������ֲ��
void recoverDamagedPlant();

//����̬����㣺 ����g_nMap
void reDrawMapPoint(int row, int col);

//��ӡһ���ַ�  
void printChar(int posX, int posY, char* pszCha, WORD wArr);

