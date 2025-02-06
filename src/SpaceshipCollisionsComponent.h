#pragma once
#include "PhysicsComponent.h"

class SpaceshipCollisionsComponent : public PhysicsComponent
{
public:
	SpaceshipCollisionsComponent(Actor* ownerP, float radius);
	SpaceshipCollisionsComponent() = delete;
	SpaceshipCollisionsComponent(const PhysicsComponent&) = delete;
	SpaceshipCollisionsComponent& operator=(const SpaceshipCollisionsComponent&) = delete;

	virtual void onHit(Vector3 normal, float distance) override;

	virtual void update(float dt) override;
	virtual void checkCollectiblesCollisions();

private:
	std::vector <class CollectibleActor*>& collectibles;
	
};
