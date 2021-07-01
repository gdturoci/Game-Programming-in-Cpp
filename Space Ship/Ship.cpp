#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
using namespace std;

Ship::Ship(Game* game):
	Actor(game),
	RightSpeed(0.0f),
	DownSpeed(0.0f) {
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
	};
	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//Update position based on speeds and deltaTime
	Vector2 pos = GetPosition();
	pos.x += RightSpeed*deltaTime;
	pos.y += DownSpeed*deltaTime;

	//Restrict position to left half of screen
	if (pos.x < 25.0f) pos.x = 25.0f;
	else if (pos.x > 500.0f) pos.x = 500.0f;
	if (pos.y < 25.0f) pos.y = 25.0f;
	else if (pos.y > 743.0f) pos.y = 743.0f;

	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state) {
	RightSpeed = 0.0f;
	DownSpeed = 0.0f;

	//Right/Left
	if (state[SDL_SCANCODE_RIGHT]) RightSpeed += 250.0f;
	if (state[SDL_SCANCODE_LEFT]) RightSpeed -= 250.0f;

	//Up/Down
	if (state[SDL_SCANCODE_UP]) DownSpeed -= 250.0f;
	if (state[SDL_SCANCODE_DOWN]) DownSpeed += 250.0f;
}