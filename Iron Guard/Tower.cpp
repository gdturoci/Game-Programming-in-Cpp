#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(class Game* game):
	Actor(game)
	{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));

	myMove = new MoveComponent(this);
	//myMove->SetAngularSpeed(Math::Pi); - I think this is for moving towers

	myNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//Decrement timer for attack
	myNextAttack -= deltaTime;
	//If it's time to attack
	if (myNextAttack <= 0.0f) {
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition()); //Find next enemy
		if (e != nullptr) { //If an enemy was found
			//Vector from me to enemy
			Vector2 dir = e->GetPosition() - GetPosition();
			float dist = dir.Length();
			if (dist < AttackRange) { //If nearest enemy is in attack range
				//Rotate to face enemy
				SetRotation(Math::Atan2(-dir.y, dir.x));
				//Spawn bullet at tower position facing enemy
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		myNextAttack += AttackTime; //Reset atack timer
	}
}