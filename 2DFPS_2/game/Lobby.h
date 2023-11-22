#pragma once

#include "iStd.h"
#include "Menu.h"
#include "Map.h"
#include "Proc.h"


void loadLobby();
void freeLobby();
void drawLobby(float dt);
bool keyLobby(iKeyState stat, iPoint p);

class StartLine
{
	
public:
	StartLine(iPoint p);
	~StartLine();

	void paint(float dt, iPoint off);
	bool touch(iRect rt);

	iPoint position;
	iImage* img;
	iImage* img2;
	iImage* img3;
};


