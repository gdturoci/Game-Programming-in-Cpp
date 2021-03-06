#pragma once
#include "Actor.h"

class Ship : public Actor {
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const {return RightSpeed;}
	float GetDownSpeed() const {return DownSpeed;}

private:
	float RightSpeed;
	float DownSpeed;
};