#pragma once

#include "iStd.h"
#include "Menu.h"
#include "Map.h"
#include "Ending.h"
//#include "Ending.h"


void loadProc();
void freeProc();
void drawProc(float dt);
bool keyProc(iKeyState stat, iPoint p);


//////////////////////////////////////////////////////////////////
// Player cpp/h
//////////////////////////////////////////////////////////////////
//#define bulletMax 20

struct Object
{
	iPoint position;
	float direction;// degree
	float moveSpeed;
	float rate, _rate;
	bool alive;
	bool fire;


	float hp, _hp, bhp, _bhp;
	int index;
	int ap;
	int dp;
	
};

struct Player :Object
{

	Player();
	virtual ~Player();

	void setMove(iPoint v, float dt);
	void setDirection(iPoint v, float dt);
	void draw(float dt, iPoint off);
	iRect rect();

	Object* own;
	bool alive;
	//Texture* tex;
	iImage* img;
};

extern Player* ply;

void loadPlayer(bool autoplay);
void freePlayer();
void drawPlayer(float dt, iPoint off);

class Goal
{
public: 
	Goal(iPoint p);
	~Goal();

	void paint(float dt, iPoint off);
	bool touch(iRect rt);

	iPoint position;
};

//////////////////////////////////////////////////////////////////
// Enemy cpp/h
//////////////////////////////////////////////////////////////////
struct Enemy;
void methodEnemyAI0(Enemy* e, float dt, iPoint off);
typedef void (*EnemyAI)(Enemy* e, float dt, iPoint off);
struct Enemy : Object
{

	EnemyAI ai;

	iPoint v;
	float rangeFind;
	float rangeAttack;

	Enemy();
	virtual ~Enemy();

	void draw(float dt, iPoint off);
	bool canAttack(float dt);
	void setDirection(iPoint v, float dt);
	iRect rect();
	Texture* tex;
	iImage* img;
};

extern Enemy** _enemy;
extern Enemy** enemy;
extern int enemyNum;
void loadEnemy();
void freeEnemy();
void drawEnemy(float dt, iPoint off);
void addEnemy(iPoint p, float dt);

//////////////////////////////////////////////////////////////////
// Bullet cpp/h
//////////////////////////////////////////////////////////////////
struct Bullet
{
	bool alive = false;
	int index;
	iPoint p, v;
	float speed;
	Object* own;

	Bullet();

	bool check();

	void draw(float dt);
	void setShoot(iPoint p, iPoint v);
	iRect rect();

	iImage* img;
};

void loadBullet();
void freeBullet();
void drawBullet(float dt);
void addBullet(Object* own, iPoint p, iPoint v);
void addBullet(Object* own, iPoint sp, iPoint tp, iPoint v);


//////////////////////////////////////////////////////////////////
// ProcUI cpp/h
//////////////////////////////////////////////////////////////////
void loadProcUI();
void freeProcUI();
void drawProcUI(float dt);
bool keyProcUI(iKeyState stat, iPoint p);
void showProcUI(bool show);

struct EndTimer
{
	int countTime;
	int endTime;
	bool alive;
};

//////////////////////////////////////////////////////////////////
// StopMenu cpp/h
//////////////////////////////////////////////////////////////////
extern iPopup* popStopMenu;
void createPopStopMenu();
void freePopStopMenu();
void drawPopStopMenu(float dt);
bool keyPopStopMenu(iKeyState stat, iPoint p);
void showPopStopMenu(bool show);

//////////////////////////////////////////////////////////////////
// Result cpp/h
//////////////////////////////////////////////////////////////////
extern iPopup* popResult;
void createPopResult();
void freePopResult();
void drawPopResult(float dt);
bool keyPopResult(iKeyState stat, iPoint p);
void showPopResult(bool show);



//////////////////////////////////////////////////////////////////
// ITEM cpp/h
//////////////////////////////////////////////////////////////////

struct Item
{
	const char* name;
	int have;
	int price;
	int x, y;

};

struct DropItem : Item
{
	DropItem();
	virtual ~DropItem();
	iRect rect();

	bool draw(float dt, iPoint off);
	void set(iPoint p);

	bool alive;
	int index;
	Texture* tex;
	iPoint position;
	float delta;
};
void loadDropItem();
void freeDropItem();
void drawDropItem(float dt, iPoint off);
void addDropItem(iPoint p);
//////////////////////////////////////////////////////////////////
///// Loot Item
//////////////////////////////////////////////////////////////////
struct LootItem
{
	LootItem()
	{
		memset(num, 0x00, sizeof(int) * 10);
	}

	void add(int index, int n)
	{
		num[index] += n;
	}
	int num[10];
};
extern LootItem* lootItem;

//////////////////////////////////////////////////////////////////
///// INVEN
//////////////////////////////////////////////////////////////////
struct Slot
{
	int index;	// 0:hp 1:mp 2:....


	int grade;	// 0:normal 1:a 2:s
	int num;	// 0:none 1~ count
};

#define itemW 40
#define itemH 40
#define itemX 6
#define itemY 5
void createPopInventory();
void freePopInventory();
void drawPopInventory(float dt);
bool keyPopInventory(iKeyState stat, iPoint p);
void showPopInventory(bool show);

void addInvetory(int index, int num);
//void removeInentory(int index, int num=1);

int styleOfSlot(int index);
