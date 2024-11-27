#pragma once
#include "Actor.h"

class ViewportActor : public Actor
{
public:
	ViewportActor();

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;

	class MoveComponent* getMoveComponent() const { return moveComponent; }
	float getBaseSpeed() const { return baseSpeed; }
	void setBaseSpeed(float speed);
private:
	class MoveComponent* moveComponent;
	class FPSCameraComponent* cameraComponent;
	Vector2 mousePos;

	float baseSpeed = 200.0f;
	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;
};