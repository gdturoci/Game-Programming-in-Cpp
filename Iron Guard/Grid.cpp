#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>
using namespace std;

Grid::Grid(class Game* game):
	Actor(game),
	mySelectedTile(nullptr)
{
	//7 rows, 16 columns
	myTiles.resize(NumRows);
	for (size_t x=0; x<myTiles.size(); ++x)	myTiles[x].resize(NumCols);

	//Create tiles
	for (size_t x=0; x<NumRows; ++x) {
		for (size_t y=0; y<NumCols; ++y) {
			myTiles[x][y] = new Tile(GetGame());
			myTiles[x][y]->SetPosition(Vector2(TileSize/2.0f + y * TileSize, StartY + x * TileSize));
		}
	}

	//Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	//Set up adjacency lists
	for (size_t x=0; x<NumRows; ++x) {
		for (size_t y=0; y<NumCols; ++y) {
			if (x > 0) 
				myTiles[x][y]->myAdjacent.push_back(myTiles[x-1][y]);
			if (x < NumRows - 1)
				myTiles[x][y]->myAdjacent.push_back(myTiles[x+1][y]);
			if (y > 0) 
				myTiles[x][y]->myAdjacent.push_back(myTiles[x][y-1]);
			if (y < NumCols - 1) 
				myTiles[x][y]->myAdjacent.push_back(myTiles[x][y+1]);
		}
	}

	//Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	myNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col) {
	//Make sure it's a valid selection
	Tile::TileState tstate = myTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) {
		//Deselect previous one
		if (mySelectedTile)	mySelectedTile->ToggleSelect();
		mySelectedTile = myTiles[row][col];
		mySelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y) {
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0) {
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
			SelectTile(y, x);
	}
}

//Implements A* pathfinding
bool Grid::FindPath(Tile* start, Tile* goal) {
	for (size_t x=0; x<NumRows; ++x) {
		for (size_t y=0; y<NumCols; ++y) {
			myTiles[x][y]->g = 0.0f;
			myTiles[x][y]->myInOpenSet = false;
			myTiles[x][y]->myInClosedSet = false;
		}
	}

	vector<Tile*> openSet;

	//Set current node to start, and add to closed set
	Tile* current = start;
	current->myInClosedSet = true;

	do {
		//Add adjacent nodes to open set
		for (Tile* neighbor : current->myAdjacent) {
			if (neighbor->myBlocked) continue;

			//Only check nodes that aren't in the closed set
			if (!neighbor->myInClosedSet) {
				if (!neighbor->myInOpenSet)	{
					//Not in the open set, so set parent
					neighbor->myParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					//g(x) is the parent's g plus cost of traversing edge
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->myInOpenSet = true;
				} else {
					//Compute g(x) cost if current becomes the parent
					float newG = current->g + TileSize;
					if (newG < neighbor->g) {
						//Adopt this node
						neighbor->myParent = current;
						neighbor->g = newG;
						//f(x) changes because g(x) changes
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		//If open set is empty, all possible paths are exhausted
		if (openSet.empty()) break;

		//Find lowest cost node in open set
		auto iter = min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->f;
			});
		//Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		current->myInOpenSet = false;
		current->myInClosedSet = true;
	}
	while (current != goal);

	//Did we find a path?
	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start) {
	//Reset all tiles to normal (except for start/end)
	for (size_t x=0; x<NumRows; ++x) {
		for (size_t y=0; y < NumCols; ++y) {
			if (!(x == 3 && y == 0) && !(x == 3 && y == 15)) myTiles[x][y]->SetTileState(Tile::EDefault);
		}
	}

	Tile* t = start->myParent;
	while (t != GetEndTile()) {
		t->SetTileState(Tile::EPath);
		t = t->myParent;
	}
}

void Grid::BuildTower() {
	if (mySelectedTile && !mySelectedTile->myBlocked) {
		mySelectedTile->myBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile())) {
			Tower* t = new Tower(GetGame());
			t->SetPosition(mySelectedTile->GetPosition());
		} else {
			// This tower would block the path, so don't allow build
			mySelectedTile->myBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile() {
	return myTiles[3][0];
}

Tile* Grid::GetEndTile() {
	return myTiles[3][15];
}

void Grid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// Is it time to spawn a new enemy?
	myNextEnemy -= deltaTime;
	if (myNextEnemy <= 0.0f) {
		new Enemy(GetGame());
		myNextEnemy += EnemyTime;
	}
}