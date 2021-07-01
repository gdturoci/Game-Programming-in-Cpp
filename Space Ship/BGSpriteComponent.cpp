#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder):
	SpriteComponent(owner, drawOrder),
	ScrollSpeed(0.0f) {}

void BGSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	for (auto& bg : BGTextures) {
		//Update the x offset
		bg.Offset.x += ScrollSpeed*deltaTime;

		//If this is completely off the screen, reset offset to the right of the last bg texture
		if (bg.Offset.x < -ScreenSize.x) bg.Offset.x = (BGTextures.size()-1)*ScreenSize.x-1;
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	//Draw each background texture
	for (auto& bg : BGTextures) {
		SDL_Rect r;

		//Assume screen size dimensions
		r.w = static_cast<int>(ScreenSize.x);
		r.h = static_cast<int>(ScreenSize.y);

		//Center the rectangle around the position of the owner
		r.x = static_cast<int>(Owner->GetPosition().x - r.w/2+bg.Offset.x);
		r.y = static_cast<int>(Owner->GetPosition().y - r.h/2+bg.Offset.y);

		//Draw this background
		SDL_RenderCopy(renderer, bg.Texture, nullptr, &r);
	}
}

void BGSpriteComponent::SetBGTextures(const vector<SDL_Texture*>& textures) {
	
	int count = 0;
	for (auto tex : textures) {
		BGTexture temp;
		temp.Texture = tex;

		//Each texture is screen width in offset
		temp.Offset.x = count*ScreenSize.x;
		temp.Offset.y = 0;
		BGTextures.emplace_back(temp);
		++count;
	}
}