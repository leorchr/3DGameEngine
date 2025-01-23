#pragma once
#include "Actor.h"

class SpaceshipActor : public Actor
{
public:
	SpaceshipActor();

	void updateActor(float dt) override;
	void actorInput(const InputState& inputState) override;
	const float getRadius() const;
	const int getCurrentLife() const;

	void onHit(float damages);

private:
	class SpaceshipMovementInput* moveInputComponent;
	class SpaceshipCameraComponent* cameraComponent;
	class SpaceshipCollisionsComponent* physicsComponent;
	class SpaceshipUi* ui;
	
	const float radius = 2;

	const int baseLife = 100;
	int currentLife;
};