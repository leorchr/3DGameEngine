#pragma once
#include "CameraComponent.h"

class TPSCameraComponent : public CameraComponent
{
public:
	TPSCameraComponent(class Actor* ownerP);

	void update(float dt) override;
	void setPitchSpeed(float speed);
	void setYawSpeed(float speed);
	void setCameraForward(Vector3 forward);
	Vector3 getCameraForward() { return cameraForward; }

private:
	float pitch;
	float pitchSpeed;
	float yaw;
	float yawSpeed;
	Vector3 cameraForward;
	const Vector3 offset = Vector3(-15.0f,0.0f,0.0f);
	const Vector3 offsetView = Vector3(0.0f,0.0f,0.0f);
};

