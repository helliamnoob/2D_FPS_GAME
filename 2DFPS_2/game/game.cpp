#include "game.h"


void loadGame()
{
	// game save data
	// audio



#if 1
	loadIntro();
	gs = GameStateIntro;
#else
	loadProc();
	gs = GameStateGame;
#endif
}

void freeGame()
{
	// audio


	switch (gs) {
	case GameStateIntro:		freeIntro();	break;
	case GameStateMenu:			freeMenu();	break;
	case GameStateLobby:		freeLobby(); break;
	case GameStateProc:			freeProc();	break;
	//case GameStateEnding:			freeEnd();	break;
	};
}

void drawGame(float dt)
{
	switch (gs) {
	case GameStateIntro:		drawIntro(dt);	break;
	case GameStateMenu:			drawMenu(dt);	break;
	case GameStateLobby:		drawLobby(dt);	break;
	case GameStateProc:			drawProc(dt);	break;

	};

	drawLoading(dt);

}

bool keyGame(iKeyState stat, iPoint p)
{
	switch (gs) {
	case GameStateIntro:		keyIntro(stat, p);	break;
	case GameStateMenu:			keyMenu(stat, p);	break;
	case GameStateLobby:		keyLobby(stat, p);  break;
	case GameStateProc:			keyProc(stat, p);	break;


	};
		
	return true;
}
