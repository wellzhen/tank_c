#pragma once

//开始玩坦克游戏
void playTank();

//方向键转方向宏
int dirKey2DirNum(char keyWord);

//设置控制台大小
void initConsoleWindow();

void shootBullet(int nTankIndex);

//初始化输入法
void initInputShift();
