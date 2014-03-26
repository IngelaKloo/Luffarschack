#include "Player.h"
#include "MoveInfo.h"

#include "Board.h"
#include "Player.h"
#include "MoveInfo.h"
#include "Defines.h"
#include <vector>
#include <cassert>

using namespace std;

MoveInfo Player::click(int curserX, int curserY)
{
	assert(kindOfPlayer==HUMAN);
	int posX=0;
	int posY=0;
	int width=board->getWidth();
	int heigth=board->getHeight();

	/*av någon anledning skickar den en clickhändelse när man startar spelet. Dess position är längst ner i högra hörnet. Detta ser till att den inte räknas*/
	if(curserX>=width || curserY>=heigth)
	{
		return MoveInfo();
	}
	vector<int>changedTile;
	for(int i=0; i<DEPTH; ++i)
	{
		int thirdOfWidth=width/3;
		int thirdOfHeight=heigth/3;
		int tileX=curserX/thirdOfWidth;
		int tileY=curserY/thirdOfHeight;
		int tileIndex=tileY*3+tileX;
		changedTile.push_back(tileIndex);
		width=thirdOfWidth;
		heigth=thirdOfHeight;
		curserX-=tileX*thirdOfWidth;
		curserY-=tileY*thirdOfHeight;

	}
	MoveInfo move(changedTile, sign);
	board->makeMove(move, 0);

	return move;

}

void Player::debugClick(int curserX, int curserY)
{
	int posX=0;
	int posY=0;
	int width=board->getWidth();
	int heigth=board->getHeight();

	vector<int>changedTile;
	for(int i=0; i<DEPTH; ++i)
	{
		int thirdOfWidth=width/3;
		int thirdOfHeight=heigth/3;
		int tileX=curserX/thirdOfWidth;
		int tileY=curserY/thirdOfHeight;
		int tileIndex=tileY*3+tileX;
		changedTile.push_back(tileIndex);
		width=thirdOfWidth;
		heigth=thirdOfHeight;
		curserX-=tileX*thirdOfWidth;
		curserY-=tileY*thirdOfHeight;

	}
	MoveInfo move(changedTile, sign);
	board->makeDebugMove(move, 0);
}

MoveInfo Player::space()
{
	assert(kindOfPlayer==AI);

	MoveInfo move= determineBestMove(board, 0);
	board->makeMove(move, 0);
	return move;
}

MoveInfo Player::determineBestMove(Board* boardBeforeMove, int depth)
{
	assert(kindOfPlayer==AI);

	Tile::Owner turn;

	if((sign==Tile::CROSS && depth%2==0)||(sign==Tile::RING && depth%2==1))
	{
		turn=Tile::CROSS;
	}
	else
	{
		turn=Tile::RING;
	}

	MoveInfo bestMove;
	
	if(depth %2==0)
	{
		bestMove.setValueForAI(-POINTS_FOR_WIN*DEPTH_ZERO_FACTOR-1);
	}
	else
	{
		bestMove.setValueForAI(POINTS_FOR_WIN*DEPTH_ZERO_FACTOR+1);
	}

	if(depth>=AI_SEARCH_TREE_DEPTH)
	{
		int AIvalue=boardBeforeMove->determineValueForAI(sign, 0);
		bestMove.setValueForAI(AIvalue);
		return bestMove;
	}

	/*gå igenom alla brädrutor*/
	for(unsigned int i=0; i<9; ++i)
	{
		/*om brädrutan är aktiverad (dvs om man kan lägga på den)*/
		if(boardBeforeMove->getTiles()[i]->isActive())
		{
			const Board* activeBoardTile=dynamic_cast<Board*>(boardBeforeMove->getTiles()[i]);
			for(unsigned int j=0; j<9; ++j)
			{
				const Tile* activeTile=activeBoardTile->getTiles()[j];//kanske bättre att ha den inne i if-satsen
				/*om man kan lägga på rutan (dvs. om den  inte är upptagen)*/
				if(activeTile->isActive())
				{
					/*skapa det faktiska draget*/
					vector<int>validMove;
					validMove.push_back(i);
					validMove.push_back(j);
					MoveInfo move(validMove, turn);
					
					Board boardAfterMove=boardBeforeMove->makeResultOfMove(move);//move har vilken tile som ändras men next move har det inte

					MoveInfo nextMove=determineBestMove(&boardAfterMove, depth+1);

					if((turn==sign && bestMove.getValueForAI()<nextMove.getValueForAI()))//||(turn!=sign && bestMove.getValueForAI()>nextMove.getValueForAI()))
					{
						bestMove=move;
						bestMove.setValueForAI(nextMove.getValueForAI());
					}
					else if((turn!=sign && bestMove.getValueForAI()>nextMove.getValueForAI()))
					{
						bestMove=move;
						bestMove.setValueForAI(nextMove.getValueForAI());
					}
				}
			}
		}
	}
	//om det inte finns något drag efter detta
	if(bestMove.getValueForAI()>POINTS_FOR_WIN*DEPTH_ZERO_FACTOR||bestMove.getValueForAI()<-POINTS_FOR_WIN*DEPTH_ZERO_FACTOR)
	{
		int AIvalue=boardBeforeMove->determineValueForAI(sign, 0);
		bestMove.setValueForAI(AIvalue);
	}
	return bestMove;
}