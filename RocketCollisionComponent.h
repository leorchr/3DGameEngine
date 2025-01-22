#pragma once
#include "PhysicsComponent.h"

class RocketCollisionComponent : public PhysicsComponent
{
public:
	RocketCollisionComponent(Actor* ownerP, float radius);
	RocketCollisionComponent() = delete;
	RocketCollisionComponent(const PhysicsComponent&) = delete;
	RocketCollisionComponent& operator=(const RocketCollisionComponent&) = delete;

	virtual void onHit(Vector3 normal, float distance) override;

private:
	class RocketActor* rocketOwner;
	
};
