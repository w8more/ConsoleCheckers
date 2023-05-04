#pragma once

#include "move.h"
#include "Board.h"

class bot
{
public:
	static move getBestMove(Board& board, int depth, bool maximaze);
};

