#pragma once
#include "Actor.h"
#include "Vector3.h"

class StationaryActor : public Actor
{
public:
	StationaryActor();

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;
	void shoot();

	void setVisible(bool isVisible);

private:
	class StationaryCameraComponent* cameraComponent;
};
