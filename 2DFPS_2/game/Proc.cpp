#include "Proc.h"
#include "Lobby.h"

Goal* goal;
void loadProc()
{
	lootItem = new LootItem();
	goal = new Goal(iPointMake(650, 550));

	loadMap();
	loadPlayer(false);
	loadEnemy();
	loadBullet();
	loadDropItem();
	
	createPopStopMenu();
	createPopResult();
	createPopHow();
	createPopSetting();

	loadProcUI();
	createPopInventory();

}

void freeProc()
{
	delete lootItem;
	delete goal;

	freeMap();
	freePlayer();
	freeEnemy();
	freeBullet();
	freeDropItem();

	freePopStopMenu();
	freePopResult();
	freePopSetting();
	freePopHow();


	freeProcUI();
	freePopInventory();
}
extern iPopup* popInven;
extern Bg* bg;
void drawProc(float dt)
{
	float _dt = dt;
	if (popStopMenu->bShow||
		popInven->bShow ||
		popResult->bShow )
	{
		dt = 0;
	}	
	drawMap(dt);
	drawPlayer(dt, bg->off);
	drawEnemy(dt, bg->off);
	drawBullet(dt);
	drawDropItem(dt,bg->off);
	
	goal->paint(dt, bg->off);

	if (goal->touch(iRectMake(ply->position.x - 20, ply->position.y - 20, 40, 40)))
	{
		printf("ss");
		//setLoading(GameStateEnding, freeProc, loadEnd);
#if 0
		showPopResult(true);
#else
		if( popResult->bShow==false)
			popResult->show(true);
#endif
	};

	drawPopStopMenu(_dt);
	drawPopResult(_dt);
	drawPopSetting(_dt);
	drawPopHow(_dt);
	drawPopInventory(_dt);
	

	drawProcUI(dt);

}

bool keyProc(iKeyState stat, iPoint p)
{
	if (
		keyPopInventory(stat, p) ||
		keyPopSetting(stat, p) ||
		keyPopHow(stat, p) ||
		keyPopStopMenu(stat, p) ||
		keyPopResult(stat, p) ||
		keyProcUI(stat, p))
	{

		return true;
	}
	return true;
	
}

//////////////////////////////////////////////////////////////////
// Player
//////////////////////////////////////////////////////////////////
Player* ply;

typedef void (*MethodPlayer)(float dt, iPoint off);
MethodPlayer methodPlayer;

void autoPlayer(float dt, iPoint off);
void ctrlPlayer(float dt, iPoint off);

void loadPlayer(bool autoplay)
{
	ply = new Player();
	methodPlayer = autoplay ? autoPlayer : ctrlPlayer;
}

void freePlayer()
{
	delete ply;

}
extern iPopup* popInven;
extern void showPopInventory(bool show);
extern Bg* bg;

void drawPlayer(float dt, iPoint off)
{
	ply->draw(dt, off);
	//autoPlayer(dt);
	//ctrlPlayer(dt);
	methodPlayer(dt, off);
}

void ctrlPlayer(float dt, iPoint off)
{
	iPoint v = iPointZero;

	if (getKeyStat(keyboard_a))
		v.x = -1.0f;
	else if (getKeyStat(keyboard_d))
		v.x = +1.0f;
	if (getKeyStat(keyboard_w))
		v.y = -1.0f;
	else if (getKeyStat(keyboard_s))
		v.y = +1.0f;
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		ply->setMove(v, dt);
		bg->setHeroPostion(ply->position);
	}

	v = iPointZero;
	if (getKeyStat(keyboard_left))
		v.x = -1.0f;
	else if (getKeyStat(keyboard_right))
		v.x = +1.0f;
	if (getKeyStat(keyboard_up))
		v.y = -1.0f;
	else if (getKeyStat(keyboard_down))
		v.y = +1.0f;
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		ply->setDirection(v, dt);
		ply->fire = true;
	}
	else
		ply->fire = false;



	if (getKeyStat(keyboard_enter))
	{
		showPopInventory(true);
		//if (true)
	//		showPopInventory(true);
		//else
		//	showPopInventory(false);
//
	}	
}

Enemy* menuPlayerEnemy = NULL;
float menuPlayerDt = 0.0f;

void autoPlayer(float dt, iPoint off)
{
	// ai 적을 찾아 움직이도록 ...

	if (menuPlayerEnemy)
	{
		// 쫓아가면서, 공격을 함 !!!!!!!!!!!!!!!!!!!!!!1
		iPoint v = menuPlayerEnemy->position - ply->position;
		v /= iPointLength(v);
		ply->setDirection(v, dt);
		ply->fire = true;
		
		// 죽었거나,, 화면에서 벗어났을 경우,, menuPlayerEnemy = NULL !!!!!!!!!!!!!!!!!!!!!!1
		if (menuPlayerEnemy->alive == false)
		{
			menuPlayerEnemy = NULL;
			ply->fire = false;
			return;
		}
		iPoint p = menuPlayerEnemy->position + off;
		iRect rt = iRectMake(-30, -30, devSize.width + 60, devSize.height + 60);
		if (containPoint(p, rt) == false)
		{
			menuPlayerEnemy = NULL;
			ply->fire = false;
		}
	}
	else// if (menuPlayerEnemy == NULL)
	{
		// 가만있기

		// 몇초가 지나면 명령 하달
		menuPlayerDt += dt;
		if (menuPlayerDt > 0.5f)
		{
			menuPlayerDt = 0;

			// menuPlayerEnemy; 제일 가까운 놈....!!!!!!!!!!!!!!!!!!!!!!1
			Enemy* e = NULL;
			float distance = 0xffffff;
			for (int i = 0; i < enemyNum; i++)
			{
				float d = iPointLength(ply->position - enemy[i]->position);
				if (distance > d)
				{
					distance = d;
					e = enemy[i];
				}
			}
			menuPlayerEnemy = e;
		}
	}
}

Goal::Goal(iPoint p)
{
	position = p;
}
Goal::~Goal()
{

}
void Goal::paint(float dt, iPoint off)
{
	iPoint p = position + off;
	setRGBA(1, 1, 1, 1);
	fillRect(1200, p.y, 50, 50);

}
bool Goal::touch(iRect rt)
{
	return containRect(rt, iRectMake(position.x - 25, position.y - 25, 50, 50));
}


Player::Player()
{
	position = iPointMake(20, 20);
	direction = 0;
	moveSpeed = 300;
	_rate = 0.5f;
	rate = 0.0f;
	hp = 100;
	_hp = 100;
	bhp = 100;
	_bhp = 100;

	ap = 20;
	dp = 20;
	alive = true;


	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(60, 60);

	img = new iImage();
	for (int i = 0; i < 4; i++)
	{
		g->init(size);

		setRGBA(1, 1, 1, 1);
		g->fillRect(10, 10, 40, 40, 20);

		igImage* ig = g->createIgImage("assets/msm/msm00.png");
		g->drawIgImage(ig, size.width / 2, size.height / 2-10, VCENTER | HCENTER);
		g->freeIgImage(ig);

		igImage* gg = g->createIgImage("assets/gun/gun2.png");
		g->drawIgImage(gg, size.width / 2+10, size.height / 2, VCENTER | HCENTER);
		g->freeIgImage(gg);


		//setLineWidth(2);
		//setRGBA(1, 0, 0, 1);
		//if (i == 0)			g->drawLine(30, 30, 50, 30);
		//else if (i == 1)	g->drawLine(30, 30, 55, 30);
		//else if (i == 2)	g->drawLine(30, 30, 60, 30);
		//else				g->drawLine(30, 30, 55, 30);
		//setRGBA(1, 1, 1, 1);

		Texture* tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	delete g;
	//img->position = iPointMake(-30, -30);
	img->anc = VCENTER | HCENTER;
	img->startAnimation();
}

Player::~Player()
{
	//freeImage(tex);
	delete img;
}

void Player::setMove(iPoint v, float dt)
{
	position += v * moveSpeed * dt;
}

void Player::setDirection(iPoint v, float dt)
{
	float tDirection = iPointAngle(iPointMake(1, 0), iPointZero, v);
	float diff = tDirection - direction;
	if (diff > 360) diff -= 360;
	if (diff > 180) diff -= 360;

	float d;
	if (diff > 0)
	{
		d = 360 * dt;

		if (d > diff)
			d = diff;
	}
	else
	{
		d = -360 * dt;
		if (d < diff)
			d = diff;
	}
	direction += d;
}

void Player::draw(float dt, iPoint off)
{
	//drawImage(tex, position, VCENTER | HCENTER);

	//setRGBA(1, 0, 0, 1);
	//setLineWidth(2);
	//iPoint tPosition = iPointRotate(iPointMake(position.x + 30, position.y), position, 360 - direction);
	//drawLine(position, tPosition);
	//printf("(%f, %f)(%f, %f)\n", position.x, position.y, tPosition.x, tPosition.y);
//	drawImage(tex, position.x, position.y, 1, 1, VCENTER | HCENTER,
//		0, 0, tex->width, tex->height, 2, 360-direction, REVERSE_NONE);
	
	if (alive == true)
	{
		img->degree = 360 - direction;
		img->paint(dt, position + off);
	}
	else
	{
		setLoading(GameStateEnding, freeProc, loadEnd);
	}
	setRGBA(1, 1, 1, 1);
	if (fire == false)
		return;


	rate += dt;
	while (rate > _rate)
	{
		rate -= _rate;
		// 총알 방향
		iPoint v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - direction);
		// 총알 발사...
		addBullet(this, position, v);
	}

}

iRect Player::rect()
{
	return iRectMake(position.x - 20, position.y - 20, 40, 40);
}

//////////////////////////////////////////////////////////////////
// Enemy
//////////////////////////////////////////////////////////////////
Enemy** _enemy;
Enemy** enemy;
Enemy* enemys;
int enemyNum;
#define _enemyNum 100

void loadEnemy()
{
	_enemy = new Enemy * [_enemyNum];

	//for (int i = 0; i < _enemyNum; i++)
		//_enemy[i] = new Enemy * [_enemyNum];
	for (int i = 0; i < _enemyNum; i++)
		_enemy[i] = new Enemy();

	enemy = new Enemy * [_enemyNum];
	enemyNum = 0;

#if 0
	enemy = new Enemy();
	enemy->position = iPointMake(50 + rand() % (int)(devSize.width - 100), -50);
	enemy->direction = 0;
	enemy->moveSpeed = 100;
	// 직진으로 내려감
	enemy->v = iPointMake(0, 1);
	enemy->rangeFind = 100;
	enemy->rangeAttack = 80;
#endif
}

void freeEnemy()
{
	for (int i = 0; i < _enemyNum; i++)
		delete _enemy[i];
	delete _enemy;

	delete enemy;

}

void drawEnemy(float dt, iPoint off)
{
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->draw(dt, off);
		if (enemy[i]->alive == false)
		{
			enemyNum--;
			enemy[i] = enemy[enemyNum];
			i--;
		}
	}
	static float testDt = 0.0f;
	testDt += dt;
	if (testDt > 2.0f)
	{
		testDt = 0.0f;
		int x = 50 + rand() % (int)(devSize.width - 100);
		addEnemy(iPointMake(x, -50), dt);
	}
}

void addEnemy(iPoint p, float dt)
{
	for (int i = 0; i < _enemyNum; i++)
	{
		if (_enemy[i]->alive == false)
		{
			//[i]->draw(dt);
			_enemy[i]->alive = true;
			_enemy[i]->position = p;
			_enemy[i]->rate = 0.0f;

			enemy[enemyNum] = _enemy[i];
			enemyNum++;
			return;
		}
	}
	printf("%d", enemyNum);
}

Enemy::Enemy()
{
	alive = false;
	fire = false;
	position = iPointMake(50 + rand() % (int)(devSize.width - 100), -50);
	direction = 0;
	moveSpeed = 100;
	// 직진으로 내려감
	v = iPointMake(0, 1);
	rangeFind = 100;
	rangeAttack = 80;
	ai = methodEnemyAI0;
	_rate = 0.9f;
	hp = 10;
	_hp = 10;
	ap = 1;
	dp = 10;


	/*
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(40, 40);
	g->init(size);

	igImage* ig = g->createIgImage("assets/msm/msm00.png");
	g->drawIgImage(ig, size.width / 2, size.height / 2 - 10, VCENTER | HCENTER);
	g->freeIgImage(ig);

	igImage* gg = g->createIgImage("assets/gun/gun2.png");
	g->drawIgImage(gg, size.width / 2 + 10, size.height / 2, VCENTER | HCENTER);
	g->freeIgImage(gg);

	tex = g->getTexture();
	delete g;
	*/

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(60, 60);

	img = new iImage();
	for (int i = 0; i < 4; i++)
	{
		g->init(size);

		setRGBA(1, 1, 1, 1);
		g->fillRect(10, 10, 40, 40, 20);

		setRGBA(1, 0, 0, 1);
		igImage* ig = g->createIgImage("assets/msm/msm00.png");
		g->drawIgImage(ig, size.width / 2, size.height / 2 - 10, VCENTER | HCENTER);
		g->freeIgImage(ig);

		igImage* gg = g->createIgImage("assets/gun/gun2.png");
		g->drawIgImage(gg, size.width / 2 + 10, size.height / 2, VCENTER | HCENTER);
		g->freeIgImage(gg);


		//setLineWidth(2);
		//setRGBA(1, 0, 0, 1);
		//if (i == 0)			g->drawLine(30, 30, 50, 30);
		//else if (i == 1)	g->drawLine(30, 30, 55, 30);
		//else if (i == 2)	g->drawLine(30, 30, 60, 30);
		//else				g->drawLine(30, 30, 55, 30);
		//setRGBA(1, 1, 1, 1);

		Texture* tex = g->getTexture();
		img->addObject(tex);
		
	}
	delete g;
	//img->position = iPointMake(-30, -30);
	img->anc = VCENTER | HCENTER;
	img->startAnimation();
}
Enemy::~Enemy()
{

	delete img;
}

void Enemy::setDirection(iPoint v, float dt)
{
	float tDirection = iPointAngle(iPointMake(1, 0), iPointZero, v);
	float diff = tDirection - direction;
	if (diff > 360) diff -= 360;
	if (diff > 180) diff -= 360;

	float d;
	if (diff > 0)
	{
		d = 360 * dt;

		if (d > diff)
			d = diff;
	}
	else
	{
		d = -360 * dt;
		if (d < diff)
			d = diff;
	}
	direction += d;
}

void Enemy::draw(float dt, iPoint off)
{
	
	//drawImage(tex, tPosition + off, VCENTER | HCENTER);
	ai(this, dt, off);
	
	//setRGBA(1, 1, 0, 1);
	//fillRect(position.x - 20, position.y - 20, 40, 40, 20);

	
	/*setRGBA(1, 0, 0, 1);
	setLineWidth(2);
	iPoint tPosition = iPointRotate(iPointMake(position.x + 30, position.y), position, 360 - direction);
	//drawLine(position+off, tPosition+off);
	drawImage(tex, tPosition + off, VCENTER | HCENTER);

	setRGBA(1, 1, 1, 1);
	*/

	if (alive == true)
	{
		img->degree = 360 - direction;
		img->paint(dt, position + off);
	}
	else
	{
		
	}
	//setRGBA(1, 1, 1, 1);



	rate += dt;
	while (rate > _rate)
	{
		rate -= _rate;
		// 총알 방향
		iPoint v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - direction);
		// 총알 발사...
		addBullet(this, position, v);
	}




	if (fire == false)
		return;


}

bool Enemy::canAttack(float dt)
{
	rate += dt;
	if (rate > _rate)
	{
		rate -= _rate;
		iPoint v = iPointRotate(iPointMake(1, 0), iPointZero, 360 - direction);

		addBullet(this, position, v);
	}
	return false;
}


iRect Enemy::rect()
{
	return iRectMake(position.x - 20, position.y - 20, 40, 40);
}



void methodEnemyAI0(Enemy* e, float dt, iPoint off)
{
	iPoint v = ply->position - e->position;
	float distance = iPointLength(v);
	if (distance < e->rangeFind)
	{
		if (distance < e->rangeAttack)
			;// attack

		// 쫓아감
		e->v = v / distance;
	}

	e->position += e->v * (e->moveSpeed * dt);
#if 1// 
	if (iPointLength(e->position - ply->position) < 40)// r 20, r 20
	{
		e->position = ply->position - e->v * 40;
	}
#endif
	e->setDirection(v, dt);
	e->fire = true;


	if (e->position.y+ off.y > devSize.height + 20)
		e->alive = false;

	if (e->canAttack(dt))
	{
		e->fire = true;

		//addBullet(e->position, v);

	}
}


//////////////////////////////////////////////////////////////////
// Bullet
//////////////////////////////////////////////////////////////////
Bullet** _b;
Bullet** b;
int bNum;
#define _bNum 100

void loadBullet()
{
	_b = new Bullet * [_bNum];
	for (int i = 0; i < _bNum; i++)
		_b[i] = new Bullet();

	b = new Bullet * [_bNum];
	bNum = 0;
}
void freeBullet()
{
	for (int i = 0; i < _bNum; i++)
		delete _b[i];
	delete _b;

	delete b;
}

void drawBullet(float dt)
{
	for (int i = 0; i < bNum; i++)
	{
		b[i]->draw(dt);
		if (b[i]->alive == false)
		{

			bNum--;
			b[i] = b[bNum];
			i--;
		}
	}
}

void addBullet(Object* own, iPoint p, iPoint v)
{
	addBullet(own, p, v, v);
#if 0
	for (int i = 0; i < _bNum; i++)
	{
		if (_b[i]->alive == false)
		{
			_b[i]->setShoot(p, v);

			b[bNum] = _b[i];
			bNum++;
			return;
		}
	}
	printf("not enought bullet\n");
#endif
}

void addBullet(Object* own, iPoint sp, iPoint tp, iPoint v)
{
	if (gs != GameStateProc)
		return;

	for (int i = 0; i < _bNum; i++)
	{
		if (_b[i]->alive == false)
		{
			_b[i]->alive = true;
			_b[i]->p = sp;
			iPoint z = tp - sp;
			z /= iPointLength(z);
			_b[i]->v = z;
			_b[i]->setShoot(sp, v);
			_b[i]->own = own;

			b[bNum] = _b[i];
			bNum++;
			return;
		}
	}
}

Bullet::Bullet()
{
	alive = false;
	index = 0;
	p = iPointZero;
	v = iPointZero;
	speed = 300;

	img = new iImage();
	Texture* tex = createImage("assets/bullet/bull.png");
	img->addObject(tex);
	freeImage(tex);

	img->position = iPointMake(-tex->width / 2, -tex->height / 2);


}

bool Bullet::check()
{
	for (int i = 0; i < bNum; i++)
	{
		Bullet* bu = b[i];
		if (own == ply)
		{

			Enemy* e = NULL;
			float distance = 0xffffff;
			for (int i = 0; i < enemyNum; i++)
			{
				if (containPoint(p, enemy[i]->rect()))
				{
					float d = iPointLength(p - enemy[i]->position);
					if (distance > d)
					{
						distance = d;
						e = enemy[i];
					}
				}
			}

			if (e)
			{
				printf("=====\n");
				bu->alive = false;
				int dmg = bu->own->ap * (90 + rand() % 20) / 100.f - e->dp;
				e->hp -= dmg;
				if (e->hp <= 0)
				{
					e->alive = false;
					addDropItem(e->position);
				}
				return true;
			}

		}
		else
		{
			if (containPoint(p, ply->rect()))
			{
				bu->alive = false;
				int dmg = bu->own->ap * (90 + rand() % 20) / 100.f - ply->dp;
				if (dmg < 1)
					dmg = 1;


				printf("****\n");
				ply->hp -= dmg;
				printf("%d", dmg);

				if (ply->hp <= 0)
					ply->alive = false;

				return true;
			}
		}


		return false;

	}
}
void Bullet::draw(float dt)
{
	p += v * (speed * dt);

	if (containPoint(p+bg->off, iRectMake(-20, -20, devSize.width + 40, devSize.height + 40)) == false ||
		check())
		alive = false;

	//setRGBA(1, 0, 0, 1);
	//fillRect(p.x - 5, p.y - 5, 10, 10, 5);
	//setRGBA(1, 1, 1, 1);
	img->paint(dt,p + bg->off);
}
void Bullet::setShoot(iPoint p, iPoint v)
{
	alive = true;
	this->p = p;
	this->v = v;

	img->degree = 360-iPointAngle(iPointMake(1, 0), iPointZero, v);
}

iRect Bullet::rect()
{
	return iRectMake(-20, -20, devSize.width + 40, devSize.height + 40);
}


//////////////////////////////////////////////////////////////////
// ProcUI cpp/h
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// ProcUI cpp/h
//////////////////////////////////////////////////////////////////
iPopup* popStop;
iImage** imgStopBtn;
void drawProcUIBefore(float dt, iPopup* pop);

iStrTex* stCountTime;
float countTime;

void loadProcUI()
{
	iPopup* pop = new iPopup();

	imgStopBtn = new iImage * [1];
	const char* strBtn[1] = { "정 지", };
	iPoint posBtn[1] = { {1200,10}, };

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(60, 30);
	iImage* img = new iImage();

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(10);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);

	for (int i = 0; i < 2; i++)
	{
		//팝업처리
		//일시정지 버튼 ,남은시간 표시, HP바표시, 
		g->init(size);
		if (i == 0)
		{
			setRGBA(1, 1, 0, 1);
		}
		else
		{
			setRGBA(1, 0, 1, 1);
		}
		g->fillRect(0,0,size.width,size.height,10);
		g->drawString(size.width/2,size.height/2, VCENTER | HCENTER, strBtn[0]);

		Texture* tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);

	}
		setRGBA(1, 1, 1, 1);
		img->position = posBtn[0];
		pop->addObject(img);
		imgStopBtn[0] = img;

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = drawProcUIBefore;
	popStop = pop;

	delete g;

	showProcUI(true);

	stCountTime = new iStrTex();
	countTime = 30.f;
}
void freeProcUI()
{
	delete popStop;
	delete imgStopBtn;
	delete stCountTime;
}
void drawProcUIBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 1; i++)
	{
		imgStopBtn[i]->frame = (i == popStop->selected);
	}
}
void drawProcUI(float dt)
{
	stCountTime->drawString(510, 25, VCENTER | RIGHT , "%.0f", countTime);
#if 1
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setStringName("assets/BMJUA_ttf.ttf");
	//drawString(120, 20, VCENTER | LEFT, "%d", endTimer());
	if (countTime > 0)
	{
		countTime -= dt;
		if (countTime <= 0)
		{
			countTime = 0.0f;
			// gamover
			//setLoading(GameStateEnding, freeProc, loadEnd);
		}
	}
#endif
	popStop->paint(dt);
}
bool keyProcUI(iKeyState stat, iPoint p)
{
	if (popStop->bShow == false)
		return false;
	if (popStop->state != iPopupStateProc)
		return true;
	
	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popStop->selected;
		if (i == -1) break;

		if (i == 0)
		{
			showPopStopMenu(true);
			printf("menu\n");
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 1; i++)
		{
			if (containPoint(p, imgStopBtn[i]->rect(popStop->closePoint)))
			{
				j = i;
				break;
			}
	}
	if (j != popStop->selected)
	{
		printf("audio btn \n");
		popStop->selected = j;
	}
	break;
		break;
	case iKeyStateEnded:
		break;
	}
	return true;
}
void showProcUI(bool show)
{
	popStop->show(show);
}



//////////////////////////////////////////////////////////////
// stopmenu
//////////////////////////////////////////////////////////////

void drawPopStopMenuBefore(float dt, iPopup* pop);
iPopup* popStopMenu;
iImage** imgStopMenuBtn;
void createPopStopMenu()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(210, 210);
	g->init(size);

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);
	//////
	//btn
	//////
	const char* strMenu[4] = { "게임재개","게임방법", "옵션", "종료" };
	size = iSizeMake(120, 40);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	imgStopMenuBtn = new iImage * [4];
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

		img->position = iPointMake(40, 10 + 50 * i);
		pop->addObject(img);
		imgStopMenuBtn[i] = img;
	}

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointMake(devSize.width / 2 - 100, devSize.height / 2 - 100);
	pop->methodDrawBefore = drawPopStopMenuBefore;
	popStopMenu = pop;
	delete g;
}
void freePopStopMenu()
{
	delete popStopMenu;
	delete imgStopMenuBtn;
}
void drawPopStopMenuBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 4; i++)
		imgStopMenuBtn[i]->frame = (i == popStopMenu->selected);
}
void drawPopStopMenu(float dt)
{
	popStopMenu->paint(dt);
}
bool keyPopStopMenu(iKeyState stat, iPoint p)
{
	if (popStopMenu->bShow == false)
		return false;
	if (popStopMenu->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popStopMenu->selected;
		if (i == -1) break;

		if (i == 0)
		{
			printf("continue\n");
			showPopStopMenu(false);
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
			if (containPoint(p, imgStopMenuBtn[i]->rect(popStopMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popStopMenu->selected)
		{
			printf("audio btn \n");
			popStopMenu->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}
void showPopStopMenu(bool show)
{
	popStopMenu->show(show);
}
///////////////////////////
////결과창
/////////////////////////////
void drawPopResultBefore(float dt, iPopup* pop);
iPopup* popResult;
iImage** imgPopResultBtn;
void createPopResult()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(210, 210);
	g->init(size);

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);
	/// 
	/// BTN
	/// 
	const char* strMenu[2] = { "다음","종료" };
	size = iSizeMake(90, 40);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	imgPopResultBtn = new iImage * [2];
	for (int i = 0; i < 2; i++)
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
		img->position = iPointMake(40, 10 + 50 * i);
		pop->addObject(img);
		imgPopResultBtn[i] = img;
	}

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = drawPopResultBefore;
	popResult = pop;
	delete g;
}
void freePopResult()
{
	delete popResult;
	delete imgPopResultBtn;
}
void drawPopResultBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 2; i++)
		imgPopResultBtn[i]->frame = (i == popResult->selected);
}
void drawPopResult(float dt)
{
	popResult->paint(dt);
}
bool keyPopResult(iKeyState stat, iPoint p)
{
	if(popResult->bShow == false)
		return false;
	if (popResult->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popResult->selected;
		if (i == -1) break;

		if (i == 0)
		{
			printf("continue\n");
			setLoading(GameStateLobby, freeProc, loadLobby);
		}
		else
		{
			printf("Quit\n");
			exit(1);
		}

		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgPopResultBtn[i]->rect(popResult->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popResult->selected)
		{
			printf("audio btn \n");
			popResult->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}
	return true;
}
void showPopResult(bool show)
{
	popResult->show(show);
}



///////////////////////////
////아이템
/////////////////////////////

#if 0

Item item[5] = {
	{ "hp", 7 },
	{ "weapon", 1 },
	{ "stone", 1 },
};
#endif

DropItem::DropItem()
{
	alive = false;
	index = 0;
	tex = createImage("assets/item/item.jpg");
	position = iPointZero;
}

DropItem::~DropItem()
{
	freeImage(tex);
}


bool DropItem::draw(float dt, iPoint off)
{
	iPoint p = position + off;
	drawImage(tex, p,VCENTER|HCENTER);

	if (iPointLength(ply->position - position) < 10)
	{
		lootItem->add(index,1);
		addInvetory(index,1);
		delta = 0;
	}

	delta -= dt;
	if (delta < 0)
	{
		delta = 0;
		alive = false;
	}
	return !alive;
}
void DropItem::set(iPoint p)
{
	alive = true;
	position = p;
	delta = 5.0f;
}


DropItem** _dropItem;
DropItem** dropItem;
DropItem* item;
int dropItemNum;
void loadDropItem()
{
	_dropItem = new DropItem*[10];
	for (int i = 0; i < 10; i++)
	{
		_dropItem[i] = new DropItem();
	}

	dropItem = new DropItem * [10];
	dropItemNum = 0;
}
void freeDropItem()
{
	for (int i = 0; i < 10; i++)
		delete _dropItem[i];
	delete _dropItem;
	delete dropItem;
}

void drawDropItem(float dt, iPoint off)
{
	for (int i = 0; i < dropItemNum; i++)
	{
		if (dropItem[i]->draw(dt, off))
		{
			dropItemNum--;
			dropItem[i] = dropItem[dropItemNum];
			i--;
		}
	}
}
void addDropItem(iPoint p)
{
	for (int i = 0; i < 10; i++)
	{
		if (_dropItem[i]->alive == false)
		{
			_dropItem[i]->set(p);

			dropItem[dropItemNum] = _dropItem[i];
			dropItemNum++;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////
///// Loot Item
//////////////////////////////////////////////////////////////////
LootItem* lootItem;

///////////////////////
//인벤
///////////////////////
Slot* slot;			// 3(equip) + 30(inventory)
iImage** imgSlot;
iStrTex*** stSlot;	// slot x 2 (index, grade, num)
Texture* methodStSlot(const char* str);

int selectedItem, selectingItem;
bool draggingItem;
iPoint draggingPoint, originPoint;

iPopup* popInven;
void draPopInventoryBefore(float dt, iPopup* pop);
void draPopInventoryAfter(float dt, iPopup* pop);
iImage** imgInventoryBtn;

void createPopInventory()
{
	iPopup* pop = new iPopup();
	//
	// bg (320, 320) (0, 0, 1, 0.5) 
	//
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(512, 320);

	g->init(size);

	setRGBA(0, 0, 1, 0.5f);
	g->fillRect(0, 0, size.width, size.height, 10);

	igImage* ig = g->createIgImage("assets/msm/man2.png");
	g->drawIgImage(ig, size.width/3, size.height / 2 -10 , VCENTER | RIGHT);
	g->freeIgImage(ig);

	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(40);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 10, TOP | HCENTER, "인벤토리");

	//hp바
	setRGBA(0.5, 0.5, 0.5, 1.0);
	g->fillRect(60,10, 100, 20);

	setRGBA(1, 1, 0, 1.0);
	float r = ply->hp / ply->_hp;
	float w = 100 - 2;
	w *= r;
	if (ply->hp > 0)
		g->fillRect(60, 10 + 1, w, 20 - 2);
	else if (ply->hp < 0)
		g->fillRect(60 + 1, 10 + 1, 0, 20 - 2);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setStringName("assets/BMJUA_ttf.ttf");
	if (ply->hp > 0)
		g->drawString(120, 20, VCENTER | LEFT, "%.f%%", r * 100);
	else
		g->drawString(120, 20, VCENTER | LEFT, "0");
	
	//bhp
	setRGBA(0.5, 0.5, 0.5, 1.0);
	g->fillRect(60, 100, 100, 20);

	setRGBA(1, 1, 0, 1.0);
	float br = ply->bhp / ply->_bhp;
	float bw = 100 - 2;
	bw *= br;
	if (ply->bhp > 0)
		g->fillRect(60, 100 + 1, bw, 20 - 2);
	else if (ply->bhp < 0)
		g->fillRect(60 + 1, 100 + 1, 0, 20 - 2);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setStringName("assets/BMJUA_ttf.ttf");
	if (ply->bhp > 0)
		g->drawString(120, 110, VCENTER | LEFT, "%.f%%", br * 100);
	else
		g->drawString(120, 110, VCENTER | LEFT, "0");


	// 6 x 5
	
	setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	
	int offX = ((size.width - itemW * itemX)*4) / 5;
	int offY = 60;
	
	for (int i = 0; i < 7; i++)
		g->drawLine(offX + itemW * i, offY, offX + itemW * i, offY + itemH * 5);
	for (int i = 0; i < 6; i++)
		g->drawLine(offX, offY + itemH * i, offX + itemW * 6, offY + itemH * i);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);
	
	//
	// btn (240, 60) 
	//
	const char* str[3] = { "오름차순", "내림차순", "닫기" };

	float stringSize = 30;
	setStringSize(stringSize);
	setStringBorder(0);
	size = iSizeMake(90, 40);

	imgInventoryBtn = new iImage * [3];
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

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}

		img->position = iPointMake(160 + 100 * i, 270);
		pop->addObject(img);

		imgInventoryBtn[i] = img;
	}
	
	slot = new Slot[33];
	memset(slot, 0x00, sizeof(Slot) * 33);
	imgSlot = new iImage*[33];
	stSlot = new iStrTex**[33];
	//
	// Slot (Equip) : [0~2]
	//
	iPoint posITemb[3] = {
		{50 + itemW, 50 + 0},
		{50 + itemW, 50 + 80 },
		{50 + itemW, 50 + 160 },
	};
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();

		stSlot[i] = new iStrTex*[2];
		Slot* s = &slot[i];

		for (int j = 0; j < 2; j++)
		{
			iStrTex* st = new iStrTex(methodStSlot);
			st->setString("%d/%d/%d/%d", s->index, s->grade, s->num, j);

			img->addObject(st->tex);
			stSlot[i][j] = st;
		}
		img->position = posITemb[i];
		pop->addObject(img);
		imgSlot[i] = img;
	}

	//
	// Slot (Inventory)
	//
#if 1// load from save file....

	// equip
	Slot* sl = &slot[1];
	sl->index = 2;
	sl->grade = 2;
	sl->num = 4;

	// inventory
	Slot* inven = &slot[6];
	inven->index = 1;
	inven->grade = 1;
	inven->num = 3;

	inven = &slot[8];
	inven->index = 2;
	inven->grade = 2;
	inven->num = 4;

#endif

	for (int i = 0; i < 30; i++)
	{
		int n = 3 + i;// equip:0, 1, 2  inventory:3, ...

		img = new iImage();

		stSlot[n] = new iStrTex * [2];
		sl = &slot[i];
		for (int j = 0; j < 2; j++)
		{
			iStrTex* st = new iStrTex(methodStSlot);
			st->setString("%d/%d/%d/%d", sl->index, sl->grade, sl->num, j);

			img->addObject(st->tex);
			stSlot[n][j] = st;
		}

		img->position = iPointMake(offX + itemW * (i % itemX),
			offY + itemH * (i / itemX));
		pop->addObject(img);
		imgSlot[n] = img;
	}

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointMake((devSize.width - 512) / 2, (devSize.height - 320) / 2);
	pop->methodDrawBefore = draPopInventoryBefore;
	pop->methodDrawAfter = draPopInventoryAfter;
	popInven = pop;
	delete g;

	selectedItem = -1; 
	selectingItem = -1;

}

Texture* methodStSlot(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum, '/');
	int index = atoi(line[0]);
	int grade = atoi(line[1]);
	int num = atoi(line[2]);
	int j = atoi(line[3]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(itemW, itemH);
	g->init(size);
	if (num)
	{
		if (j == 0)
			setRGBA(0.2, 0.2, 0.2, 1);
		else
			setRGBA(1, 1, 0, 1);
		g->fillRect(0, 0, size.width, size.height, 3);

		setStringSize(itemH * 0.333333f);
		setStringRGBA(0, 0, 0, 1);
		g->drawString(0, 0, TOP | LEFT, "%d", index);
		g->drawString(size.width, 0, TOP | RIGHT, "%d", grade);
		g->drawString(size.width / 2, size.height, BOTTOM | HCENTER, "%d", num);
	}
	else
	{
		if (j == 0)
			setRGBA(0.2, 0.2, 0.2, 0.5);
		else
			setRGBA(0.7, 0.7, 0.7, 0.5);
		g->fillRect(0, 0, size.width, size.height, 3);
	}
	Texture* tex = g->getTexture();
	delete g;
	
	return tex;
}



void freePopInventory()
{
	delete popInven;
	delete imgInventoryBtn;

	for (int i = 0; i < 33; i++)
	{
		for (int j = 0; j < 2; j++)
			delete stSlot[i][j];
		delete stSlot[i];
	}
	delete stSlot;
	delete imgSlot;	
}

void draPopInventoryBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 33; i++)
	{
		Slot* s = &slot[i];
		for (int j = 0; j < 2; j++)
			stSlot[i][j]->setString("%d/%d/%d/%d", s->index, s->grade, s->num, j);

		imgSlot[i]->frame = (i == selectedItem || i == selectingItem);

	}

	for (int i = 0; i < 3; i++)
		imgInventoryBtn[i]->frame = (i == popInven->selected);
}
void draPopInventoryAfter(float dt, iPopup* pop)
{

	if (draggingItem)
	{
		Texture* tex = imgSlot[selectedItem]->tex;
		drawImage(tex, originPoint, TOP | LEFT);
	}
}
void drawPopInventory(float dt)
{
	popInven->paint(dt);
}

bool keyPopInventory(iKeyState stat, iPoint p)
{
	if (popInven->bShow == false)
		return false;
	if (popInven->state != iPopupStateProc)
		return true;

	int i = -1;
	int j = -1;

	switch (stat) {
	case iKeyStateBegan:

		if (getKeyStat(keyboard_enter))
			{
				showPopInventory(true);
				printf("Inven\n");
			}


		if (selectedItem != -1)
		{
			imgSlot[selectedItem]->alpha = 0;
			

			draggingItem = true;
			draggingPoint = p;

			originPoint = imgSlot[selectedItem]->position + popInven->closePoint;
		}

		i = popInven->selected;
		if (i == -1) break;
		
		if (i == 0)
		{
			printf("오름차순");
		}
		else if (i == 1)
		{
			printf("내림차순");
		}
		else
		{
			printf("닫기");
			showPopInventory(false);
		}
	case iKeyStateMoved:
		if (draggingItem)
		{
			iPoint mp = p - draggingPoint;
			originPoint += mp;
			draggingPoint = p;

			for (i = 0; i < 33; i++)
			{
				if (containPoint(p, imgSlot[i]->rect(popInven->closePoint)))
				{
					selectingItem = i;
					break;
				}
			}

			break;
		}

		for (i = 0; i < 33; i++)
		{
			if (containPoint(p, imgSlot[i]->rect(popInven->closePoint)))
			{
				selectedItem = i;
				break;
			}
		}
		

		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgInventoryBtn[i]->rect(popInven->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popInven->selected)
		{
			printf("audio play btn\n");
			popInven->selected = j;
		}
		break;

	case iKeyStateEnded:
		if (draggingItem)
		{
			draggingItem = false;
			imgSlot[selectedItem]->alpha = 1;

			if (selectingItem == -1 || selectingItem == selectedItem)
			{
			}
			else
			{
				Slot* src = &slot[selectedItem];
				Slot* dst = &slot[selectingItem];
				if (dst->num == 0)
				{
					// move
					memcpy(dst, src, sizeof(Slot));
					memset(src, 0x00, sizeof(Slot));
				}
				else// if( dst->num>0 )
				{
					if (dst->index == src->index &&
						dst->grade == src->grade)
					{
						// add(일단, 최대개수 무한대)
#if 0
						dst->num += src->num;
#else
						int max = item[dst->index].have;

						int n = dst->num;
						dst->num += src->num;
						if (dst->num > max)
							dst->num = max;
						src->num -= (dst->num - n);
#endif
					}
					else
					{
						// swap
						Slot tmp;
						memcpy(&tmp, src, sizeof(Slot));
						memcpy(src, dst, sizeof(Slot));
						memcpy(dst, &tmp, sizeof(Slot));
					}
				}
			}
			selectedItem = selectingItem;//selectedItem = -1;
			selectingItem = -1;
		}
		break;

	}

	return true;
	
}

void showPopInventory(bool show)
{
	popInven->show(show);
}


void addInvetory(int index, int num)
{
	for (int i = 3; i < 33; i++)
		if (&slot[i] == 0)
			continue;
		else
		{
			Slot* inven = &slot[i];
			inven->index = index;
			inven->grade = 1;
			inven->num += 1;
			break;
		}
}

int styleOfSlot(int index)
{
	// 0~9 : head
	// 10~19 : weapon
	// 20~29 : shoes
	// etc

	if (index < 10)
		return 0;
	else if (index < 20)
		return 1;
	else if (index < 30)
		return 2;

	return 100;
}
