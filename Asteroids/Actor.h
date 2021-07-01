#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>
using namespace std;

class Actor {
public:
	enum State {
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	//update function called from Game (not overridable)
	void Update(float deltaTime);
	//Updates all the componeents attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	//Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	//ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	//Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);

	//Getters/setters
	const Vector2& GetPosition() const { return myPosition; }
	void SetPosition(const Vector2& pos) { myPosition = pos; }
	float GetScale() const { return myScale; }
	void SetScale(float scale) { myScale = scale; }
	float GetRotation() const { return myRotation; }
	void SetRotation(float rotation) { myRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(myRotation), -Math::Sin(myRotation)); }

	State GetState() const {return myState; }
	void SetState(State state) { myState = state; }

	class Game* GetGame() { return myGame; }

	//Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	//Actor's state
	State myState;

	//Transform
	Vector2 myPosition;
	float myScale;
	float myRotation;

	vector<class Component*> myComponents;
	class Game* myGame;
};