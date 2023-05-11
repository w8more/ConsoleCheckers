#include "Game.h"
#include "Enums.h"
#include "bot.h"

#include <iostream>
#include <windows.h>

Box box;
bool Game::bot_vs_bot;
bool Game::Turn;
bool Game::moved;
bool Game::running;
HANDLE Game::hConsole;

Game::Game()
{
	init();
}

void Game::init()
{
	bot_vs_bot = false;
	Turn = White;
	forceMoves = true;
	running = true;
	endPressed = false;
	moved = bot_vs_bot;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);
	board.init();
	CreateCompatibleDC(hdc);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	width = (csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	system("cls");
}

void Game::update()
{
	
	if (bot_vs_bot)
	{
		Game::draw();
		Sleep(200);
		if (Game::moved)
		{
			Sleep(1000);
			Game::board.simulate_move(bot::getBestMove(Game::board, Game::difficulty, Game::Turn));
			Game::Turn = !Game::Turn;
			Game::draw();
		}
		if (Game::board.game_over(Game::Turn))
			Game::running = false;
		Game::draw();
	}
	else
	{
		box.update();
	}
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	width = (csbi.srWindow.Right - csbi.srWindow.Left + 1);
	height = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
	
}

void Game::draw()
{
	Game::board.draw();
}

void Game::setDif(int a)
{
	difficulty = a;
	Game::draw();
}

void Game::printWinner()
{
	SetConsoleTextAttribute(Game::hConsole, 176);
	system("cls");

	if (!endPressed)
	{
		if (!Game::Turn)
			std::cout << "Game over: White have won!" << std::endl;
		else
			std::cout << "Game over: Black have won!" << std::endl;
	}
	else
	{
		std::cout << "You successfully left the game" << std::endl;
	}
}
