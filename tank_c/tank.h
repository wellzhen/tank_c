#pragma once
#include <Windows.h>
#include <conio.h>
#include "data.h"
#include "map.h"

extern vector<TANK *>  g_vecTank;

//��ʼ��Npc̹��
void initNpcTank();

//��ʼ�����̹��
void initTank();
//�Զ�����Npc̹��
void autoRunNpcTank();

//��ʾ̹�� true|| false
void drawTank(int index, bool isShow);

//�ƶ�̹��, ������ʾ������
bool moveTank(int nDir, int index);

//�ж�Ѫֵ�ʹ� ���е�̹��
void judgeAlive();




