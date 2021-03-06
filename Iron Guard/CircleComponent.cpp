#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(class Actor* owner):
	Component(owner),
	myRadius(0.0f) {}

const Vector2& CircleComponent::GetCenter() const {
	return myOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
	return myOwner->GetScale()*myRadius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
	//Calculate distance squared
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	//Calculate sum of radii squared
	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq += radiiSq;

	return distSq <= radiiSq;
}