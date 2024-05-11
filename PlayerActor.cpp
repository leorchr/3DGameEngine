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

	arrow = new ArrowActor(1.0f, 50.0f);
	currentState = 1;
	shotNumber = 0;
}

void PlayerActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void PlayerActor::actorInput(const InputState& inputState)
{
	// Shoot bowling ball
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
	{
		if (currentState != 4) {
			currentState++;
			switch (currentState) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				shoot();
				shotNumber++;
				break;
			case 4:
				break;
			case 5:
				break;
			}
		}
	}
}

void PlayerActor::shoot()
{
	BallActor* ball = new BallActor();
	ball->setPlayer(this);
	ball->setPosition(Vector3(80.0f, 0.0f, -85.0f));
	Vector3 dir = arrow->getForward();
	dir.normalize();
	ball->getBallMoveComponent()->setVelocity(dir * (arrow->getScale().y * 2000 / arrow->getMaxScaleY()));
}

void PlayerActor::setCameraOrientation()
{
	cameraComponent->recomputeRotation();
}

void PlayerActor::setVisible(bool isVisible)
{
	arrow->setVisible(isVisible);
}