#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder):
	Owner(owner),
	UpdateOrder(updateOrder)
	{Owner->AddComponent(this);}

Component::~Component() {
	Owner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {}