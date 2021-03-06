#include <conio.h>
#include <time.h>
#include "bullet.h"
#include "tank.h"
#include  "map.h"

vector<BULLET *> g_vecBullet;
extern  BULLET * g_pBulletMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;


//此函数： 损坏植物时，同步受损植物数据；
void drawBullet(bool isShow)
{
	int posX;
	int posY;
	int color;
	for (unsigned int i = 0; i < g_vecBullet.size(); i++)
	{
		if (!g_vecBullet[i]->isValid) {
			continue;
		}
		posX = g_vecBullet[i]->posX;
		posY = g_vecBullet[i]->posY;
		color = g_vecBullet[i]->color;
		if (g_nMap[posY][posX] == MAP_TREE) { 
			//树林要隐藏子弹： 不显示也不擦除
		}
		else if (isShow) {
			if ((g_nMap[posY][posX] == MAP_GRASS) || (g_nMap[posY][posX] == MAP_ICE) || (g_nMap[posY][posX] == MAP_RIVER)) {
				//显示子弹（经过后擦除子弹位置，植物被破坏， 但是要恢复被破坏的植物）
				g_vecDamagedPlant.push_back({ posX, posY }); //同步地图
			}
			printChar(posX, posY, "⊙", color);	
		}
		else {
			if ((g_nMap[posY][posX] == MAP_GRASS) || (g_nMap[posY][posX] == MAP_ICE) || (g_nMap[posY][posX] == MAP_RIVER)) {//擦除位置有植物
				g_vecDamagedPlant.push_back({ posX, posY });  //同步地图
			}
			printChar(posX, posY, "  ", color);
		}
	}
}

//此函数0. 处理静态地图物体
//      1. 处理  g_pBulletMap
//      2. 检测g_pTankMap 是否击中了坦克
    
void removeInvalidBullet()
{
	int posX;
	int posY;

	for (unsigned int i = 0; i < g_vecBullet.size(); i++)
	{
		if (!g_vecBullet[i]->isValid) {
			continue;
		}
		posX = g_vecBullet[i]->posX;
		posY = g_vecBullet[i]->posY;

		//坦克地图检测
		if (g_pTankMap[posY][posX] != NULL) {//击中了坦克
			BULLET* pBullet = g_vecBullet[i];
			TANK* pTank = g_pTankMap[posY][posX];
			if (pBullet->isNPC != pTank->isNPC) { //敌人： 减血
				pTank->curHP -= pBullet->power;
			}
			//子弹失效
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}// 开始静态地图检测
		else if (g_nMap[posY][posX] == MAP_STONE) { //stone: 子弹失效
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (g_nMap[posY][posX] == MAP_SPACE) { //空区域：不操作 
			//nothing
		}
		else if (g_nMap[posY][posX] == MAP_BRICK) {//砖块： 都消失
			g_nMap[posY][posX] = MAP_SPACE;
			reDrawMapPoint(posY, posX);
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (g_nMap[posY][posX] == MAP_TREE) {//树林
			//nothing 
		}
		else if (g_nMap[posY][posX] == MAP_ICE || g_nMap[posY][posX] == MAP_GRASS || g_nMap[posY][posX] == MAP_RIVER) { //草地和冰块
			//nothing
		}
		else if (posX < 0 || posX >= MAPWIDTH || posY < 0 || posY >= MAPHEIGHT) {
			//never happen
			printf("子弹坐标异常：（%d, %d）\n", posX, posY);

		}
		else {
			printf(" 尚未定义的地图静态对象： removeInvalidBullet()\n");
			int map = g_nMap[posY][posX];
			printf(" x = %d y = %d  %d\n", posX, posY, map);
		}
	}
	//crontab: 删除无效子弹
	for (vector<BULLET*>::iterator itr = g_vecBullet.begin(); itr != g_vecBullet.end(); ++itr) {
		if (!(*itr)->isValid) {
			g_vecBullet.erase(itr);
			break;
		}
	}

}

bool runBullet()
{
	//擦除所有子弹
	drawBullet(false);
	for (unsigned int i = 0; i < g_vecBullet.size(); i++)
	{
		if (!g_vecBullet[i]->isValid) {
			continue;
		}
		//飞行速度设置
		clock_t start_time = g_vecBullet[i]->last_fly_time;
		clock_t end_time = clock();
		if (end_time - start_time < 50) {
			continue;
		}
		g_vecBullet[i]->last_fly_time = end_time;

		//子弹坐标
		int posX = g_vecBullet[i]->posX;
		int posY = g_vecBullet[i]->posY;
		//修改坐标
		switch (g_vecBullet[i]->dir) {
		case DIR_UP:
			posY--;
			break;
		case DIR_DOWN:
			posY++;
			break;
		case DIR_LEFT:
			posX--;
			break;
		case DIR_RIGHT:
			posX++;
			break;
		default:
			printf("错误的子弹方向属性：drawbullet\n");
			return false;
			break;
		}
		g_vecBullet[i]->posX = posX;
		g_vecBullet[i]->posY = posY;
	}
	//移除无效子弹
	removeInvalidBullet();
	//显示
	drawBullet(true);
	return true;
}



void shootBullet(int nTankIndex)
{
	//射速设置
	clock_t start_time = g_vecTank[nTankIndex]->last_shoot_time;
	clock_t end_time = clock();
	if (g_vecTank[nTankIndex]->isNPC && end_time - start_time < 500) {
		return;
	}
	else if (!g_vecTank[nTankIndex]->isNPC && end_time - start_time < 50) {
		return;
	}
	g_vecTank[nTankIndex]->last_shoot_time = end_time;

	int posX = g_vecTank[nTankIndex]->posX;
	int posY = g_vecTank[nTankIndex]->posY;
	int dir = g_vecTank[nTankIndex]->dir;
	switch (dir) {
	case DIR_UP:
		posY -= 2;
		break;
	case DIR_DOWN:
		posY += 2;
		break;
	case DIR_LEFT:
		posX -= 2;
		break;
	case DIR_RIGHT:
		posX += 2;
		break;
	default:
		printf("错误的子弹初始方向：shootBullet\n");
		return;
		break;
	}

	BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
	pBullet->posX = posX;
	pBullet->posY = posY;
	pBullet->dir = dir;
	pBullet->isNPC = g_vecTank[nTankIndex]->isNPC;
	pBullet->tankId = nTankIndex;
	pBullet->last_fly_time = 0;
	pBullet->power = 10; 
	pBullet->color = g_vecTank[nTankIndex]->color;

	g_vecBullet.push_back(pBullet);
	//立即移除无效子弹
	removeInvalidBullet();
	//显示
	drawBullet(true);
}
