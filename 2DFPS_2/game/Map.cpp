#include "Map.h"

Bg::Bg()
{
	tex = new Texture * [6];
	for (int i = 0; i < 6; i++)
		tex[i] = createImage("assets/bg/bgb%d.png", i);
	position = iPointZero;

	off = iPointZero;
	max = iPointZero;
	min = iPointMake(devSize.width - (tex[0]->width + tex[1]->width),
		devSize.height - (tex[0]->height + tex[2]->height + tex[4]->height));
}

Bg::~Bg()
{
	for (int i = 0; i < 6; i++)
		freeImage(tex[i]);
	delete tex;
}

void Bg::draw(float dt)
{
	setRGBA(1, 1, 1, 1);
	//drawImage(tex, position, TOP | LEFT);
	//drawImage(tex, position.x, position.y, devSize.width / tex->width, devSize.height / tex->height,
	//	TOP | LEFT, 0, 0, tex->width, tex->height, 2, 0);
	for (int i = 0; i < 6; i++)
		drawImage(tex[i], off.x + 640 * (i % 2), off.y + 240 * (i / 2), TOP | LEFT);
}

void Bg::setHeroPostion(iPoint& p)
{
	if (p.x < 10)
		p.x = 10;
	else if (p.x > devSize.width - min.x - 10)
		p.x = devSize.width - min.x - 10;
	if (p.y < 10)
		p.y = 10;
	else if (p.y > devSize.height - min.y - 10)
		p.y = devSize.height - min.y - 10;

	off.x = -p.x + devSize.width / 2;
	off.y = -p.y + devSize.height / 2;

	if (off.x < min.x)
		off.x = min.x;
	else if (off.x > max.x)
		off.x = max.x;

	if (off.y < min.y)
		off.y = min.y;
	else if (off.y > max.y)
		off.y = max.y;

	//Bg::
}
void loadMap()
{
	loadProcBg();
}

void freeMap()
{
	freeProcBg();
}

void drawMap(float dt)
{
	drawProcBg(dt);

}

bool keyMap(iKeyState stat, iPoint p)
{

	return true;
}

Bg* bg;
void loadProcBg()
{
	bg = new Bg();
}
void freeProcBg()
{
	delete bg;
}
void drawProcBg(float dt)
{
	bg->draw(dt);
}

void moveProcBg()
{

}
