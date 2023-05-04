#include "Piece.h"
#include "Enums.h"
#include "Game.h"
#include "Board.h"

#include <iostream>

Board Game::board;
bool Game::forceMoves;

extern HANDLE hConsole;

Piece::Piece(Vector2D pos, bool color, bool king)
{
	this->pos = pos;
	this->color = color;
	this->king = king;
	selected = false;
}

Piece::Piece(Vector2D pos)
{
	this->pos = pos;
	selected = false;
}

std::vector<move> Piece::getLegalMoves(Board& Inst)
{
	std::vector<move> Result;

	if (king == false)
	{
		if (color)
		{
			if (pos.y - 1 == 0)
			{
				if(Inst.getPiece(Vector2D(pos.y - 1, pos.x + 1)) == nullptr && pos.y > 0 && pos.x < 7) Result.push_back(move(pos, Vector2D(pos.y - 1, pos.x + 1), 1));
				if (Inst.getPiece(Vector2D(pos.y - 1, pos.x - 1)) == nullptr && pos.y > 0 && pos.x > 0) Result.push_back(move(pos, Vector2D(pos.y - 1, pos.x - 1),1));
			}
			else
			{
				if (Inst.getPiece(Vector2D(pos.y - 1, pos.x + 1)) == nullptr && pos.y > 0 && pos.x < 7) Result.push_back(move(pos, Vector2D(pos.y - 1, pos.x + 1)));
				if (Inst.getPiece(Vector2D(pos.y - 1, pos.x - 1)) == nullptr && pos.y > 0 && pos.x > 0) Result.push_back(move(pos, Vector2D(pos.y - 1, pos.x - 1)));
			}
		}
		else
		{
			if (pos.y + 1 == 7)
			{
				if (Inst.getPiece(Vector2D(pos.y + 1, pos.x + 1)) == nullptr && pos.y < 7 && pos.x < 7) Result.push_back(move(pos, Vector2D(pos.y + 1, pos.x + 1), 1));
				if (Inst.getPiece(Vector2D(pos.y + 1, pos.x - 1)) == nullptr && pos.y < 7 && pos.x > 0) Result.push_back(move(pos, Vector2D(pos.y + 1, pos.x - 1), 1));
			}
			else
			{
				if (Inst.getPiece(Vector2D(pos.y + 1, pos.x + 1)) == nullptr && pos.y < 7 && pos.x < 7) Result.push_back(move(pos, Vector2D(pos.y + 1, pos.x + 1)));
				if (Inst.getPiece(Vector2D(pos.y + 1, pos.x - 1)) == nullptr && pos.y < 7 && pos.x > 0) Result.push_back(move(pos, Vector2D(pos.y + 1, pos.x - 1)));
			}
		}
		for (int a = -2; a < 3; a += 4)
		{
			for (int b = -2; b < 3; b += 4)
			{
				Vector2D dest(Vector2D(pos.y + a, pos.x + b));

				if (dest.y >= 0 && dest.y < 8 && dest.x >= 0 && dest.x < 8)
				{
					Piece* capturedPiece = Inst.getPiece(Vector2D(dest.y - a / 2, dest.x - b / 2));
					
					if (Inst.getPiece(dest) == nullptr && capturedPiece != nullptr && capturedPiece->color == !color)
					{
						move temp(pos, dest, 0, capturedPiece);
						if ((color == White && pos.y + a == 0) || (!color && pos.y + a == 7))
							temp.toKing = true;
						Result.push_back(temp);


						Board second = NewBoard(Inst, temp.toDelete);

						if (temp.toKing)
							check_king_moves_after_leap(temp, second, Result, temp.dest.y - temp.src.y == temp.dest.x - temp.src.x);
						else
							check_moves_after_leap(temp, second, Result);
					}
				}
			}
		}
	}
	else if (king == true)
	{
		std::vector<Piece> jumpedPieces;
		for (int a = -1; a < 2; a += 2)
		{
			for (int b = -1; b < 2; b += 2)
			{
				for (int y = pos.y + a, x = pos.x + b; x < 8 && x >= 0 && y < 8 && y >= 0; y += a, x += b)
				{
					if (Inst.getPiece(Vector2D(y, x)) == nullptr)
					{
						move temp(pos, Vector2D(y,x), 1, jumpedPieces);
						Result.push_back(temp);

						if (!jumpedPieces.empty())
						{
							Board second = NewBoard(Inst, temp.toDelete);

							check_king_moves_after_leap(temp, second, Result, y - pos.y == x - pos.x);
						}
					}
					else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == color)
						break;
					else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == !color)
					{
						if (Inst.getPiece(Vector2D(y + a, x + b)) != nullptr || y + a > 7 || y + a < 0 || x + b > 7 || x + b < 0)
							break;
						jumpedPieces.push_back(*Inst.getPiece(Vector2D(y, x)));
					}
				}
				jumpedPieces.clear();
			}
		}
	}
	return Result;
}

void Piece::check_moves_after_leap(move& curr, Board& Inst, std::vector<move>& Result)
{
	bool deleteCoords = false;
	for (int a = -2; a < 3; a += 4)
	{
		for (int b = -2; b < 3; b += 4)
		{
			Vector2D dest(Vector2D(curr.dest.y + a, curr.dest.x + b));
			if (dest.y >= 0 && dest.y < 8 && dest.x >= 0 && dest.x < 8)
			{
				if (dest == curr.src) continue;
				Piece* capturedPiece = Inst.getPiece(Vector2D(dest.y - a / 2, dest.x - b / 2));

				if (
					Inst.getPiece(dest) == nullptr && 
					capturedPiece != nullptr && 
					capturedPiece->color == !color
					)
				{
					move temp(pos, dest, 0, curr.toDelete);
					temp.toDelete.push_back(*capturedPiece);
					if ((color == White && dest.y == 0) || (color == Black && dest.y == 7))
						temp.toKing = true;
					Result.push_back(temp);

					temp.src = curr.dest;

					Board second = NewBoard(Inst, temp.toDelete);

					if (temp.toKing)
						check_king_moves_after_leap(temp, second, Result, temp.dest.y - temp.src.y == temp.dest.x - temp.src.x);
					else
						check_moves_after_leap(temp, second, Result);

					deleteCoords = true;
				}
			}
		}
	}
	if (deleteCoords)
	{
		curr.src = pos;
		Result.erase(std::find(Result.begin(), Result.end(), curr));
	}
		
}



void Piece::check_king_moves_after_leap(move& curr, Board& Inst, std::vector<move>& Result, bool LeftToRight)
{
	std::vector<Piece> jumpedPieces = curr.toDelete;
	bool deleteSrcCoords = false;
	if (LeftToRight)
	{
		for (int a = -1, b = 1; a < 2; a += 2, b -= 2)
		{
			for (int y = curr.dest.y + a, x = curr.dest.x + b, c = 0; y >= 0 && y < 8 && x >= 0 && x < 8; y += a, x += b)
			{
				if (Inst.getPiece(Vector2D(y, x)) == nullptr && c)
				{
					move temp(pos, Vector2D(y, x), 1, jumpedPieces);
					Result.push_back(temp);
					temp.src = curr.dest;

					Board second = NewBoard(Inst, temp.toDelete);

					check_king_moves_after_leap(temp, second, Result, false);
					deleteSrcCoords = true;
				}
				else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == color) break;
				else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == !color)
				{
					if (y + a < 0 || y + a > 7 || x + b < 0 || x + b > 7 || Inst.getPiece(Vector2D(y + a, x + b)) != nullptr || (c && Inst.getPiece(Vector2D(y - a, x - b)) != nullptr)) break;
					else
					{
						jumpedPieces.push_back(*Inst.getPiece(Vector2D(y, x)));
						c = 1;
					}
				}
			}
			jumpedPieces = curr.toDelete;
		}
	}
	else
	{
		for (int a = -1, b = -1; a < 2; a += 2, b += 2)
		{
			for (int y = curr.dest.y + a, x = curr.dest.x + b, c = 0; y >= 0 && y < 8 && x >= 0 && x < 8; y += a, x += b)
			{
				if (Inst.getPiece(Vector2D(y, x)) == nullptr && c)
				{
					move temp(pos, Vector2D(y, x),1,jumpedPieces);
					Result.push_back(temp);
					temp.src = curr.dest;

					Board second = NewBoard(Inst, temp.toDelete);

					check_king_moves_after_leap(temp, second, Result, true);
					deleteSrcCoords = true;
				}
				else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == color) break;
				else if (Inst.getPiece(Vector2D(y, x)) && Inst.getPiece(Vector2D(y, x))->color == !color)
				{
					if (y + a < 0 || y + a > 7 || x + b < 0 || x + b >7 || Inst.getPiece(Vector2D(y + a, x + b)) != nullptr || (c && Inst.getPiece(Vector2D(y - a, x - b)) != nullptr)) break;
					else
					{
						jumpedPieces.push_back(*Inst.getPiece(Vector2D(y, x)));
						c = 1;
					}
				}
			}
			jumpedPieces = curr.toDelete;
		}
	}
	if (deleteSrcCoords)
	{
		curr.src = pos;
		Result.erase(std::find(Result.begin(), Result.end(), curr));
	}
}

Board Piece::NewBoard(Board& Inst, std::vector<Piece>& d)
{
	Board second(Inst);
	for (Piece p : d)
	{
		auto n = std::find_if(second.AllPieces.begin(), second.AllPieces.end(), [p](Piece* p2) {return p2->pos == p.pos; });
		if (n != second.AllPieces.end())
			second.AllPieces.erase(n);
	}
	return second;
}