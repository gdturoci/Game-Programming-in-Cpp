#pragma once
#include "Actor.h"
#include <vector>
using namespace std;

class Tile : public Actor {
public:
	friend class Grid;
	enum TileState {
		EDefault,
		EPath,
		EStart,
		EBase
	};

	Tile(class Game* game);

	void SetTileState(TileState state);
	TileState GetTileState() const {return myTileState;}
	void ToggleSelect();
	const Tile* GetParent() const {return myParent;}

private:
	//For pathfinding
	vector<Tile*> myAdjacent;
	Tile* myParent;
	float f;
	float g;
	float h;
	bool myInOpenSet;
	bool myInClosedSet;
	bool myBlocked;

	void UpdateTexture();
	class SpriteComponent* mySprite;
	TileState myTileState;
	bool mySelected;
};