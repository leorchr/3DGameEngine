#include "SpaceshipActor.h"
#include "BoxComponent.h"
#include "SpaceshipMovementInput.h"
#include "SpaceshipCameraComponent.h"
#include "PhysicsComponent.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr),
	boxComponent(nullptr)
{
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);

	boxComponent = new BoxComponent(this);
	AABB collision(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 10.0f, 10.0f));
	boxComponent->setObjectBox(collision);
	boxComponent->setShouldRotate(true);

	physicsComponent = new PhysicsComponent(this, *boxComponent);
	setName("SpaceshipActor");
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}