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

	int GetDrawOrder() const { return myDrawOrder; }
	int GetTexWidth() const { return myTexWidth; }
	int GetTexHeight() const { return myTexHeight; }

protected:
	SDL_Texture* myTexture;
	int myDrawOrder;
	int myTexWidth;
	int myTexHeight;
};