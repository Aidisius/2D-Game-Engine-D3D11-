#include "Game.h"


Game::Game(HWND& hWnd)
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
	if (GetKeyState(VK_UP) & 0xF000) {

		Link.SetPos(Link.GetPos().x, Link.GetPos().y - 1);
	}
	else if (GetKeyState(VK_DOWN) & 0xF000) {

		Link.SetPos(Link.GetPos().x, Link.GetPos().y + 1);
	}
	if (GetKeyState(VK_RIGHT) & 0xF000) {

		Link.SetPos(Link.GetPos().x + 1, Link.GetPos().y);
	}
	else if (GetKeyState(VK_LEFT) & 0xF000) {

		Link.SetPos(Link.GetPos().x - 1, Link.GetPos().y);
	}
}

void Game::GameGraphicsUpdate()
{
	gfx.DrawSprite(Link, {128, 0, 128});
}
