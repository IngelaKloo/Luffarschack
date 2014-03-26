#include "Board.h"
#include "Tile.h"
#include "Defines.h"
#include <cassert>


vector<Row> Board::allPossibleRows;
Board::Board(int pX, int pY, int sX, int sY, int depth):Tile(pX, pY, sX, sY)
{
	int penSize=depth*2;
	DeleteObject(blackPen);
	DeleteObject(redPen);
	blackPen=CreatePen(PS_SOLID,penSize,RGB(0,0,0));
	redPen=CreatePen(PS_SOLID,penSize,RGB(255,0,0));

	int thirdOfSizeX=sizeX/3;
	int thirdOfSizeY=sizeY/3;
	if(depth > 1)
	{
		for(int i=0; i<3; ++i)
		{
			for (int j=0; j<3; ++j)
			{
				tiles.push_back(new Board(posX + i*thirdOfSizeX, posY + j*thirdOfSizeY, thirdOfSizeX, thirdOfSizeY, depth-1));
			}
		}
	}
	else
	{
		for(int i=0; i<3; ++i)
		{
			for (int j=0; j<3; ++j)
			{
				tiles.push_back(new Tile(posX + i*thirdOfSizeX, posY + j*thirdOfSizeY, thirdOfSizeX, thirdOfSizeY));
			}
		}
	}
	/*definiera alla rader*/
	if(allPossibleRows.empty())
	{
		/*vågräta*/
		allPossibleRows.push_back(Row(0, 1, 2));
		allPossibleRows.push_back(Row(3, 4, 5));
		allPossibleRows.push_back(Row(6, 7, 8));
		/*lodräta*/
		allPossibleRows.push_back(Row(0, 3, 6));
		allPossibleRows.push_back(Row(1, 4, 7));
		allPossibleRows.push_back(Row(2, 5, 8));
		/*diagonala*/
		allPossibleRows.push_back(Row(0, 4, 8));
		allPossibleRows.push_back(Row(2, 4, 6));
	}
}

Board::Board(const Board& b):Tile(b.posX, b.posY, b.sizeX, b.sizeY)
{
	activated=b.activated;
	for(unsigned int i=0; i<b.tiles.size(); ++i)
	{
		
		if(Board* board=dynamic_cast<Board*>(b.tiles[i]))//tiles.push_back(new Tile(*b.tiles[i]));//pekare till en kopior av b:s tiles
		{
			tiles.push_back(new Board(*board));//rekursivt men ok
		}
		else
		{
			tiles.push_back(new Tile(*b.tiles[i]));
		}
	}
	owner=b.owner;
}
Board Board::makeResultOfMove( MoveInfo& move)//om man kan göra så att denna håller på med referenser istället för att returnera en hel board vore det bra
{
	Board boardAfterMove(*this);
	boardAfterMove.makeMove(move, 0);
	assert(move.getPlacement()!=MoveInfo::UNVALID);
	return boardAfterMove;
}


Board:: ~Board()
{
	for(unsigned int i=0; i<tiles.size(); ++i)
	{
		delete tiles[i];
	}
}

void Board::render(HDC* hdc)
{
	HPEN oldPen;
	if(!activated)
	{
		oldPen=(HPEN)SelectObject(*hdc, blackPen);
	}
	else
	{
		oldPen=(HPEN) SelectObject(*hdc, redPen);
	}
	Tile::render(hdc);
	for(int i=0; i<(int)tiles.size();++i)
	{
		tiles[i]->render(hdc);
	}
	int thirdOfX=sizeX/3;
	int thirdOfY=sizeY/3;

	MoveToEx(*hdc, posX + thirdOfX, posY, 0);
	LineTo(*hdc, posX + thirdOfX, posY+sizeY);
	MoveToEx(*hdc, posX + 2 * thirdOfX, posY, 0);
	LineTo(*hdc, posX + 2 * thirdOfX, posY + sizeY);
	MoveToEx(*hdc, posX , posY+ thirdOfY, 0);
	LineTo(*hdc, posX + sizeX, posY+ thirdOfY);
	MoveToEx(*hdc, posX , posY + 2 * thirdOfY, 0);
	LineTo(*hdc, posX + sizeX, posY + 2 * thirdOfY);





	SelectObject(*hdc, oldPen);
}

void Board::setHeight(int h, int pY)
{
	int thirdOfNewHeight=h/3;
	sizeY=h;
	posY=pY;
	for(unsigned int i=0; i<3;++i)
	{
		for(unsigned int j=0; j<3;++j)
		{
			tiles[i*3+j]->setHeight(thirdOfNewHeight,posY+i*thirdOfNewHeight );
		}
	}
}

void Board::setWidth(int w, int pX)
{
	int thirdOfNewWidth=w/3;
	sizeX=w;
	posX=pX;
	for(unsigned int i=0; i<3;++i)
	{
		for(unsigned int j=0; j<3;++j)
		{
			tiles[i*3+j]->setWidth(thirdOfNewWidth,posX+j*thirdOfNewWidth );
		}
	}
}

void Board::determineOutcome(MoveInfo& moveInfo)
{
	Owner moveOwner=moveInfo.getOwner();
	if(tiles[4]->getOwner()==moveOwner)
	{
		if(tiles[0]->getOwner()==moveOwner && tiles[8]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
		if(tiles[2]->getOwner()==moveOwner && tiles[6]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
		if(tiles[3]->getOwner()==moveOwner && tiles[5]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
		if(tiles[1]->getOwner()==moveOwner && tiles[7]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
	}
	if(tiles[0]->getOwner()==moveOwner)
	{
		if(tiles[1]->getOwner()==moveOwner && tiles[2]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
		if(tiles[3]->getOwner()==moveOwner && tiles[6]->getOwner()==moveOwner)
		{
			owner=moveOwner;
			return;
		}
	}
	if(tiles[1]->getOwner()==moveOwner && tiles[4]->getOwner()==moveOwner && tiles[7]->getOwner()==moveOwner)
	{
		owner=moveOwner;
		return;
	}
	if(tiles[2]->getOwner()==moveOwner && tiles[5]->getOwner()==moveOwner && tiles[8]->getOwner()==moveOwner)
	{
		owner=moveOwner;
		return;
	}
	if(tiles[6]->getOwner()==moveOwner && tiles[7]->getOwner()==moveOwner && tiles[8]->getOwner()==moveOwner)
	{
		owner=moveOwner;
		return;
	}
	moveInfo.setPlacement(MoveInfo::VALIDNOTWON);
}

void Board::makeMove(MoveInfo& move, int depth)
{
	if(activated)
	{
		int hitTile=move.getChangedTile()[depth];
		tiles[hitTile]->makeMove(move, depth+1);

		if(move.getPlacement()==MoveInfo::WON)
		{
			determineOutcome(move);
		}
	}
	else
	{
		move=MoveInfo();
		return;
	}
	if(depth==0 && move.getPlacement()!=MoveInfo::UNVALID)
	{
		/*deaktivera alla boards*/
		for(int i=0; i<9; ++i)
		{
			tiles[i]->deactivateBoard();
		}
		int activatedBoardIndex=move.getChangedTile()[move.getChangedTile().size()-1];//returnerar sista talet i changedTile

		if(!tiles[activatedBoardIndex]->activateBoard())//försöker aktivera det bräde som ska aktiveras om det inte är taget
		{
			bool successfullActivation=false;
			/*om det är taget och inte blir aktiverat...*/
			for(int i=0; i<9; ++i)
			{
				tiles[i]->activateBoard();//aktivera alla bräden som går
			}
		}
	}
}

void Board::makeDebugMove(MoveInfo& move, int depth)
{
	
		int hitTile=move.getChangedTile()[depth];
		tiles[hitTile]->makeMove(move, depth+1);

		if(move.getPlacement()==MoveInfo::WON)
		{
			determineOutcome(move);
		}
}

bool Board::tileIsTaken(int tile)
{
	assert(tile<9);
	if(tiles[tile]->getOwner()!=Tile::NONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Board::activateBoard()
{
	if(owner!=Tile::NONE)
	{
		activated=false;
		return false;
	}
	else
	{
		bool activeTileExists=false;
		for( unsigned int i=0; i<tiles.size(); ++i)
		{
			if(tiles[i]->isActive())
			{
				activeTileExists=true;
				i=tiles.size();
			}
		}
		if(activeTileExists)
		{
			activated=true;
			return true;
		}
		else
		{
			activated=false;
			return false;
		}
	}
}
void Board::deactivateBoard()
{
	activated=false;
}

int Board::determineValueForAI(Tile::Owner sign, int depth)//misstänkt
{
	Tile::Owner opponent;
	if(sign==Tile::CROSS)
	{
		opponent=Tile::RING;
	}
	else
	{
		opponent=Tile::CROSS;
	}
	int valueOfTiles=0;
	if(depth<DEPTH)
	{
		for(unsigned int i=0; i<tiles.size();++i)
		{
			valueOfTiles+=tiles[i]->determineValueForAI(sign, depth+1);
		}
	}
	int valueOfBoard=0;
	if(owner==Tile::NONE)
	{
		/*gå igenom alla raden*/
		for(unsigned int i=0; i<allPossibleRows.size(); ++i)
		{
			int rowElement1=allPossibleRows[i].element1;
			int rowElement2=allPossibleRows[i].element2;
			int rowElement3=allPossibleRows[i].element3;
			/*om denna raden går att ta*/
			if(!(tiles[rowElement1]->getOwner()==opponent || tiles[rowElement2]->getOwner()==opponent || tiles[rowElement3]->getOwner()==opponent))
			{
				/*räkna hur många tiles i raden som är tagna*/
				int numberOfTakenTiles=0;
				if(tiles[rowElement1]->getOwner()==sign)
				{
					++numberOfTakenTiles;
				}
				if(tiles[rowElement2]->getOwner()==sign)
				{
					++numberOfTakenTiles;
				}
				if(tiles[rowElement3]->getOwner()==sign)
				{
					++numberOfTakenTiles;
				}
				if(numberOfTakenTiles==0)
				{
					//inget
				}
				else if(numberOfTakenTiles==1)
				{
					valueOfBoard+=POINTS_FOR_ONE;
				}
				else if(numberOfTakenTiles==2)
				{
					valueOfBoard+= POINTS_FOR_TWO;
				}
			}
			/*om denna raden kan tas av motståndaren*/
			else if(!(tiles[rowElement1]->getOwner()==sign || tiles[rowElement2]->getOwner()==sign || tiles[rowElement3]->getOwner()==sign))
			{
				int numberOfTakenTiles=0;
				if(tiles[rowElement1]->getOwner()==opponent)
				{
					++numberOfTakenTiles;
				}
				if(tiles[rowElement2]->getOwner()==opponent)
				{
					++numberOfTakenTiles;
				}
				if(tiles[rowElement3]->getOwner()==opponent)
				{
					++numberOfTakenTiles;
				}
				if(numberOfTakenTiles==0)
				{
					//inget
				}
				else if(numberOfTakenTiles==1)
				{
					valueOfBoard-=POINTS_FOR_ONE;
				}
				else if(numberOfTakenTiles==2)
				{
					valueOfBoard-= POINTS_FOR_TWO;
				}
			}
		}
	}
	else if(owner==sign)
	{
		valueOfBoard+=POINTS_FOR_WIN;
	}
	else if(owner==opponent)
	{
		valueOfBoard-=POINTS_FOR_WIN;
	}

	if(depth==0)
	{
		valueOfBoard*=DEPTH_ZERO_FACTOR;
	}
	else if(depth==1)
	{
		valueOfBoard*=DEPTH_ONE_FACTOR;
	}
	else
	{
		/*om man inte har stöd så här långt ner*/
		assert(false);
	}
	if(owner==Tile::NONE)
	{
		return valueOfBoard+valueOfTiles;
	}
	else//ignorera tiles om brädet är vunnet
	{
		return valueOfBoard;
	}
}

