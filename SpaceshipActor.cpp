#include "SpaceshipActor.h"
#include "SpaceshipMovementInput.h"
#include "SpaceshipCameraComponent.h"
#include "PhysicsComponent.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr)
{
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);
	physicsComponent = new PhysicsComponent(this, 2);
	setName("SpaceshipActor");
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}