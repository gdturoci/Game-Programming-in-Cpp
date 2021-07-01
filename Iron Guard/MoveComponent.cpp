#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) :
	Component(owner, updateOrder),
	myAngularSpeed(0.0f),
	myForwardSpeed(0.0f)
{}

void MoveComponent::Update(float deltaTime) {
	//Don't bother updating the rotation or position of the actor
	//if the corresponding speed is near 0
	if (!Math::NearZero(myAngularSpeed)) {
		float rot = myOwner->GetRotation();
		rot += myAngularSpeed * deltaTime;
		myOwner->SetRotation(rot);
	}
	if (!Math::NearZero(myForwardSpeed)) {
		Vector2 pos = myOwner->GetPosition();
		pos += myOwner->GetForward() * myForwardSpeed * deltaTime;
		//myOwner->SetPosition(pos); this would go here if not for screen wrapping

		//Screen wrapping
		if (pos.x < 0.0f) pos.x = 1022.0f;
		else if (pos.x > 1024.0f) pos.x = 2.0f;

		if (pos.y < 0.0f) pos.y = 766.0f;
		else if (pos.y > 768.0f) pos.y = 2.0f;

		myOwner->SetPosition(pos);
	}
}