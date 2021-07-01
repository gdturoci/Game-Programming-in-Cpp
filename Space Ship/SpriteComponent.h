#pragma once
#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component {
public:
	//Lower draw order corresponds with further back
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const {return DrawOrder;}
	int GetTexHeight() const {return TexHeight;}
	int GetTexWidth() const {return TexWidth;}

protected:
	//Texture to draw
	SDL_Texture* Texture;

	//Draw order used for painter's algorithm
	int DrawOrder;

	//Width/height of texture
	int TexWidth;
	int TexHeight;
};