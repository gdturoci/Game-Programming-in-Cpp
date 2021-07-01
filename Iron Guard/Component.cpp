#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder) :
	myOwner(owner),
	myUpdateOrder(updateOrder)
{
	myOwner->AddComponent(this);
} //Add to actor's vector of components

Component::~Component() {
	myOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {}