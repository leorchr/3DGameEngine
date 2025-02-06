#pragma once
#include "Component.h"
#include "CubeActor.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Actor* ownerP, float radius);
	PhysicsComponent() = delete;
	PhysicsComponent(const PhysicsComponent&) = delete;
	PhysicsComponent& operator=(const PhysicsComponent&) = delete;
	
	void update(float dt) override;
	virtual void onHit(Vector3 normal, float distance) = 0;
	
protected:
	float radius;
};
