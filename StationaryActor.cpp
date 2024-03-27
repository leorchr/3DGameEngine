#include "StationaryActor.h"
#include "StationaryCameraComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "BallActor.h"
#include "Collisions.h"

StationaryActor::StationaryActor() :
	Actor(),
	meshComponent(nullptr),
	cameraComponent(nullptr),
	boxComponent(nullptr)
{
	cameraComponent = new StationaryCameraComponent(this);
}

void StationaryActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void StationaryActor::actorInput(const InputState& inputState)
{
	// Shoot
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
	{
		shoot();
	}
}

void StationaryActor::shoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 1.0f, 0.0f);
	Vector3 start = getGame().getRenderer().unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = getGame().getRenderer().unproject(screenPoint);
	// Get direction vector
	Vector3 dir = end - start;
	dir.normalize();
	// Spawn a ball
	BallActor* ball = new BallActor();
	ball->setPlayer(this);
	ball->setPosition(start + dir * 20.0f);
	// Rotate the ball to face new direction
	ball->rotateToNewForward(dir);
}

void StationaryActor::setVisible(bool isVisible)
{
	//meshComponent->setVisible(isVisible);
}