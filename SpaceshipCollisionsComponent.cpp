#include "SpaceshipCollisionsComponent.h"

SpaceshipCollisionsComponent::SpaceshipCollisionsComponent(Actor* ownerP, float radius) :
PhysicsComponent(ownerP, radius){}

void SpaceshipCollisionsComponent::onHit(Vector3 normal, float distance)
{
	owner.setPosition(owner.getPosition()-normal*(radius-distance));
}

