#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
using namespace std;

Actor::Actor(Game* game):
	State(EActive),
	Position(Vector2::Zero),
	Scale(1.0f),
	Rotation(0.0f),
	mGame(game) {mGame->AddActor(this);}

Actor::~Actor() {
	mGame->RemoveActor(this);

	//need to delete components
	//Because ~Component calls RemoveComponent, need a different style loop
	while (!Components.empty()) delete Components.back();
}

void Actor::Update(float deltaTime) {
	if (State == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : Components) comp->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::AddComponent(Component* component) {
	//Find the insertion point in the sorted vector
	//The first element with an order higher than me
	int myOrder = component->GetUpdateOrder();
	auto iter = Components.begin();
	for (; iter != Components.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) break;
	}

	//Inserts element before position of iterator
	Components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = find(Components.begin(), Components.end(), component);
	if (iter != Components.end()) Components.erase(iter);
}