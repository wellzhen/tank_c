#include <conio.h>
#include <time.h>
#include "data.h"
#include "map.h"
#include "tank.h"
#include "bullet.h"

vector<TANK *>  g_vecTank;
extern TANK * g_pTankMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;

bool moveTank(int nDir, int index);

void initNpcTank()
{
	for (int nNum = 0; nNum < 1; nNum++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = true;
		pTank->posX = 3 + nNum* 3;
		pTank->posY = 6;
		pTank->color = COLOR_RED;
		pTank->dir = DIR_DOWN;
		pTank->maxHP = 100;
		pTank->curHP = 100;
		pTank->maxSpeed = 60;
		pTank->curSpeed = 50;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 0;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;

		g_vecTank.push_back(pTank);
		drawTank(g_vecTank.size() - 1, true);
	}
	
}

void autoRunNpcTank()
{
	srand((unsigned int)time(NULL));
	for (unsigned int index = 0; index < g_vecTank.size(); index++) {
		if (g_vecTank[index]->isNPC && g_vecTank[index]->isAlive) {
			int nDir = rand() % 4;
			moveTank(nDir, index); //�ƶ�
			shootBullet(index); //���
		}
	}
}

void initTank()
{
	TANK*  pTank = (TANK*)malloc(sizeof(TANK));
	pTank->isNPC = false;
	pTank->posX = MAPWIDTH / 2 - 4;
	pTank->posY = MAPHEIGHT - 3;
	pTank->color = COLOR_BLUE;
	pTank->dir = DIR_UP;
	pTank->maxHP = 100;
	pTank->curHP = 100;
	pTank->maxSpeed = 60;
	pTank->curSpeed = 50;
	pTank->oldSpeed = 0;
	pTank->nKill = 0;
	pTank->nDie = 0;
	pTank->nlife = 30;
	pTank->nlevel = 1;
	pTank->last_move_time = 0;
	pTank->last_shoot_time = 0;
	pTank->isAlive = true;

	g_vecTank.push_back(pTank);
}

//�˺���ͬ��g_pTankMap����
void drawTank(int index, bool isShow)
{
	int posX = g_vecTank[index]->posX;
	int posY = g_vecTank[index]->posY;
	int dirNum = g_vecTank[index]->dir;
	for (int row = posY - 1; row <= posY + 1; row++) {
		for (int col = posX - 1; col <= posX + 1; col++) {
			if (g_TankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
				if (g_nMap[row][col] == MAP_TREE) {
					//����Ҫ����̹�ˣ� ����ʾҲ������
					int test = 1;
				}
				else if (isShow) {
					printChar(col, row, "��", g_vecTank[index]->color);
					g_pTankMap[row][col] = g_vecTank[index]; // ͬ��̹�˵�ͼ
					//�����ʾλ����ֲ��
					if (g_nMap[row][col] == MAP_GRASS || g_nMap[row][col] == MAP_ICE) {//ֲ�ﱻ�ƻ�
						g_vecDamagedPlant.push_back({ col, row });   //ͬ������ֲ���ͼ
					}
				}
				else {
					printChar(col, row, "  ", COLOR_BLACK);
					g_pTankMap[row][col] = NULL;
					if (g_nMap[row][col] == MAP_GRASS || g_nMap[row][col] == MAP_ICE) {//ֲ�ﱻ�ƻ�
						g_vecDamagedPlant.push_back({ col, row });   //ͬ������ֲ���ͼ
					}
				}
			}
		}
	}
}


bool moveTank(int nDirNum, int index = 0)
{
	if (g_vecTank[index]->isAlive == false) {
		return false;
	}
	//�ٶ�����
	clock_t start_time = g_vecTank[index]->last_move_time;
	clock_t end_time = clock();
	if (end_time - start_time < 150) {
		return false;
	}
	g_vecTank[index]->last_move_time = end_time;

	//������
	int nDestPosX = g_vecTank[index]->posX;
	int nDestPosY = g_vecTank[index]->posY;
	int nDestDir = g_vecTank[index]->dir;
	//�ж��ƶ��������������겻��
	int  blockNumSum = MAP_STONE + MAP_BRICK + MAP_RIVER;
	int changeValue = g_vecTank[index]->dir - nDirNum;
	if (changeValue == 0) { //ǰ�����޸�����,���򲻱�
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 2][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != NULL || g_pTankMap[nDestPosY - 2][nDestPosX] != NULL || g_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY--;
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 2] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL || g_pTankMap[nDestPosY][nDestPosX + 2] != NULL || g_pTankMap[nDestPosY + 1][nDestPosX + 1]!= NULL) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosX++;
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 2][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 1][nDestPosX - 1]!= 0 || g_pTankMap[nDestPosY + 2][nDestPosX] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY++;
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 2] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1]!= 0 || g_pTankMap[nDestPosY][nDestPosX - 2]!= 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			nDestPosX--;
			break;
		default:
			printf("̹��ǰ���������dirNum����0123���� moveTank\n");
			return false;
		}
	}
	else if (changeValue == -1 || changeValue == -3 || changeValue == 1 || changeValue == 3) { //�����ƶ�: ֻ�ı䷽�����Ҽ��β�����������壩
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			break;
		default:
			printf("̹�������ҷ����ƶ�����dirNum����0123���� moveTank\n");
			return false;
		}
		nDestDir = nDirNum;
	}
	else if (changeValue == 2 || changeValue == -2) { //�����ƶ�: ֻ�ı����꣬���򲻱䣨β�����������������壩
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY + 2][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 2][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 2][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX] != 0 || g_pTankMap[nDestPosY + 2][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY++;
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 2] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 2] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 2] != 0 || g_pTankMap[nDestPosY][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 2] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosX--;
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY - 2][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 2][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 2][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX]!= 0 || g_pTankMap[nDestPosY - 2][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY--;
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 2] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 2] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 2] != 0 || g_pTankMap[nDestPosY][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 2] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			nDestPosX++;
			break;
		default:
			printf("̹�����ҷ������dirNum����0123���� moveTank\n");
			return false;
		}
	}
	//�����ƶ�
	drawTank(index, false); //����
	//�޸�����
	g_vecTank[index]->posX = nDestPosX;
	g_vecTank[index]->posY = nDestPosY;
	g_vecTank[index]->dir = nDestDir;
	//����
	drawTank(index, true);
	//����?
	return true;
}

void judgeAlive()
{
	int playerNum = 0;
	for (unsigned int i = 0; i < g_vecTank.size(); i++) {
		if (g_vecTank[i]->curHP > 0 ||g_vecTank[i]->isAlive == false) { //�����ж�:��Ѫ��������
			continue;
		}
		if (g_vecTank[i]->nlife >= 1) { //��life��Ѫ
			g_vecTank[i]->curHP = g_vecTank[i]->maxHP; 
			g_vecTank[i]->nlife = g_vecTank[i]->nlife - 1;
			g_vecTank[i]->nDie = g_vecTank[i]->nDie + 1;
		}
		//����
		g_vecTank[i]->isAlive = false;
		drawTank(i, false);
	}
}





