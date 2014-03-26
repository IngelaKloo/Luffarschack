#include "GameManager.h"
#include "Defines.h"
#include "BoardLight.h"
#include "BoardLightTree.h"
#include "BoardLightNode.h"
#include "Player.h"
#include <stdlib.h>
#include <string>


GameManager::GameManager(HWND* hwnd, int bw, int bh):boardWidth(bw), boardHeight(bh), debugMode(false)
{
	this->hwnd=hwnd;
	hdc=GetDC(*hwnd);

	startUp=true;

	board=new Board(0,0,boardWidth, boardHeight, DEPTH);

	if(debugMode)
	{
		player1= new Player(board,Tile::CROSS, Player::HUMAN);
		player2=new Player(board, Tile::RING, Player::HUMAN);
		turn=player1;
		startUp=false;
	}
}

GameManager::~GameManager()
{
	delete board;
	if(!startUp)
	{
		delete player1;
		delete player2;
	}
	ReleaseDC(*hwnd, hdc);
}



void GameManager::run()
{
	if(debugMode)
	{
		RECT TextBox;
		TextBox.top = 0;
		TextBox.left = 0;
		TextBox.bottom = 30;
		TextBox.right = 100;

		char AIValueArrayBeginning[21]="kryss värde: ";
		char AIValueArrayEnd[6];
		itoa(board->determineValueForAI(Tile::CROSS, 0),AIValueArrayEnd, 10);
		strncat(AIValueArrayBeginning, AIValueArrayEnd, 6);
		int length=strlen(AIValueArrayBeginning);

		wchar_t wtext[21];
		mbstowcs(wtext, AIValueArrayBeginning, strlen(AIValueArrayBeginning)+1);//Plus null
		LPWSTR ptr = wtext;

		//char AIValueArray[]=(char*)AIValueString;
		//int arrayLength=AIValueArray.length();
		//LPCWSTR hej=(LPCWSTR)AIValueArray;
		////LPWSTR AIValueString = L"kryss värde: ";
		////int AIValue=board->determineValueForAI(Tile::CROSS, 0);
		////LPWSTR fullAIValueString=AIValueString+AIValue;//´plus betyder nog inte vad jag tror
		////DrawText(hdc,fullAIValueString,100,&TextBox,DT_LEFT|DT_TOP);
		TextOut(hdc,0,0,ptr, strlen(AIValueArrayBeginning));

	}
	if(!startUp)
	{
		board->render(&hdc);

	}
	else 
	{
		HPEN pen=CreatePen(PS_SOLID,4,RGB(0,0,0));
		HPEN oldPen;
		oldPen=(HPEN)SelectObject(hdc, pen);

		MoveToEx(hdc, 0, boardHeight/2, 0);
		LineTo(hdc, boardWidth, boardHeight/2);
		MoveToEx(hdc, boardWidth/2, 0, 0);
		LineTo(hdc, boardWidth/2, boardHeight);

		LPWSTR onlyHuman=L"Två människor";
		LPWSTR humanFirst=L"En AI, En Människa, Människa Börjar";
		LPWSTR AIFirst=L"En AI, En Människa, AI Börjar";
		LPWSTR onlyAI=L"Två AI:r";

		RECT TextBox;
		TextBox.top = boardHeight/5;
		TextBox.left = boardWidth/5;
		TextBox.bottom = 4*boardHeight/5;
		TextBox.right = 4*boardWidth/5;

		DrawText(hdc,onlyHuman,14,&TextBox,DT_LEFT|DT_TOP);
		DrawText(hdc,humanFirst,36,&TextBox,DT_RIGHT|DT_TOP);
		DrawText(hdc,AIFirst,30,&TextBox,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		DrawText(hdc,onlyAI,9,&TextBox,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);

		(HPEN)SelectObject(hdc, oldPen);
		DeleteObject(pen);

	}

}

void GameManager::setBoardWidth(int w)
{
	board->setWidth(w, 0);
}

void GameManager::setBoardHeight(int h)
{
	board->setHeight(h, 0);
}

void GameManager::click(int curserPosX, int curserPosY)
{
	if(startUp)
	{
		return;
	}

	if(turn->kindOfPlayer!=Player::HUMAN)
	{
		return;
	}
	if(turn->click(curserPosX, curserPosY).getPlacement()!=MoveInfo::UNVALID)
	{
		if(turn==player1)
		{
			turn=player2;
		}
		else
		{
			turn=player1;
		}
	}
}

void GameManager::debugClick(int curserPosX, int curserPosY, bool leftClick)
{
	if(leftClick)
	{
		player1->debugClick(curserPosX, curserPosY);
	}
	else
	{
		player2->debugClick(curserPosX, curserPosY);
	}
}

void GameManager::decideMode(int curserX, int curserY)
{
	if(curserX<boardWidth/2)
	{
		if(curserY<boardHeight/2)
		{
			player1= new Player(board,Tile::CROSS, Player::HUMAN);
			player2=new Player(board, Tile::RING, Player::HUMAN);
			turn=player1;
		}
		else
		{
			player1= new Player(board,Tile::CROSS, Player::HUMAN);
			player2=new Player(board, Tile::RING, Player::AI);
			turn=player2;
		}
	}
	else
	{
		if(curserY<boardHeight/2)
		{
			player1= new Player(board,Tile::CROSS, Player::HUMAN);
			player2=new Player(board, Tile::RING, Player::AI);
			turn=player1;
		}
		else
		{
			player1= new Player(board,Tile::CROSS, Player::AI);
			player2=new Player(board, Tile::RING, Player::AI);
			turn=player1;
		}
	}
	startUp=false;
}

void GameManager::space()
{
	if(startUp)
	{
		return;
	}
	if(turn->kindOfPlayer!=Player::AI)
	{
		return;
	}
	if(turn->space().getPlacement()!=MoveInfo::UNVALID)
	{
		if(turn==player1)
		{
			turn=player2;
		}
		else
		{
			turn=player1;
		}
	}
}
