#include "Component.h"
#include "Actor.h"
#include "InputSystem.h"

Component::Component(Actor* ownerP, int updateOrderP):
	owner(*ownerP),
	updateOrder(updateOrderP),
	type(ComponentType::Unknown)
{
	owner.addComponent(this);
}

Component::~Component()
{
	owner.removeComponent(this);
}

void Component::setType(const ComponentType newType)
{
	this->type = newType;
}

void Component::processInput(const InputState& inputState)
{
}

void Component::update(float dt)
{
}
