#include "RocketCollisionComponent.h"
#include "MeshComponent.h"
#include "RocketActor.h"

RocketCollisionComponent::RocketCollisionComponent(Actor* ownerP, float radius) :
PhysicsComponent(ownerP, radius),rocketOwner(nullptr)
{
	rocketOwner = dynamic_cast<RocketActor*>(&owner);
}

void RocketCollisionComponent::onHit(Vector3 normal, float distance)
{
	rocketOwner->setState(Actor::ActorState::Dead);
}

