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
	for (int y = 0; y < 50; y++) {

		for (int x = 0; x < Graphics::screenWidth; x++) {

			gfx.AddPixelToBuffer(x, y, Colors::White);
		}
	}
}
