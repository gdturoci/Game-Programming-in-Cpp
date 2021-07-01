#pragma once
#include <vector>
#include "Math.h"
using namespace std;

class Actor {
public:
	
	//Used to track state of actor
	enum State {
		EActive,
		EPaused,
		EDead
	};

	//Constructor/destructor
	Actor(class Game* game);
	virtual ~Actor();

	//Update function called from Game (not overridable)
	void Update(float deltaTime);

	//Updates all components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);

	//Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	//Getters/setters
	const Vector2& GetPosition() const {return Position;}
	void SetPosition(const Vector2& pos) {Position = pos;}
	float GetScale() const {return Scale;}
	void SetScale(float scale) {Scale = scale;}
	float GetRotation() const {return Rotation;}
	void SetRotation(float rotation) {Rotation = rotation;}

	State GetState() const {return State;}
	void SetState(State state) {State = state;}
	
	class Game* GetGame() {return mGame;}

	//Add/Remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:

	//Actor's state
	State State;

	//Transform
	Vector2 Position; //Center position of actor
	float Scale; //Uniforms scale of actor
	float Rotation; //Rotation angle in radians

	//Components held by this actor
	vector<class Component*> Components;
	class Game* mGame;
};