#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder):
	MoveComponent(owner, updateOrder),
	myNextNode(nullptr) {}

void NavComponent::Update(float deltaTime) {
	if (myNextNode) {
		//If we're at the next node, advance along path
		Vector2 diff = myOwner->GetPosition() - myNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f)) {
			myNextNode = myNextNode->GetParent();
			TurnTo(myNextNode->GetPosition());
		}
	}

	//Moves the actor forward
	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start) {
	myNextNode = start->GetParent();
	TurnTo(myNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos) {
	//Vector from me to pos
	Vector2 dir = pos - myOwner->GetPosition();
	//New angle is just atan2 of this dir vector; negate y bc +y is down
	float angle = Math::Atan2(-dir.y, dir.x);
	myOwner->SetRotation(angle);
}