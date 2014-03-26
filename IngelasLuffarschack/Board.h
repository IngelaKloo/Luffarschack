#pragma once

#include "Tile.h"
#include <vector>
#include <Windows.h>
#include "MoveInfo.h"
#include "Row.h"

using namespace std;

class Board:public Tile
{
public:
	Board(int pX, int pY, int sX, int sY, int depth);

	Board(const Board&);
	~Board();
	virtual void render(HDC* hdc);

	virtual void setWidth(int w, int pX);
	virtual void setHeight(int h, int pY);

	virtual void makeMove(MoveInfo& move, int depth);
	virtual void makeDebugMove(MoveInfo& move, int depth);

	void determineOutcome(MoveInfo& moveInfo);

	bool tileIsTaken(int tile);
	virtual bool activateBoard();
	virtual void deactivateBoard();

	Board makeResultOfMove( MoveInfo& move);
	virtual int determineValueForAI(Tile::Owner sign, int depth);
	const vector<Tile*>& getTiles()const {return tiles;}

private:
	vector<Tile*> tiles;
	static vector<Row> allPossibleRows;
};