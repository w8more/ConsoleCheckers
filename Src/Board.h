#pragma once

#include <vector>
#include <windows.h>

#include "Piece.h"
#include "move.h"


class Board
{
public:
	Board() noexcept;
	~Board();
	Board(const Board& other) noexcept;
	void init();
	void draw();
	float avaluate();
	bool game_over(bool Turn);
	void simulate_move(const move& Move);
	std::vector<move> getLegalMoves(int Turn = -1);
	std::vector<Piece*> getAllPieces(int color = -1);
	Piece* getPiece(Vector2D position);
	Piece* getPiece(Piece* p);
	COLORREF determineColor(int y, int x);

	bool Selected;
	std::vector<Piece*> AllPieces;
	Piece* SelectedPiece;
private:
	void drawRect(int y, int x, char c);
};