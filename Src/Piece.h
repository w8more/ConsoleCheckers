#pragma once

class Piece;
class Board;
class move;

#include <vector>
#include "Vector2D.h"
#include "Board.h"
#include "move.h"



class Piece
{
public:
	Piece(Vector2D position);
	Piece(Vector2D pos, bool color, bool king);
	Piece(const Piece& other) = default;
	std::vector<move> getLegalMoves(Board& Inst);

	void draw();
	void setNewPos(Vector2D pos) { this->pos = pos; }
	
	Vector2D pos;
	bool color;
	bool king;
	bool selected;

	bool operator==(const Piece* other) { return this->pos == other->pos; }

private:
	Board NewBoard(Board&, std::vector<Piece>&);
	void check_moves_after_leap(move& curr, Board& Inst, std::vector<move>& Result);
	void check_king_moves_after_leap(move& curr, Board& Inst, std::vector<move>& Result, bool LeftToRight);
	
};