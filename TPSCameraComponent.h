#pragma once
#include "CameraComponent.h"

class TPSCameraComponent : public CameraComponent
{
public:
	TPSCameraComponent(class Actor* ownerP);

	void update(float dt) override;
	void setPitchSpeed(float speed);
	void setYawSpeed(float speed);

private:
	float pitch;
	float pitchSpeed;
	float yaw;
	float yawSpeed;
	const Vector3 offset = Vector3(-50.0f,0.0f,0.0f);
	const Vector3 offsetView = Vector3(0.0f,0.0f,0.0f);
};

