#include "Game.h"

Game::Game(HWND hWnd)
	:
	gfx(hWnd)
{}

void Game::Cycle()
{
	gfx.BeginFrame();
	GameLogicUpdate();
	GameGraphicsUpdate();
	gfx.EndFrame();
}

void Game::GameLogicUpdate()
{
}

void Game::GameGraphicsUpdate()
{
}
