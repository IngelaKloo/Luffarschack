#pragma once

#include <Windows.h>
#include "Tile.h"
#include "Board.h"
#include <vector>

using namespace std;
class Player;
class GameManager
{
public:
	GameManager(HWND* hwnd, int bw, int bh);
	
	~GameManager();
	void run();
	void setBoardWidth(int w) ;
	void setBoardHeight(int h) ;
	void click(int curserPosX, int curserPosY);
	void decideMode(int curserPosX, int curserPosY);

	bool isStartingUp() {return startUp;}

	const bool debugMode;
	void debugClick(int curserPosX, int curserPosY, bool leftClick);

	void space();
private:
	HDC hdc;
	HWND* hwnd;

	bool startUp;
	
	Board* board;
	int boardWidth;
	int boardHeight;
	Player* player1;
	Player* player2;
	Player* turn;

};