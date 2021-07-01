#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL_log.h>

AIComponent::AIComponent(class Actor* owner):
	Component(owner),
	myCurrentState(nullptr) {}

void AIComponent::Update(float deltaTime) {
	if (myCurrentState) myCurrentState->Update(deltaTime);
}

void AIComponent::ChangeState(const string& name) {
	//First exit the current state
	if (myCurrentState) myCurrentState->OnExit();

	//Try to find the state we're changing to in the map
	auto iter = myStateMap.find(name);
	if (iter != myStateMap.end()) {
		myCurrentState = iter->second;
		//Entering the new state
		myCurrentState->OnEnter();
	} else {
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		myCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state) {
	myStateMap.emplace(state->GetName(), state);
}