#pragma once
#include "Component.h"

class MoveComponent : public Component {
public:
	//Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return myAngularSpeed; }
	float GetForwardSPeed() const { return myForwardSpeed; }
	void SetAngularSpeed(float speed) { myAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { myForwardSpeed = speed; }

private:
	//Controls rotation in radians/second
	float myAngularSpeed;

	//Controls forward movement in units/second
	float myForwardSpeed;
};