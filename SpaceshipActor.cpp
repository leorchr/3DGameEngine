#include "SpaceshipActor.h"
#include "DoorActor.h"
#include "Game.h"
#include "SpaceshipMovementInput.h"
#include "SpaceshipCameraComponent.h"
#include "PhysicsComponent.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr)
{
	Game::instance().getInputSystem().setMouseRelativeMode(true);
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);
	physicsComponent = new PhysicsComponent(this, 2);
	setName("Spaceship");
	setPosition(Vector3(0.0f,0.0f,10.0f));
	Game::instance().setPlayer(this);
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}