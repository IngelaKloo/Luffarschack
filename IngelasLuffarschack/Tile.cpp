#include "Tile.h"
#include <Windows.h>
#include <cmath>
#include "Defines.h"
#include "MoveInfo.h"
#include <vector>
#include <cassert>
#include <set>

using namespace std;

//set<int> Tile::sCreatedTiles;
//int Tile::iCreatedTiles=0;
Tile::Tile(int pX, int pY, int sX, int sY):posX(pX), posY(pY), sizeX(sX), sizeY(sY)
{
	owner=NONE;
	activated = true;

	blackPen=CreatePen(PS_SOLID,1,RGB(0,0,0));
	redPen =CreatePen(PS_SOLID,1,RGB(255,0,0));
	/*sCreatedTiles.insert(++iCreatedTiles);
	id=iCreatedTiles;*/
}

Tile::Tile(const Tile& t)
{
	posX=t.posX;
	posY=t.posY;
	sizeX=t.sizeX;
	sizeY=t.sizeY;
	owner=t.owner;
	activated=t.activated;
	//sCreatedTiles.insert(++iCreatedTiles);
	//id=iCreatedTiles;
}

Tile::~Tile()
{
	//sCreatedTiles.erase(id);
	DeleteObject(blackPen);
	DeleteObject(redPen);
}

void Tile::render(HDC* hdc)
{
	HPEN oldPen=(HPEN)SelectObject(*hdc, blackPen);
	if(owner!=NONE)
	{
		if(owner==RING)
		{
			//rita en ring
			int centerX=posX+sizeX/2;
			int centerY=posY+sizeY/2;
			int radius;
			float proportion=(float)sizeY/(float)sizeX;

			radius=sizeX/2;

			MoveToEx(*hdc, centerX+radius,centerY,0);
			float pi= 3.141592F;
			for(int i=0;i<POINTS_IN_CIRCLE+1;++i)
			{
				float cosine=cos((float)(i*2*pi/POINTS_IN_CIRCLE));
				float sine=sin((float)(i*2*pi/POINTS_IN_CIRCLE));
				float proportionalSine=sine*proportion;
				LineTo(*hdc, centerX+(int)(radius*cosine), centerY+(int)(radius*proportionalSine));
			}

		}
		else if(owner==CROSS)
		{
			MoveToEx(*hdc, posX, posY, 0);
			LineTo(*hdc, posX + sizeX, posY + sizeY);
			MoveToEx(*hdc, posX + sizeX, posY, 0);
			LineTo(*hdc, posX, posY + sizeY);
		}
	}

	SelectObject(*hdc, oldPen);

}

void Tile::setHeight(int h, int pY)
{
	posY=pY;
	sizeY=h;
}

void Tile::setWidth(int w, int pX)
{
	posX=pX;
	sizeX=w;
}

void Tile::makeMove(MoveInfo& move, int depth)
{
	/*depth kommer inte användas här men den används i board*/
	if(owner==NONE)
	{
		owner=move.getOwner();
		activated=false;
		move.setPlacement(MoveInfo::WON);

	}
	else
	{
		move.setPlacement(MoveInfo::UNVALID);
	}
}

void Tile::makeDebugMove(MoveInfo& move, int depth)
{

	owner=move.getOwner();
	activated=false;
	move.setPlacement(MoveInfo::WON);

}

bool Tile::activateBoard()
{
	//assert(false);//ska bara anropas på board
	//return false;
	return activated;
}
void Tile::deactivateBoard()
{
	assert(false);//ska bara anropas på board
}

int Tile::determineValueForAI(Tile::Owner sign, int depth)//huvudsakligen till för Board. ska helst inte anropas
{
	return 0;
}

