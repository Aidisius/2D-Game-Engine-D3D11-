#pragma once
#include "Graphics.h"
#include "Vec2D.h"
#include <string>


class Game
{
public:
	// Constructor and destructor
	Game(HWND& hWnd);
	Game(const Game&) = delete;
	Game operator=(const Game) = delete;

private:
	// Functions
	void GameLogicUpdate();
	void GameGraphicsUpdate();
public:
	void Cycle();

private:
	// Variables
	Graphics gfx;
	Sprite Link = {"LinkStanding.bmp"};
};