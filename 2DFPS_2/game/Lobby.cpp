#include "Lobby.h"


StartLine** stline;
iStrTex* stStart;
void loadLobby()
{
	loadMap();
	iPoint pos[4] = { {100, 200}, {300, 200}, {100, 400}, {400, 400} };
	stline = new StartLine * [4];
	for (int i = 0; i < 4; i++)
		stline[i] = new StartLine(pos[i]);

	loadPlayer(false);
	createPopInventory();
}

void freeLobby()
{
	for (int i = 0; i < 4; i++)
		delete stline[i];
	delete stline;
	delete stStart;

	freeMap();
	freePlayer();
	freePopInventory();

}
extern iPopup* popInven;
extern Bg* bg;
bool checked = true;
void drawLobby(float dt)
{
	drawMap(dt);
	drawPlayer(dt, bg->off);

	for (int i = 0; i < 4; i++)
	{
		StartLine* sl = stline[i];
		sl->paint(dt, bg->off);
		if (sl->touch(iRectMake(ply->position.x - 20, ply->position.y - 20, 40, 40)))
		{
			printf("touch!");
			if (i == 0)
			{
				showPopInventory(true);
			}
			if (i == 3)
				setLoading(GameStateProc, freeLobby, loadProc);

		};
		
	}
	drawPopInventory(dt);

}

bool keyLobby(iKeyState stat, iPoint p)
{
	if (
		keyPopInventory(stat, p) ||
		keyPopSetting(stat, p) ||
		keyPopHow(stat, p))
	{

		return true;
	}
	return false;
}

StartLine::StartLine(iPoint p)
{
	position = p;
	
	img = new iImage();
	Texture* tex = createImage("assets/shop/shop.png");
	img->position = iPointMake(100-25, 200-25);
	img->addObject(tex);
	freeImage(tex);
	
	
	img2 = new iImage();
	Texture* tex2 = createImage("assets/shop/inven.png");
	img2->position = iPointMake(100 - 25, 400 - 25);
	img2->addObject(tex2);
	freeImage(tex2);
	/*
	img3 = new iImage();
	Texture* tex3 = createImage("assets/shop/exit.png");
	img3->position = iPointMake(400 - 25, 400 - 25);
	img3->addObject(tex3);
	freeImage(tex3);
	*/
}

StartLine::~StartLine()
{
}

void StartLine::paint(float dt, iPoint off)
{

	iPoint p = position + off;
	setRGBA(1, 1, 1, 1);
	fillRect(p.x - 25, p.y - 25, 50, 50);

	img->paint(dt, bg->off);
	img2->paint(dt, bg->off);
	//img3->paint(dt, bg->off);
}

bool StartLine::touch(iRect rt)
{
	return containRect(rt, iRectMake(position.x - 25, position.y - 25, 50, 50));
}


