#include "SpaceshipActor.h"
#include "SpaceshipCameraComponent.h"
#include "InputSystem.h"
#include "SpaceshipMovementInput.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr)
{
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}