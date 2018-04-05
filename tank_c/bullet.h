#pragma once
#include <Windows.h>
#include <conio.h>



//发射子弹
void shootBullet(int nTankIndex);
//炮弹飞行 
bool runBullet();
//绘制子弹
void drawBullet(bool isShow);
//移除无效的子弹
void removeInvalidBullet();
