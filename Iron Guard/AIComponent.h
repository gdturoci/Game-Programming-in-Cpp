#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>
using namespace std;

class AIComponent : public Component {
public:
	AIComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void ChangeState(const string& name);

	//Add a new state to the map
	void RegisterState(class AIState* state);

private:
	//Maps name of state to AIState instance
	unordered_map<string, class AIState*> myStateMap;
	//Current state we're in
	class AIState* myCurrentState;
};