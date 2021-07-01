#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"
using namespace std;

class BGSpriteComponent : public SpriteComponent {
public:
	//set draw order to default to lower (so it's in the background)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	//Update/draw overridden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	//Set the textures used for the background
	void SetBGTextures(const vector<SDL_Texture*>& textures);

	//Get/set screen size and scroll speed
	void SetScreenSize(const Vector2& size) {ScreenSize=size;}
	void SetScrollSpeed(float speed) {ScrollSpeed = speed;}
	float GetScrollSpeed() const {return ScrollSpeed;}

private:
	//Struct to encapsulate each BG image and its offset
	struct BGTexture {
		SDL_Texture* Texture;
		Vector2 Offset;
	};

	vector<BGTexture> BGTextures;
	Vector2 ScreenSize;
	float ScrollSpeed;
};