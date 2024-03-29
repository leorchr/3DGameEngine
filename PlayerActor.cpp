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

	arrow = new ArrowActor(1.0f, 15.0f);

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
		if(arrow->getCurrentState() != 4) arrow->setCurrentState(arrow->getCurrentState() + 1);
		if (arrow->getCurrentState() == 3) shoot();
	}
}

void PlayerActor::shoot()
{
	BallActor* ball = new BallActor();
	ball->setPlayer(this);
	ball->setPosition(Vector3(80.0f, 0.0f, -85.0f));
	Vector3 dir = arrow->getForward();
	dir.normalize();

	ball->getBallMoveComponent()->setDir(dir);
	ball->getBallMoveComponent()->setForwardSpeed(arrow->getScale().y*500/arrow->getMaxScaleY());
	arrow->setCurrentState(arrow->getCurrentState() + 1);
}

void PlayerActor::setCameraOrientation()
{
	cameraComponent->recomputeRotation();
}

void PlayerActor::setVisible(bool isVisible)
{
	arrow->setVisible(isVisible);
}