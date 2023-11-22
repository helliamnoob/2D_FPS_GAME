#pragma once

#include "iStd.h"

void loadMap();
void freeMap();
void drawMap(float dt);
bool keyMap(iKeyState stat, iPoint p);

struct Bg
{
	Bg();
	virtual ~Bg();
	void draw(float dt);
	void setHeroPostion(iPoint& p);

	Texture** tex;
	iPoint position;

	iPoint off, max, min;
};

void loadProcBg();
void freeProcBg();
void drawProcBg(float dt);
void moveProcBg();