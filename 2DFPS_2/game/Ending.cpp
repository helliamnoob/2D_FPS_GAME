#include "Ending.h"
#include "Menu.h"
#include "Proc.h"
#include "intro.h"

extern void drawPopAnyBefore(float dt, iPopup* pop);
extern void drawPopMenuBefore(float dt, iPopup* pop);
extern void drawPopSettingBefore(float dt, iPopup* pop);
extern void drawPopHowBefore(float dt, iPopup* pop);


void loadEnd()
{
	createPopAny();
	showPopAny(true);
	createPopMenu();
	createPopSetting();
	createPopHow();

}

void freeEnd()
{
	freePopAny();
	freePopMenu();
	freePopSetting();
	freePopHow();
}

void drawEnd(float dt)
{
	setStringSize(35);
	setStringRGBA(0, 1, 1, 1);
	setStringBorder(0);
	setStringName("assets/BMJUA_ttf.ttf");
	drawString(devSize.width/2, devSize.height/3, VCENTER | HCENTER, "GAME OVER");

	setRGBA(1, 0, 0, 0);
	fillRect(0, 0, devSize.width, devSize.height);


	drawPopAny(dt);
	drawPopMenu(dt);
	drawPopSetting(dt);
	drawPopHow(dt);
}

bool keyEnd(iKeyState stat, iPoint p)
{
	if (keyPopAny(stat, p)||
		keyPopSetting(stat, p) ||
		keyPopHow(stat, p) ||
		keyPopMenu(stat, p))
		return true;
	return true;
}

