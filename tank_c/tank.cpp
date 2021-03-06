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
			moveTank(nDir, index); //移动
			shootBullet(index); //射击
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

//此函数同步g_pTankMap数据
void drawTank(int index, bool isShow)
{
	int posX = g_vecTank[index]->posX;
	int posY = g_vecTank[index]->posY;
	int dirNum = g_vecTank[index]->dir;
	for (int row = posY - 1; row <= posY + 1; row++) {
		for (int col = posX - 1; col <= posX + 1; col++) {
			if (g_TankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
				if (g_nMap[row][col] == MAP_TREE) {
					//树林要隐藏坦克： 不显示也不擦除
					int test = 1;
				}
				else if (isShow) {
					printChar(col, row, "■", g_vecTank[index]->color);
					g_pTankMap[row][col] = g_vecTank[index]; // 同步坦克地图
					//如果显示位置有植物
					if (g_nMap[row][col] == MAP_GRASS || g_nMap[row][col] == MAP_ICE) {//植物被破坏
						g_vecDamagedPlant.push_back({ col, row });   //同步受损植物地图
					}
				}
				else {
					printChar(col, row, "  ", COLOR_BLACK);
					g_pTankMap[row][col] = NULL;
					if (g_nMap[row][col] == MAP_GRASS || g_nMap[row][col] == MAP_ICE) {//植物被破坏
						g_vecDamagedPlant.push_back({ col, row });   //同步受损植物地图
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
	//速度设置
	clock_t start_time = g_vecTank[index]->last_move_time;
	clock_t end_time = clock();
	if (end_time - start_time < 150) {
		return false;
	}
	g_vecTank[index]->last_move_time = end_time;

	//参数；
	int nDestPosX = g_vecTank[index]->posX;
	int nDestPosY = g_vecTank[index]->posY;
	int nDestDir = g_vecTank[index]->dir;
	//判断移动操作，否则坐标不变
	int  blockNumSum = MAP_STONE + MAP_BRICK + MAP_RIVER;
	int changeValue = g_vecTank[index]->dir - nDirNum;
	if (changeValue == 0) { //前进，修改坐标,方向不变
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 2][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != NULL || g_pTankMap[nDestPosY - 2][nDestPosX] != NULL || g_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY--;
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 2] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL || g_pTankMap[nDestPosY][nDestPosX + 2] != NULL || g_pTankMap[nDestPosY + 1][nDestPosX + 1]!= NULL) {
				return false;//上中下--目的位置有墙
			}
			nDestPosX++;
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 2][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 1][nDestPosX - 1]!= 0 || g_pTankMap[nDestPosY + 2][nDestPosX] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY++;
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 2] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1]!= 0 || g_pTankMap[nDestPosY][nDestPosX - 2]!= 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上中下-目的位置有墙
			}
			nDestPosX--;
			break;
		default:
			printf("坦克前进方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	else if (changeValue == -1 || changeValue == -3 || changeValue == 1 || changeValue == 3) { //左右移动: 只改变方向（左右肩和尾部不能有物体）
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0) {
				return false;//左下右--目的位置有墙
			}
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//上左下--目的位置有墙
			}
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左上右--目的位置有墙
			}
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上右下-目的位置有墙
			}
			break;
		default:
			printf("坦克向左右方向移动参数dirNum错误（0123）： moveTank\n");
			return false;
		}
		nDestDir = nDirNum;
	}
	else if (changeValue == 2 || changeValue == -2) { //反向移动: 只改变坐标，方向不变（尾部三个方向不能有物体）
		switch (g_vecTank[index]->dir) {
		case DIR_UP:
			if ((g_nMap[nDestPosY + 2][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY + 2][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY + 2][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX] != 0 || g_pTankMap[nDestPosY + 2][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY++;
			break;
		case DIR_RIGHT:
			if ((g_nMap[nDestPosY - 1][nDestPosX - 2] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX - 2] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX - 2] != 0 || g_pTankMap[nDestPosY][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX - 2] != 0) {
				return false;//上中下--目的位置有墙
			}
			nDestPosX--;
			break;
		case DIR_DOWN:
			if ((g_nMap[nDestPosY - 2][nDestPosX - 1] & blockNumSum) != 0 || (g_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (g_nMap[nDestPosY - 2][nDestPosX + 1] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 2][nDestPosX - 1] != 0 || g_pTankMap[nDestPosY - 1][nDestPosX]!= 0 || g_pTankMap[nDestPosY - 2][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY--;
			break;
		case DIR_LEFT:
			if ((g_nMap[nDestPosY - 1][nDestPosX + 2] & blockNumSum) != 0 || (g_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (g_nMap[nDestPosY + 1][nDestPosX + 2] & blockNumSum) != 0
				|| g_pTankMap[nDestPosY - 1][nDestPosX + 2] != 0 || g_pTankMap[nDestPosY][nDestPosX + 1] != 0 || g_pTankMap[nDestPosY + 1][nDestPosX + 2] != 0) {
				return false;//上中下-目的位置有墙
			}
			nDestPosX++;
			break;
		default:
			printf("坦克向右方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	//允许移动
	drawTank(index, false); //擦除
	//修改属性
	g_vecTank[index]->posX = nDestPosX;
	g_vecTank[index]->posY = nDestPosY;
	g_vecTank[index]->dir = nDestDir;
	//绘制
	drawTank(index, true);
	//减速?
	return true;
}

void judgeAlive()
{
	int playerNum = 0;
	for (unsigned int i = 0; i < g_vecTank.size(); i++) {
		if (g_vecTank[i]->curHP > 0 ||g_vecTank[i]->isAlive == false) { //不用判断:有血或者已死
			continue;
		}
		if (g_vecTank[i]->nlife >= 1) { //用life换血
			g_vecTank[i]->curHP = g_vecTank[i]->maxHP; 
			g_vecTank[i]->nlife = g_vecTank[i]->nlife - 1;
			g_vecTank[i]->nDie = g_vecTank[i]->nDie + 1;
		}
		//凉了
		g_vecTank[i]->isAlive = false;
		drawTank(i, false);
	}
}





