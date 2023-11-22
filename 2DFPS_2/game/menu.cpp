#include "menu.h"

enum {
	snd_btn_sel = 0,
	snd_btn_ok,

	snd_eff_max,
	snd_bgm_title = snd_eff_max,
	snd_bgm_map,
	snd_bgm_song,

	snd_max,
};

void loadMenu()
{
	createPopMenu();
	createPopSetting();
	createPopHow();
}

void freeMenu()
{
	freePopMenu();
	freePopSetting();
	freePopHow();
}

void drawMenu(float dt)
{
	drawPopMenu(dt);
	drawPopSetting(dt);
	drawPopHow(dt);
}


bool keyMenu(iKeyState stat, iPoint p)
{
	
	if (keyPopHow(stat, p) ||
		keyPopSetting(stat, p) ||
		keyPopMenu(stat, p))
		return true;
	return true;
}


// 메뉴창
iPopup* popMenu;
iImage** imgMenuBtn;

void drawPopMenuBefore(float dt, iPopup* pop);

void createPopMenu()
{
	iPopup* pop = new iPopup();

	//
	// bg
	//
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(300, 400);
	g->init(size);

	setRGBA(1, 0, 0, 1);
	g->fillRect(0, 0, size.width, size.height, 10);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	// 
	// btn
	//
	const char* strMenu[4] = { "시작","게임방법", "옵션", "종료" };
	size = iSizeMake(200, 50);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	imgMenuBtn = new iImage * [4];
	for (int i = 0; i < 4; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0) setRGBA(1, 1, 0, 1);
			else setRGBA(0, 0, 1, 1);
			g->fillRect(0, 0, size.width, size.height, 10);

			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strMenu[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}

		img->position = iPointMake(size.width-150, 60+80 * i);
		pop->addObject(img);
		imgMenuBtn[i] = img;
	}

	//pop->style = iPopupStyleAlpha;
	//pop->openPoint = iPointMake(devSize.width / 2 - 160, devSize.height / 2 - 160);
	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointMake(devSize.width / 2 -150, devSize.height / 2 -150 );
	pop->methodDrawBefore = drawPopMenuBefore;
	popMenu = pop;
	delete g;
}

void freePopMenu()
{
	delete popMenu;
	delete imgMenuBtn;
}

void drawPopMenuBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 4; i++)
		imgMenuBtn[i]->frame = (i == popMenu->selected);
}

void drawPopMenu(float dt)
{
	popMenu->paint(dt);
}

bool keyPopMenu(iKeyState stat, iPoint p)
{
	if (popMenu->bShow == false)
		return false;
	if (popMenu->state != iPopupStateProc)
		return true;


	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popMenu->selected;
		if (i == -1) break;
		
		if (i == 0)
		{
			printf("start\n");
			//loadProc();
			//gs = GameStateProc;// 아래 셋로딩으로
			setLoading(GameStateLobby, freeMenu, loadLobby);
		}
		else if (i == 1)
		{
			printf("howtoplay\n");
			showPopHow(true);
		}
		else if (i == 2)
		{
			printf("settings\n");
			showPopSetting(true);
			//showPopMenu(false);
		}
		
		else// if( i==3 )
		{
			printf("quit\n");
			exit(1);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 4; i++)
		{
			if (containPoint(p, imgMenuBtn[i]->rect(popMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popMenu->selected)
		{
			printf("audio btn \n");
			popMenu->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;

}

void showPopMenu(bool show)
{
	popMenu->show(true);
}

float rangeBtn[2];
void drawPopSettingBefore(float dt, iPopup* pop);

iPopup* popSetting;
iImage** imgSettingBtn;

void createPopSetting()
{
	iPopup* pop = new iPopup();
	//
	// bg (320, 320) (0, 1, 1, 0.5) 
	//
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 320);
	g->init(size);

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(40);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 10, TOP | HCENTER, "옵션");

	const char* strOption[2] = { "배경음악", "효과음" };
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		g->drawString(120, 160 + 40 * i, VCENTER | RIGHT, strOption[i]);
		g->fillRect(140, 160 - 1 + 40 * i, 140, 2, 1);
	}

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	// 
	// btn
	//
	imgSettingBtn = new iImage * [3];

	// [0] -    [1] - clone
	img = new iImage();
	size = iSizeMake(20, 20);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(1, 1, 1, 0.5f);
		g->fillRect(0, 0, size.width, size.height, size.width / 2);

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(140 - 10, 160 - 10);
	pop->addObject(img);
	imgSettingBtn[0] = img;

	img = img->clone();
	img->position.y += 40;
	pop->addObject(img);
	imgSettingBtn[1] = img;

	rangeBtn[0] = 140 - 10;
	rangeBtn[1] = rangeBtn[0] + 140;

	// [2]
	img = new iImage();
	size = iSizeMake(240, 40);
	for (int j = 0; j < 2; j++)
	{
		g->init(size);

		if (j == 0)
		{
			setRGBA(0.5, 0.5, 0.5, 1);
			setStringRGBA(0, 0, 0, 1);
		}
		else// if (j == 1)
		{
			setRGBA(1, 1, 0, 1);
			setStringRGBA(1, 1, 1, 1);
		}
		g->fillRect(0, 0, size.width, size.height, 5);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "닫기");

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}

	img->position = iPointMake(40, 260);
	pop->addObject(img);
	imgSettingBtn[2] = img;

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;//imgMenuBtn[2]->center(popMenu->closePoint);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 320) / 2);
	pop->methodDrawBefore = drawPopSettingBefore;
	popSetting = pop;
	delete g;
}

void freePopSetting()
{
	delete popSetting;
	delete imgSettingBtn;
}
void drawPopSettingBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		imgSettingBtn[i]->frame = (i == popSetting->selected);
}
void drawPopSetting(float dt)
{

	popSetting->paint(dt);

}

bool settingDrag = false;
iPoint prevPoint;
bool keyPopSetting(iKeyState stat, iPoint p)
{
	if (popSetting->bShow == false)
		return false;
	if (popSetting->state != iPopupStateProc)
		return true;

	int i, j = -1;


	switch (stat) {

	case iKeyStateBegan:
		i = popSetting->selected;
		if (i == -1) break;

		//audioPlay(snd_btn_ok);
		if (i == 0)
		{
			printf("thumb-up\n");
			settingDrag = true;
			prevPoint = p;
		}
		else if (i == 1)
		{
			printf("thumb-down\n");
			settingDrag = true;
			prevPoint = p;
		}
		else// if (i == 2)
		{
			printf("close\n");
			showPopSetting(false);
		}
		break;
	case iKeyStateMoved:
		if (settingDrag)
		{
			iPoint mp = (p - prevPoint);
			prevPoint = p;
			float& x = imgSettingBtn[popSetting->selected]->position.x;
			x += mp.x;
			if (x < rangeBtn[0])		x = rangeBtn[0];
			else if (x > rangeBtn[1])	x = rangeBtn[1];
			float percent = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			printf("%s %.f%%\n",
				popSetting->selected == 0 ? "bgm" : "sfx",
				percent * 100);

			float vol[2];
			for (int i = 0; i < 2; i++)
			{
				float& x = imgSettingBtn[i]->position.x;
				vol[i] = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			}
			audioVolume(vol[0], vol[1], snd_eff_max);
			break;
		}
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgSettingBtn[i]->rect(popSetting->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popSetting->selected)
		{
			printf("audio play btn\n");
			//audioPlay(snd_btn_sel);
			popSetting->selected = j;
		}
		break;

	case iKeyStateEnded:
		if (settingDrag)
		{
			settingDrag = false;
			popSetting->selected = -1;
		}
		break;

	}

	return true;
}

void showPopSetting(bool show)
{
	popSetting->show(show);
}

iPopup* popHow;
iImage** imgHowBtn;

void drawPopHowBefore(float dt, iPopup* pop);

iStrTex* stHow;
Texture* methodStHow(const char* str);

int page, _page;

const char* strHow[5][3] = {
	{
		"0이름 : 아무개",
		"0프로그래머 지망생",
		"0게임 좋아함"
	},
	{
		"1이름 : 아무개",
		"1프로그래머 지망생",
		"1게임 좋아함"
	},
	{
		"2이름 : 아무개",
		"2프로그래머 지망생",
		"2게임 좋아함"
	},
	{
		"3이름 : 아무개",
		"3프로그래머 지망생",
		"3게임 좋아함"
	},
	{
		"4이름 : 아무개",
		"4프로그래머 지망생",
		"4게임 좋아함"
	},
};

Texture* methodStHow(const char* str)
{
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 320);
	g->init(size);

	int currPage = atoi(str);
	if (currPage == 0)
	{

	}

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);
	setRGBA(0, 1, 0, 0.5f);
	g->fillRect(20, 50, size.width - 40, size.height - 55, 10);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(40);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 10, TOP | HCENTER, "게임 방법");

	setStringSize(20);
	int index = atoi(str);
	const char** s = strHow[index];
	for (int i = 0; i < 3; i++)
		g->drawString(size.width / 2, 50 + 25 * i, TOP | HCENTER, s[i]);

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

void createPopHow()
{
	iPopup* pop = new iPopup();

	iStrTex* st = new iStrTex(methodStHow);
	st->setString("0");
	stHow = st;

	iImage* img = new iImage();
	img->addObject(st->tex);
	pop->addObject(img);

	page = 0;
	_page = 5;

	const char* str[3] = { "이전", "닫기", "다음" };

	float stringSize = 30;
	setStringSize(stringSize);
	setStringBorder(0);
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(90, 40);

	imgHowBtn = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
				setStringRGBA(0, 0, 0, 1);
			}
			else// if (j == 1)
			{
				setRGBA(1, 1, 0, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			g->fillRect(0, 0, size.width, size.height, 5);
#if 1
			setStringSize(stringSize);
			iRect rt = rectOfString(str[i]);
			if (rt.size.width > 86)
				setStringSize(stringSize * 86 / rt.size.width);
#endif
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str[i]);

			Texture* tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}

		img->position = iPointMake(10 + 100 * i, 270);
		pop->addObject(img);
		imgHowBtn[i] = img;
	}
	delete g;


	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;//imgMenuBtn[1]->center(popMenu->closePoint);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 320) / 2);
	pop->methodDrawBefore = drawPopHowBefore;
	popHow = pop;
}
void drawPopHowBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		imgHowBtn[i]->frame = (i == popHow->selected);
}

void freePopHow()
{
	delete popHow;
	delete imgHowBtn;
}
void drawPopHow(float dt)
{
	setRGBA(1, 1, 1, 1);
	popHow->paint(dt);
}
bool keyPopHow(iKeyState stat, iPoint p)
{
	if (popHow->bShow == false)
		return false;
	if (popHow->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popHow->selected;
		if (i == -1) break;

		if (i == 0)
		{
			printf("이전\n");
			page--;
			if (page < 0)
				page = _page - 1;
			stHow->setString("%d", page);
		}
		else if (i == 1)
		{
			printf("닫기\n");
			showPopHow(false);
		}
		else// if (i == 2)
		{
			printf("다음\n");
			page++;
			if (page == _page)
				page = 0;
			stHow->setString("%d", page);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgHowBtn[i]->rect(popHow->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popHow->selected)
		{
			printf("audio btn \n");
			popHow->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

void showPopHow(bool show)
{
	popHow->show(show);
}

///////////////////////////////////////////////////////////////////////////////////
////// 종료 팝업
//////////////////////////////////////////////////////////////////////////////////


iPopup* popPopExit;
iImage** imgPopExitBtn;
void createPopExit()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(200, 100);
	g->init(size);

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(40);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 10, TOP | HCENTER, "종료하시겠습니까?");

	const char* strOption[2] = { "네","아니오" };
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		g->drawString(120, 160 + 40 * i, VCENTER | RIGHT, strOption[i]);
		g->fillRect(140, 160 - 1 + 40 * i, 140, 2, 1);

	}

}

void freePopExit()
{
	delete popPopExit;
	delete imgPopExitBtn;

}

void drawPopexit(float dt)
{


}

bool keyPopExit(iKeyState stat, iPoint p)
{
	return false;
}

void showPopExit(bool show)
{


}
