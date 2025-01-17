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

void Component::load(const rapidjson::Value& data) {}
void Component::save(rapidjson::Value& actorAttributes, rapidjson::Document::AllocatorType& allocator){}

#ifdef _DEBUG
void Component::updateImGUIOutliner() {}
#endif
