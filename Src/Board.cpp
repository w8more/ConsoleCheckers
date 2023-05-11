#include "Board.h"
#include "Enums.h"
#include "Game.h"

#include <Windows.h>
#include <iostream>

extern Box box;

int Game::width;
int Game::height;
HDC Game::hdc;
HWND Game::hwnd;

Board::Board() noexcept
{}

Board::~Board()
{
	for (;!AllPieces.empty();)
	{
		delete AllPieces[0];
		AllPieces.erase(AllPieces.begin());
	}
}

Board::Board(const Board& other) noexcept
{
	for (int i = 0; i < other.AllPieces.size(); ++i)
	{
		AllPieces.push_back(new Piece(*other.AllPieces[i]));
	}
	Selected = false;
	SelectedPiece = nullptr;
}

void Board::init()
{
	for (; !AllPieces.empty();)
	{
		delete AllPieces[0];
		AllPieces.erase(AllPieces.begin());
	}
	
	// Black
	AllPieces.push_back(new Piece(Vector2D(0, 1), Black, false));
	AllPieces.push_back(new Piece(Vector2D(0, 3), Black, false));
	AllPieces.push_back(new Piece(Vector2D(0, 5), Black, false));
	AllPieces.push_back(new Piece(Vector2D(0, 7), Black, false));

	AllPieces.push_back(new Piece(Vector2D(1, 0), Black, false));
	AllPieces.push_back(new Piece(Vector2D(1, 2), Black, false));
	AllPieces.push_back(new Piece(Vector2D(1, 4), Black, false));
	AllPieces.push_back(new Piece(Vector2D(1, 6), Black, false));

	AllPieces.push_back(new Piece(Vector2D(2, 1), Black, false));
	AllPieces.push_back(new Piece(Vector2D(2, 3), Black, false));
	AllPieces.push_back(new Piece(Vector2D(2, 5), Black, false));
	AllPieces.push_back(new Piece(Vector2D(2, 7), Black, false));

	// White
	
	AllPieces.push_back(new Piece(Vector2D(5, 0), White, false));
	AllPieces.push_back(new Piece(Vector2D(5, 2), White, false));
	AllPieces.push_back(new Piece(Vector2D(5, 4), White, false));
	AllPieces.push_back(new Piece(Vector2D(5, 6), White, false));

	AllPieces.push_back(new Piece(Vector2D(6, 1), White, false));
	AllPieces.push_back(new Piece(Vector2D(6, 3), White, false));
	AllPieces.push_back(new Piece(Vector2D(6, 5), White, false));
	AllPieces.push_back(new Piece(Vector2D(6, 7), White, false));

	AllPieces.push_back(new Piece(Vector2D(7, 0), White, false));
	AllPieces.push_back(new Piece(Vector2D(7, 2), White, false));
	AllPieces.push_back(new Piece(Vector2D(7, 4), White, false));
	AllPieces.push_back(new Piece(Vector2D(7, 6), White, false));

	Selected = false;
}

void Board::drawRect(int y, int x, char c)
{
	RECT rect({ (x + 1) * Game::width, y * Game::height * 3, x * Game::width, (y + 1) * Game::height * 3 });
	HBRUSH hbr = CreateSolidBrush(determineColor(y,x));
	FillRect(Game::hdc,&rect,hbr);

	if (c != ' ')
	{
		char a[2];
		a[0] = c;
		a[1] = '\0';

		SetBkMode(Game::hdc, TRANSPARENT);
		SetTextColor(Game::hdc, RGB(255,255,255));
		
		DrawTextA(Game::hdc, a, 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
	}
}

void Board::draw()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* a = getPiece(Vector2D(i,j));
			if (a == nullptr)
			{
				drawRect(i, j, ' ');
			}
			else
			{
				char c;
				if (a->color)
				{
					if (a->king)
						c = 'W';
					else
						c = 'w';
				}
				else
				{
					if (a->king)
						c = 'B';
					else
						c = 'b';
				}
				drawRect(i, j, c);
			}
		}
	}
}



COLORREF Board::determineColor(int y, int x)
{
	if (box.x == x && box.y == y)
		return RGB(255,255,0); // yellow for box
	if ((y + x) % 2)
	{
		if (Selected)
		{
			std::vector<move> legalmoves = SelectedPiece->getLegalMoves(*this);
			std::vector<move> legalmoves1 = Game::board.getLegalMoves(Game::Turn);

			if (
				std::find_if(legalmoves.begin(), legalmoves.end(), [&](move& a) { return y == a.dest.y && x == a.dest.x && SelectedPiece->pos == a.src; }) != legalmoves.end() &&
				std::find_if(legalmoves1.begin(), legalmoves1.end(), [&](move& a) { return y == a.dest.y && x == a.dest.x && SelectedPiece->pos == a.src; }) != legalmoves1.end()
				)
				return RGB(0, 255, 0); //green for legal moves
			else
				return RGB(51, 0, 25); //black
		}
		else
			return RGB(51, 0, 25); //black
	}
	else
		return RGB(255, 255, 255); //white
}

std::vector<Piece*> Board::getAllPieces(int color)
{
	if (color == -1)
		return AllPieces;
	else
	{
		std::vector<Piece*> Result;

		if (color == White)
		{
			for (const auto& p : AllPieces)
			{
				if (p->color == White)
				{
					Result.push_back(p);
				}
					
			}
		}
		else if (color == Black)
		{
			for (const auto& p : AllPieces)
			{
				if (p->color == Black)
				{
					Result.push_back(p);
				}
			}
		}

		return Result;
	}
}

std::vector<move> Board::getLegalMoves(int Turn)
{
	std::vector<move> Result;

	for (const auto& p : getAllPieces(Turn))
	{
		for (move& Move : p->getLegalMoves(*this))
		{
			Result.push_back(Move);
		}
	}

	if (Game::forceMoves)
	{
		int c = 0;

		for (auto i = Result.begin(); i != Result.end(); ++i)
		{
			if (!i->toDelete.empty())
			{
				c = 1;
				break;
			}
		}

		if (c)
		{
			for (int i = 0; i < Result.size(); ++i)
			{
				if (Result[i].toDelete.empty())
				{
					Result.erase(Result.begin() + i--);
				}
			}
		}
	}

	return Result;
}

void Board::simulate_move(const move& Move)
{
	if (Move.toKing)
		getPiece(Move.src)->king = true;

	getPiece(Move.src)->setNewPos(Move.dest);

	for (auto& e : Move.toDelete)
	{
		AllPieces.erase(std::find_if(AllPieces.begin(), AllPieces.end(), [&](const Piece* p) { return e.pos == p->pos; }));
	}
}

Piece* Board::getPiece(Vector2D position)
{
	auto r = std::find_if(AllPieces.begin(), AllPieces.end(), [&](const Piece* a) {
		return a->pos == position;
		});
	if (r == AllPieces.end()) {
		return nullptr;
	}
	return *r;
}

Piece* Board::getPiece(Piece* p)
{
	if (p == nullptr) return nullptr;
	auto r = std::find_if(AllPieces.begin(), AllPieces.end(), [&](const Piece* a) {
		return a->pos == p->pos;
		});
	if (r == AllPieces.end()) {
		return nullptr;
	}
	return *r;
}

float Board::avaluate()
{
	float score = 0;

	for (const auto& p : AllPieces)
	{
		if (p->color == White)
		{
			score += 1;
			if (p->king)
				score += 0.5;
		}

		else if (p->color == Black)
		{
			score -= 1;
			if (p->king)
				score -= 0.5;
		}
	}

	return score;
}

bool Board::game_over(bool Turn)
{
	if (getLegalMoves(Turn).empty())
		return 1;

	bool w = 0, b = 0;

	for (const auto& p : AllPieces)
	{
		if (p->color == White)
			w = 1;
		else
			b = 1;
	}
	return !(w && b);
}
