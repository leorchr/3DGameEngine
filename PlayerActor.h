#pragma once
#include "Actor.h"
#include "Vector3.h"

class PlayerActor : public Actor
{
public:
	PlayerActor(float pitchP = 0, float yawP = 0);

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;
	void shoot();
	void setCameraOrientation();

private:
	class StationaryCameraComponent* cameraComponent;
};