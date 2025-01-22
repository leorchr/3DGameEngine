#include "RocketActor.h"
#include "Assets.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "RocketCollisionComponent.h"

RocketActor::RocketActor() :
	Actor(),
	moveComponent(nullptr),
	meshComponent(nullptr),
	collisionComponent(nullptr),
	lifeTimeRemaining(startLifeSpan)
{
	moveComponent = new MoveComponent(this);
	meshComponent = new MeshComponent(this);
	meshComponent->setMesh(Assets::getMesh("Sphere"));
	
	moveComponent->setForwardSpeed(400.0f);
	collisionComponent = new RocketCollisionComponent(this, 10);
}

void RocketActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	if(lifeTimeRemaining > 0.0f) lifeTimeRemaining -= dt;
	else setState(ActorState::Dead);
}

MeshComponent* RocketActor::getMeshComponent()
{
	return meshComponent;
}