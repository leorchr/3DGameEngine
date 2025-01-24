#pragma once
#include "Actor.h"
#include "CameraComponent.h"

class SpaceshipCameraComponent : public CameraComponent
{
public:
	SpaceshipCameraComponent(class Actor* ownerP);

	void update(float dt) override;

	float getPitch() const { return pitch; }
	float getPitchSpeed() const { return pitchSpeed; }
	float getMaxPitch() const { return maxPitch; }

	void setPitchSpeed(float speed);
	void setMaxPitch(float pitch);

	Vector3 getViewForward() const { return viewForward; }
private:
	float pitchSpeed;
	float maxPitch;
	float pitch;
	
	Vector3 viewForward;
};