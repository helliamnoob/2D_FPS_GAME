#pragma once

#include "iStd.h"

enum GameState
{
	GameStateIntro = 0,
	GameStateMenu,
	GameStateProc,
	GameStateLobby,
	GameStateEnding,
	GameStateNone
};

extern GameState gs;

typedef void (*MethodLoading)();

void setLoading(GameState gs, MethodLoading free, MethodLoading load);
void drawLoading(float dt);

#define loadingDelta 0.5f
