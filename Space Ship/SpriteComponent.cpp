#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder):
	Component(owner),
	Texture(nullptr),
	DrawOrder(drawOrder),
	TexWidth(0),
	TexHeight(0)
	{Owner->GetGame()->AddSprite(this);}

SpriteComponent::~SpriteComponent() {
	Owner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (Texture) {
		SDL_Rect r;

		//Scale the width/height by owner's scale
		r.w = static_cast<int>(TexWidth*Owner->GetScale());
		r.h = static_cast<int>(TexHeight*Owner->GetScale());

		//Center the rectangle around the position of the owner
		r.x = static_cast<int>(Owner->GetPosition().x - r.w/2);
		r.y = static_cast<int>(Owner->GetPosition().y - r.h/2);

		//Draw
		SDL_RenderCopyEx(renderer,
			Texture, //Texture to draw
			nullptr, //Source rectangle
			&r, //Destination rectangle
			-Math::ToDegrees(Owner->GetRotation()), //Convert angle
			nullptr, //Point of rotation
			SDL_FLIP_NONE); //Flip behavior
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	Texture = texture;

	//Get width/height of texture
	SDL_QueryTexture(texture, nullptr, nullptr, &TexWidth, &TexHeight);
}