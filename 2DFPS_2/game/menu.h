#pragma once

#include "iStd.h"
#include "iOpenAL.h"
#include "Loading.h"
#include "Lobby.h"

void loadMenu();
void drawMenu(float dt);
void freeMenu();
bool keyMenu(iKeyState stat, iPoint p);

void createPopMenu();
void freePopMenu();
void drawPopMenu(float dt);
bool keyPopMenu(iKeyState stat, iPoint p);
void showPopMenu(bool show);

//�ɼ�
void createPopSetting();
void freePopSetting();
void drawPopSetting(float dt);
bool keyPopSetting(iKeyState stat, iPoint p);
void showPopSetting(bool show);

//���ӹ��
void createPopHow();
void freePopHow();
void drawPopHow(float dt);
bool keyPopHow(iKeyState stat, iPoint p);
void showPopHow(bool show);


//���� �˾�

void createPopExit();
void freePopExit();
void drawPopexit(float dt);
bool keyPopExit(iKeyState stat, iPoint p);
void showPopExit(bool show);