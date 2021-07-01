#pragma once

class Component {
public:

	//Constructor
	//The lower the update order, the earlier the component updates
	Component(class Actor* owner, int updateOrder = 100);
	
	//Destructor
	virtual ~Component();

	//Update this component by delta time
	virtual void Update(float deltaTime);
	int GetUpdateOrder() const {return UpdateOrder;}

protected:
	//Owning Actor
	class Actor* Owner;

	//Update order of component
	int UpdateOrder;
};