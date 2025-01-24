#include "SpaceshipActor.h"
#include "Game.h"
#include "SpaceshipMovementInput.h"
#include "SpaceshipCameraComponent.h"
#include "SpaceshipCollisionsComponent.h"
#include "RocketActor.h"
#include "SpaceshipUI.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr),
	physicsComponent(nullptr),
	ui(nullptr),
	currentLife(baseLife)
{
	Game::instance().getInputSystem().setMouseRelativeMode(true);
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);
	physicsComponent = new SpaceshipCollisionsComponent(this, radius);
	setName("Spaceship");
	setPosition(Vector3(0.0f,0.0f,10.0f));
	Game::instance().setPlayer(this);

	ui = new SpaceshipUi(*this);
}

SpaceshipActor::~SpaceshipActor()
{
	ui->close();
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void SpaceshipActor::actorInput(const InputState& inputState)
{
	Actor::actorInput(inputState);
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
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
		RocketActor* ball = new RocketActor();
		ball->setPosition(start + dir * 20.0f);
		// Rotate the ball to face new direction
		ball->rotateToNewForward(dir);
	}
}

const float SpaceshipActor::getRadius() const
{
	return radius;
}

const int SpaceshipActor::getCurrentLife() const
{
	return currentLife;
}

void SpaceshipActor::onHit(float damages)
{
	currentLife -= damages;
	if(currentLife <= 0)
	{
		game.gameOver();
		return;
	}
	if(ui) ui->updateText();
}