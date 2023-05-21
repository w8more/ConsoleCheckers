#pragma once

#include <Windows.h>

#include "Box.h"
#include "Board.h"

class Game
{
public:
	~Game();
	Game();
	void init();
	void update();
	static void draw();
	static Board board;
	static bool running;
	static bool Turn;
	static bool forceMoves;
	static bool moved;
	static bool endPressed;
	static bool bot_vs_bot;
	void printWinner();
	void setDif(int);
	static HANDLE hConsole;
	static HWND hwnd;
	static HDC hdc;
	static int width;
	static int height;
	static int difficulty;
};