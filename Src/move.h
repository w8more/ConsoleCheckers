#pragma once

#include <vector>
#include "Vector2D.h"
#include "Piece.h"

class move
{
public:
	move() { toKing = false; }
	move(Vector2D x, Vector2D y) { src = x; dest = y; toKing = false; }
	move(Vector2D x, Vector2D y, bool c) { src = x; dest = y; toKing = c; }
	move(Vector2D x, Vector2D y, bool c, Piece* toD) {
		src = x; dest = y; toKing = c; 
		toDelete.push_back(*toD);
	}
	move(Vector2D x, Vector2D y, bool c, std::vector<Piece> toD) {
		src = x; dest = y; toKing = c;
		toDelete = toD;
	}
	std::vector<Piece> toDelete;
	Vector2D src;
	Vector2D dest;
	bool toKing;
	float score;
	bool operator==(const move& other) { return src == other.src && dest == other.dest; }
};