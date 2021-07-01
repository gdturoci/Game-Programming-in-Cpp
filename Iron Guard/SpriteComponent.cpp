#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder):
	Component(owner),
	myTexture(nullptr),
	myDrawOrder(drawOrder),
	myTexWidth(0),
	myTexHeight(0)
	{ myOwner->GetGame()->AddSprite(this); }

SpriteComponent::~SpriteComponent() {
	myOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (myTexture) {
		SDL_Rect r;
		//Scale width/height by owner's scale
		r.w = static_cast<int>(myTexWidth * myOwner->GetScale());
		r.h = static_cast<int>(myTexHeight * myOwner->GetScale());
		//Center the rectangle around the position of the owner
		r.x = static_cast<int>(myOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(myOwner->GetPosition().y - r.h / 2);

		//Draw; have to convert angle from radians to degrees, and clockwise to counter
		SDL_RenderCopyEx(renderer, myTexture, nullptr, &r,
			-Math::ToDegrees(myOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	myTexture = texture;
	//Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &myTexWidth, &myTexHeight);
}