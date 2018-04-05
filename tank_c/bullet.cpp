#include <conio.h>
#include <time.h>
#include "bullet.h"
#include "tank.h"
#include  "map.h"

vector<BULLET *> g_vecBullet;
extern  BULLET * g_pBulletMap[MAPHEIGHT][MAPWIDTH];
extern vector<POS> g_vecDamagedPlant;


//�˺����� ��ֲ��ʱ��ͬ������ֲ�����ݣ�
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
			//����Ҫ�����ӵ��� ����ʾҲ������
		}
		else if (isShow) {
			if ((g_nMap[posY][posX] == MAP_GRASS) || (g_nMap[posY][posX] == MAP_ICE) || (g_nMap[posY][posX] == MAP_RIVER)) {
				//��ʾ�ӵ�������������ӵ�λ�ã�ֲ�ﱻ�ƻ��� ����Ҫ�ָ����ƻ���ֲ�
				g_vecDamagedPlant.push_back({ posX, posY }); //ͬ����ͼ
			}
			printChar(posX, posY, "��", color);	
		}
		else {
			if ((g_nMap[posY][posX] == MAP_GRASS) || (g_nMap[posY][posX] == MAP_ICE) || (g_nMap[posY][posX] == MAP_RIVER)) {//����λ����ֲ��
				g_vecDamagedPlant.push_back({ posX, posY });  //ͬ����ͼ
			}
			printChar(posX, posY, "  ", color);
		}
	}
}

//�˺���0. ����̬��ͼ����
//      1. ����  g_pBulletMap
//      2. ���g_pTankMap �Ƿ������̹��
    
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

		//̹�˵�ͼ���
		if (g_pTankMap[posY][posX] != NULL) {//������̹��
			BULLET* pBullet = g_vecBullet[i];
			TANK* pTank = g_pTankMap[posY][posX];
			if (pBullet->isNPC != pTank->isNPC) { //���ˣ� ��Ѫ
				pTank->curHP -= pBullet->power;
			}
			//�ӵ�ʧЧ
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}// ��ʼ��̬��ͼ���
		else if (g_nMap[posY][posX] == MAP_STONE) { //stone: �ӵ�ʧЧ
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (g_nMap[posY][posX] == MAP_SPACE) { //�����򣺲����� 
			//nothing
		}
		else if (g_nMap[posY][posX] == MAP_BRICK) {//ש�飺 ����ʧ
			g_nMap[posY][posX] = MAP_SPACE;
			reDrawMapPoint(posY, posX);
			g_vecBullet[i]->isValid = false;
			g_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (g_nMap[posY][posX] == MAP_TREE) {//����
			//nothing 
		}
		else if (g_nMap[posY][posX] == MAP_ICE || g_nMap[posY][posX] == MAP_GRASS || g_nMap[posY][posX] == MAP_RIVER) { //�ݵغͱ���
			//nothing
		}
		else if (posX < 0 || posX >= MAPWIDTH || posY < 0 || posY >= MAPHEIGHT) {
			//never happen
			printf("�ӵ������쳣����%d, %d��\n", posX, posY);

		}
		else {
			printf(" ��δ����ĵ�ͼ��̬���� removeInvalidBullet()\n");
			int map = g_nMap[posY][posX];
			printf(" x = %d y = %d  %d\n", posX, posY, map);
		}
	}
	//crontab: ɾ����Ч�ӵ�
	for (vector<BULLET*>::iterator itr = g_vecBullet.begin(); itr != g_vecBullet.end(); ++itr) {
		if (!(*itr)->isValid) {
			g_vecBullet.erase(itr);
			break;
		}
	}

}

bool runBullet()
{
	//���������ӵ�
	drawBullet(false);
	for (unsigned int i = 0; i < g_vecBullet.size(); i++)
	{
		if (!g_vecBullet[i]->isValid) {
			continue;
		}
		//�����ٶ�����
		clock_t start_time = g_vecBullet[i]->last_fly_time;
		clock_t end_time = clock();
		if (end_time - start_time < 50) {
			continue;
		}
		g_vecBullet[i]->last_fly_time = end_time;

		//�ӵ�����
		int posX = g_vecBullet[i]->posX;
		int posY = g_vecBullet[i]->posY;
		//�޸�����
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
			printf("������ӵ��������ԣ�drawbullet\n");
			return false;
			break;
		}
		g_vecBullet[i]->posX = posX;
		g_vecBullet[i]->posY = posY;
	}
	//�Ƴ���Ч�ӵ�
	removeInvalidBullet();
	//��ʾ
	drawBullet(true);
	return true;
}



void shootBullet(int nTankIndex)
{
	//��������
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
		printf("������ӵ���ʼ����shootBullet\n");
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
	//�����Ƴ���Ч�ӵ�
	removeInvalidBullet();
	//��ʾ
	drawBullet(true);
}
