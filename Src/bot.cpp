#include "bot.h"
#include "Enums.h"

#include <vector>
#include <iostream>



move bot::getBestMove(Board& board, int depth, bool maximaze)
{
    if (depth == 0 || board.game_over(maximaze))
    {
        move a;
        a.score = board.avaluate();
        return a;
    }

    move best_move;

    if (maximaze)
    {
        best_move.score = -9999;
        std::vector<move>allMoves = board.getLegalMoves(maximaze);
        for (auto Move : allMoves)
        {
            Board NewBoard(board);
            NewBoard.simulate_move(Move);
            Move.score = getBestMove(NewBoard, depth - 1, false).score;
            if (Move.score > best_move.score)
                best_move = Move;
            else if (Move.score == best_move.score)
                if (!(rand() % 3)) 
                    best_move = Move;
                
        }
        return best_move;
    }
    else
    {
        best_move.score = 9999;
        std::vector<move>allMoves = board.getLegalMoves(maximaze);
        for (auto Move : allMoves)
        {
            Board NewBoard(board);
            NewBoard.simulate_move(Move);
            Move.score = getBestMove(NewBoard, depth - 1, true).score;
            if (Move.score < best_move.score)
                best_move = Move;
            else if (Move.score == best_move.score)
                if (!(rand() % 3)) 
                    best_move = Move;
        }
        return best_move;
    }
}