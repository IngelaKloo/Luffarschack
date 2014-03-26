#pragma once
#include "Tile.h"
#include <vector>

class MoveInfo
{
public:
	enum Placement
	{
		UNVALID,
		VALIDNOTWON,
		WON
	};
	//MoveInfo(Tile::Owner o, Placement p, int tnr):owner(o), placement(p), tileNr(tnr) {}
	//MoveInfo():owner(Tile::NONE), placement(MISSED), tileNr(-1) {}
	MoveInfo(std::vector<int> ct,Tile::Owner o):changedTile(ct), owner(o){}
	MoveInfo():placement(UNVALID){}
	
	Tile::Owner getOwner() {return owner;}
	Placement getPlacement() {return placement;}

	int getActivatedBoard() {return activatedBoard;}
	void setActivatedBoard(int ab){activatedBoard=ab;}

	void setPlacement(Placement p){placement=p;}
	void setOwner(Tile::Owner o){owner=o;}

	std::vector<int> getChangedTile() {return changedTile;}
	void setChangedTile(std::vector<int> ct){changedTile=ct;}

	int getValueForAI() const{return valueForAI;}
	void setValueForAI(int vfai){valueForAI=vfai;}

	

private:
	Tile::Owner owner;
	Placement placement;
	int activatedBoard;
	std::vector<int> changedTile;
	int valueForAI;
};