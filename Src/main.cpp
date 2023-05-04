#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Game.h"

extern Box box;

int main()
{
	Game* game = new Game();

	std::srand(std::time(NULL));

	SetConsoleTextAttribute(Game::hConsole, 181);
	system("cls");

	
	int d;
	std::cout << "Welcome to game of checkers!" << std::endl;
	std::cout << "You can with WASD and SPACE enter difficulty to start." << std::endl;
	std::cout << "Press End to end the game anytime" << std::endl;
	std::cout << "Enter difficulty(2 - recommended): ";
	std::cin >> d;

	game->setDif(d);

	while (Game::running)
	{
		game->update();
	}

	game->printWinner();

	Sleep(10000);
	return 0;
}