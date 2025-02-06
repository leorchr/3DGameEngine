#pragma once
#include "Actor.h"
#include "Vector3.h"

class StationaryActor : public Actor
{
public:
	StationaryActor(float pitchP = 0, float yawP = 0);
	void updateActor(float dt) override;
	void setCameraOrientation();

private:
	class StationaryCameraComponent* cameraComponent;
};
