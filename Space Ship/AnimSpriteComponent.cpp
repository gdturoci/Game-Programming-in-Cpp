#include "AnimSpriteComponent.h"
#include "Math.h"
using namespace std;

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder):
	SpriteComponent(owner, drawOrder),
	CurrFrame(0.0f),
	AnimFPS(24.0f) {}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (AnimTextures.size() > 0) {
		//Update the current frame based on frame rate and delta time
		CurrFrame += AnimFPS*deltaTime;

		//Wrap current frame if needed
		while (CurrFrame >= AnimTextures.size()) CurrFrame -= AnimTextures.size();

		//Set the current texture
		SetTexture(AnimTextures[static_cast<int>(CurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const vector<SDL_Texture*>& textures) {
	//Set the AnimTextures variable to the provided vector and reset CurrFrame to 0
	AnimTextures = textures;
	if (AnimTextures.size() > 0) {
		//Set the active texture to first frame
		CurrFrame = 0.0f;
		SetTexture(AnimTextures[0]);
	}
}