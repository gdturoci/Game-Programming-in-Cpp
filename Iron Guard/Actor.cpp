#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
using namespace std;

Actor::Actor(Game* game) :
	myState(EActive),
	myPosition(Vector2::Zero),
	myScale(1.0f),
	myRotation(0.0f),
	myGame(game)
{
	myGame->AddActor(this);
}

Actor::~Actor() {
	myGame->RemoveActor(this);

	//Need to delete components bc ~Component calls RemoveComponent, needs a diff style loop
	while (!myComponents.empty()) delete myComponents.back();
}

void Actor::Update(float deltaTime) {
	if (myState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : myComponents) comp->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::ProcessInput(const uint8_t* keyState) {
	if (myState == EActive) {
		//First process input for components
		for (auto comp : myComponents) comp->ProcessInput(keyState);

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState) {}

void Actor::AddComponent(Component* component) {
	//Find the insertion point in the sorted vector; the first element w/ an order higher than me
	int myOrder = component->GetUpdateOrder();
	auto iter = myComponents.begin();
	for (; iter != myComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) break;
	}

	//Inserts element before position of iterator
	myComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = find(myComponents.begin(), myComponents.end(), component);
	if (iter != myComponents.end()) myComponents.erase(iter);
}