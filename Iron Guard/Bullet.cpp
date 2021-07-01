#include "Bullet.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet(class Game* game):
	Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.0f);

	myCircle = new CircleComponent(this);
	myCircle->SetRadius(5.0f);

	myLiveTime = 1.0f;
}

void Bullet::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//Check for collision vs enemies
	for (Enemy* e : GetGame()->GetEnemies()) {
		if (Intersect(*myCircle, *(e->GetCircle()))) {
			//We both die on collision
			e->SetState(EDead);
			SetState(EDead);
			break;
		}
	}

	myLiveTime -= deltaTime;
	if (myLiveTime <= 0.0f) SetState(EDead); //If I lived too long, die
}