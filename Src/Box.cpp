#include "Box.h"
#include "Vector2D.h"
#include "Game.h"
#include "Enums.h"
#include "bot.h"

#include <windows.h>
#include <iostream>
#include <string>

#define boxPiece Game::board.getPiece(Vector2D(y, x))

extern Box box;

int Game::difficulty;
bool Game::endPressed;

Box::Box()
{
	init();
}

void Box::init()
{
	x = 0;
	y = 5;
}

void Box::update()
{
	if (GetAsyncKeyState(VK_END) && 0x8000)
	{
		Game::endPressed = true;
		Game::running = false;
	}
	else if ((GetAsyncKeyState('s') || GetAsyncKeyState('S')) && 0x8000)
	{
		if (y < 7)
		{
			y += 1;
			if (x < 7 && x > 0)
			{
				x += y % 2 ? 1 : -1;
			}
			else if (x >= 1)
				x -= 1;
			else x += 1;
		}
	}

	else if ((GetAsyncKeyState('w') || GetAsyncKeyState('W')) && 0x8000)
	{
		if (y > 0)
		{
			y--;
			if (x < 7 && x > 0)
			{
				x += y % 2 ? 1 : -1;
			}
			else if (x >= 1)
				x -= 1;
			else x += 1;
		}
	}

	else if ((GetAsyncKeyState('a') || GetAsyncKeyState('A')) && 0x8000)
	{
		if (x > 1) x -= 2;
	}

	else if ((GetAsyncKeyState('d') || GetAsyncKeyState('D')) && 0x8000)
	{
		if (x < 6) x += 2;
	}

	else if (GetAsyncKeyState(VK_SPACE) && 0x8000)
	{
		if (Game::board.Selected)
		{
			std::vector<move> legalmoves = Game::board.SelectedPiece->getLegalMoves(Game::board);
			std::vector<move> legalmoves1 = Game::board.getLegalMoves(Game::Turn);

			if (
				Game::board.getPiece(boxPiece) == nullptr &&
				std::find(legalmoves.begin(), legalmoves.end(), move(Game::board.SelectedPiece->pos, Vector2D(y, x))) != legalmoves.end() &&
				std::find(legalmoves1.begin(), legalmoves1.end(), move(Game::board.SelectedPiece->pos, Vector2D(y, x))) != legalmoves1.end()
				)
			{

				auto mm = std::find(legalmoves.begin(), legalmoves.end(), move(Game::board.SelectedPiece->pos, Vector2D(y, x)));
				Game::board.simulate_move(*mm);

				Game::board.Selected = false;
				Game::Turn = !Game::Turn;

				if (!Game::bot_vs_bot) Game::moved = true;
			}
			else if (boxPiece != nullptr && boxPiece->color == Game::Turn)
				Game::board.SelectedPiece = boxPiece;
		}
		else if (boxPiece != nullptr && boxPiece->color == Game::Turn)
		{
			Game::board.Selected = true;
			Game::board.SelectedPiece = boxPiece;
		}
	}
	else
		return;

	if (!Game::bot_vs_bot)
	{
		Game::draw();
		Sleep(200);
		if (Game::moved)
		{
			Sleep(800);
			Game::board.simulate_move(bot::getBestMove(Game::board, Game::difficulty, Game::Turn));
			Game::Turn = !Game::Turn;
			Game::moved = false;
			Game::draw();
		}
		if (Game::board.game_over(Game::Turn))
			Game::running = false;
	}
}