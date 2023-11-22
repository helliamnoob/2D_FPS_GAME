#include "intro.h"

void loadIntro()
{
#if 1
	loadPlayer(true);
	loadEnemy();
	loadBullet();
#endif

	createBg();
	createPopAny();
	showPopAny(true);

	loadMenu();
}

void freeIntro()
{
#if 1
	freePlayer();
	freeEnemy();
	freeBullet();
#endif

	freeBg();
	freePopAny();

	freeMenu();
}

void drawIntro(float dt)
{
#if 1
	drawPlayer(dt, iPointZero);
	drawEnemy(dt, iPointZero);
	drawBullet(dt);
#endif
	if (gs != GameStateProc)
		ply->position = iPointMake(280, 380);
	drawBg(dt);
	drawPopAny(dt);

	drawMenu(dt);
}

bool keyIntro(iKeyState stat, iPoint p)
{
	if (keyPopAny(stat, p)||
		keyMenu(stat, p))
		return true;

	return false;;
}

//배경
Texture* texBg;
void createBg()
{

}

void freeBg()
{
}

void drawBg(float dt)
{
	setRGBA(1, 1, 1, 1);
	fillRect(0, 0, devSize.width, devSize.height);
}


//press any key... 

iPopup* popAny;
iImage* imgAnyString;
float anyDt;

void drawPopAnyBefore(float dt, iPopup* pop);
void createPopAny()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(512, 60);
	g->init(size);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(55);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "Press Any Key");

	Texture* tex = g->getTexture();
	delete g;

	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);
	imgAnyString = img;

	pop->style = iPopupStyleAlpha;
	pop->openPoint = pop->closePoint = iPointMake((devSize.width - size.width) / 2, devSize.height * 0.7f);
	pop->methodDrawBefore = drawPopAnyBefore;
	popAny = pop;

	anyDt = 0.0f;
}

void freePopAny()
{
	delete  popAny;
}

void drawPopAnyBefore(float dt, iPopup* pop)
{
	anyDt += dt;
	imgAnyString->alpha = fabsf(_sin(180 * anyDt));
}

void drawPopAny(float dt)
{
	popAny->paint(dt);
}

bool keyPopAny(iKeyState stat, iPoint p)
{
	if (popAny->bShow == false)
		return false;

	if (stat == iKeyStateBegan)
	{
		showPopAny(false);
		showPopMenu(true);
	}

	return true;
}

void showPopAny(bool show)
{
	popAny->show(show);
}
