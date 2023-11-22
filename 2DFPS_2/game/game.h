#pragma once

#include "iStd.h"
#include "intro.h"
#include "Menu.h"
#include "Loading.h"
#include "Lobby.h"

void loadGame();
void drawGame(float dt);
void freeGame();
bool keyGame(iKeyState stat, iPoint p);

