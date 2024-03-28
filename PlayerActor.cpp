#include "StationaryCameraComponent.h"
#include "Game.h"
#include "BallActor.h"
#include "PlayerActor.h"

PlayerActor::PlayerActor(float pitchP, float yawP) :
	Actor(),
	cameraComponent(nullptr)
{
	cameraComponent = new StationaryCameraComponent(this);
	cameraComponent->setPitch(pitchP);
	cameraComponent->setYaw(yawP);
}

void PlayerActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void PlayerActor::actorInput(const InputState& inputState)
{
	// Shoot
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
	{
		shoot();
	}
}

void PlayerActor::shoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
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
	ball->setPosition(Vector3(0, 0, -90) + dir * 20.0f);
	// Rotate the ball to face new direction
	ball->rotateToNewForward(dir);
}

void PlayerActor::setCameraOrientation()
{
	cameraComponent->recomputeRotation();
}