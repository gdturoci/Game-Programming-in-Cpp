#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"
using namespace std;

Tile::Tile(class Game* game):
	Actor(game),
	myParent(nullptr),
	f(0.0f),
	g(0.0f),
	h(0.0f),
	myBlocked(false),
	mySprite(nullptr),
	myTileState(EDefault),
	mySelected(false)
{
	mySprite = new SpriteComponent(this);
	UpdateTexture();
}

void Tile::SetTileState(TileState state) {
	myTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect() {
	mySelected = !mySelected;
	UpdateTexture();
}

void Tile::UpdateTexture() {
	string text;
	switch (myTileState) {
	case EStart:
		text = "Assets/TileTan.png";
		break;

	case EBase:
		text = "Assets/TileGreen.png";
		break;

	case EPath:
		if (mySelected) text = "Assets/TileGreySelected.png";
		else text = "Assets/TileGrey.png";
		break;
	case EDefault:
	default:
		if (mySelected) text = "Assets/TileBrownSelected.png";
		else text = "Assets/TileBrown.png";
		break;
	}
	mySprite->SetTexture(GetGame()->GetTexture(text));
}