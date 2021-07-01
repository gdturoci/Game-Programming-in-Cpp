#pragma once
#include "SpriteComponent.h"
#include <vector>
using namespace std;

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);

	//Update animation every frame (overridden from component)
	void Update(float deltaTime) override;

	//Set the textures used for animation
	void SetAnimTextures(const vector<SDL_Texture*>& textures);

	//Set/get the animation FPS
	float GetAnimFPS() const {return AnimFPS;}
	void SetAnimFPS(float fps) {AnimFPS = fps;}

private:
	//All textures in the animation
	vector<SDL_Texture*> AnimTextures;

	//Current frame displayed
	float CurrFrame;

	//Animation frame rate
	float AnimFPS;
};