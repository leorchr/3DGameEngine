#pragma once
#include "Actor.h"
#include "CameraComponent.h"

class StationaryCameraComponent : public CameraComponent
{
public:
	StationaryCameraComponent(class Actor* ownerP);

	void recomputeRotation();
	void setPitch(float pitchP);
	void setYaw(float yawP);
	float getPitch() const { return pitch; }
	float getYaw() const { return yaw; }
private:
	float pitch;
	float yaw;
};