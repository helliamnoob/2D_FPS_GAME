#pragma once

#include "iStd.h"
#include "menu.h"
#include "loading.h"

void loadIntro();
void drawIntro(float dt);
void freeIntro();
bool keyIntro(iKeyState stat, iPoint p);

//¹è°æ
void createBg();
void freeBg();
void drawBg(float dt);

void createPopAny();
void freePopAny();
void drawPopAny(float dt);
bool keyPopAny(iKeyState stat, iPoint p);
void showPopAny(bool show);

