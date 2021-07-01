#pragma once
#include "Actor.h"

class Tower : public Actor {
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* myMove;
	float myNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange =  100.0f;
};