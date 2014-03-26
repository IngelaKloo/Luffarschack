#pragma once
#include "Tile.h"

class Board;
class Player
{
public:
	enum KindOfPlayer
	{
		HUMAN,
		AI
	};
	Player(Board* b, Tile::Owner s, KindOfPlayer kop):board(b), sign(s),kindOfPlayer(kop) {}
	MoveInfo click(int curserX, int curserY);
	void debugClick(int curserX, int curserY);
	MoveInfo space();
	MoveInfo determineBestMove(Board* boardBeforeMove, int depth);
	const KindOfPlayer kindOfPlayer;
	
protected:
	Board* board;
	const Tile::Owner sign;
	
};