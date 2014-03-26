#pragma once

#include <Windows.h>
#include <vector>
#include <set>


class MoveInfo;

class Tile
{
public:
	enum Owner
	{
		NONE,
		CROSS,
		RING
	};
	Tile(int pX, int pY, int sX, int sY);
	Tile(const Tile& t);
	virtual ~Tile();
	virtual void render(HDC* hdc);
	virtual void setWidth(int w, int pX);
	virtual void setHeight(int h, int pY);
	int getWidth()		{return sizeX;}
	int getHeight()		{return sizeY;}

	virtual void makeMove(MoveInfo& move, int depth);
	virtual void makeDebugMove(MoveInfo& move, int depth);

	Owner getOwner()	{return owner;}
	virtual bool activateBoard();
	virtual void deactivateBoard();
	bool isActive() const {return activated;}
	virtual int determineValueForAI(Tile::Owner sign, int depth);

	

protected:
	int posX;
	int posY;
	int sizeX;
	int sizeY;
	Owner owner;
	HPEN blackPen;
	HPEN redPen;
	bool activated;
	//int id;
	//static std::set<int> sCreatedTiles;
	//static int iCreatedTiles;
};