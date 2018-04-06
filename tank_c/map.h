#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "data.h"


extern int g_nMap[MAPHEIGHT][MAPWIDTH];
extern BULLET* g_pBulletMap[MAPHEIGHT][MAPWIDTH];
extern TANK* g_pTankMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;


//��ʼ����ǽ
void initOuterWall();
//��ʼ����ͼ
void initMapData();
//�Զ����ͼ
void customMapData();
//��ʾ�Զ����ͼ��Ҫ������
void showNeedStaticObj();
//��ӡ��ͼ��̬����
void drawMap();

//�ָ������ֲ��
void recoverDamagedPlant();

//����̬����㣺 ����g_nMap
void reDrawMapPoint(int row, int col);

//��ӡһ���ַ�  
void printChar(int posX, int posY, char* pszCha, WORD wArr);

