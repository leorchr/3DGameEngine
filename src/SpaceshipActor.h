#pragma once
#include "Actor.h"

class SpaceshipActor : public Actor
{
public:
	SpaceshipActor();
	virtual ~SpaceshipActor() override;

	void updateActor(float dt) override;
	void actorInput(const InputState& inputState) override;
	const float getRadius() const;
	const int getCurrentLife() const;
	void setCurrentLife(int newLife);
	void shoot() const;

	
	void onHit(int damages);
	void onCollect(int collectibleHealAmount);

private:
	class SpaceshipMovementInput* moveInputComponent;
	class SpaceshipCameraComponent* cameraComponent;
	class SpaceshipCollisionsComponent* physicsComponent;
	class AkComponent* akComponent;
	class SpaceshipUi* ui;
	
	const float radius = 10;

	const int baseLife = 100;
	int currentLife;
};