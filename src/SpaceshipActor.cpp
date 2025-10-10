#include "SpaceshipActor.h"

#include "AkComponent.h"
#include "Game.h"
#include "SpaceshipMovementInput.h"
#include "SpaceshipCameraComponent.h"
#include "SpaceshipCollisionsComponent.h"
#include "RocketActor.h"
#include "SpaceshipUI.h"
#include "../Wwise/GeneratedSoundBanks/Wwise_IDs.h"

SpaceshipActor::SpaceshipActor() :
	moveInputComponent(nullptr),
	cameraComponent(nullptr),
	physicsComponent(nullptr),
	akComponent(nullptr),
	ui(nullptr),
	currentLife(baseLife)
{
	Game::instance().getInputSystem().setMouseRelativeMode(true);
	moveInputComponent = new SpaceshipMovementInput(this);
	cameraComponent = new SpaceshipCameraComponent(this);
	physicsComponent = new SpaceshipCollisionsComponent(this, radius);
	akComponent = new AkComponent(this);
	setName("Spaceship");
	setPosition(Vector3(0.0f,0.0f,10.0f));
	Game::instance().setPlayer(this);

	akComponent->setAsListener();
	ui = new SpaceshipUi(*this);
}

SpaceshipActor::~SpaceshipActor()
{
	ui->close();
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	akComponent->updatePosition();
}

void SpaceshipActor::actorInput(const InputState& inputState)
{
	if (inputState.mouse.getButtonState(1) == ButtonState::Pressed)
	{
		shoot();
	}
	Actor::actorInput(inputState);
}

const float SpaceshipActor::getRadius() const
{
	return radius;
}

const int SpaceshipActor::getCurrentLife() const
{
	return currentLife;
}

void SpaceshipActor::setCurrentLife(int newLife)
{
	currentLife = newLife;
	if(currentLife > baseLife) currentLife = baseLife;
	if(ui) ui->updateText();
}

void SpaceshipActor::onHit(int damages)
{
	setCurrentLife(currentLife-damages);
	if(currentLife <= 0)
	{
		game.gameOver();
		return;
	}
}

void SpaceshipActor::onCollect(int collectibleHealAmount)
{
	setCurrentLife(getCurrentLife()+collectibleHealAmount);
}

void SpaceshipActor::shoot() const
{
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = getGame().getRenderer().unproject(screenPoint);
	screenPoint.z = 0.9f;
	Vector3 end = getGame().getRenderer().unproject(screenPoint);
	Vector3 dir = end - start;
	dir.normalize();
	

	Vector3 up = getUp(); // Ensure getUp() returns the correct 'up' vector for the player's current orientation
	Vector3 positionOffset = up * -4.0f;
	Vector3 adjustedStart = start + positionOffset;

	RocketActor* ball = new RocketActor();
	ball->rotateToNewForward(dir);
	ball->setPosition(adjustedStart);

	akComponent->PostEvent(AK::EVENTS::PLAY_SHOT);
}
