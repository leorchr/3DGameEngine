#pragma once
#include "EnumComponentType.h"
#include <document.h>

class Actor;

class Component
{
public:
	Component(Actor* ownerP, int updateOrderP = 100);
	Component() = delete;
	virtual ~Component();
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	int getUpdateOrder() const { return updateOrder; }
	class Actor& getOwner() { return owner; }

	ComponentType getType() const { return type;}
	virtual void processInput(const struct InputState& inputState);
	virtual void update(float dt);
	virtual void onUpdateWorldTransform() {}

	virtual void load(const rapidjson::Value& data);
	virtual void save(rapidjson::Value& actorAttributes, rapidjson::MemoryPoolAllocator<>& allocator);


#ifdef _DEBUG
	virtual void updateImGUIOutliner();
#endif

protected:
	void setType(const ComponentType newType);
	
	Actor& owner;
	int updateOrder;		// Order of the component in the actor's updateComponent method
	ComponentType type;
};

