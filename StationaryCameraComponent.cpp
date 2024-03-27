#include "StationaryCameraComponent.h"

StationaryCameraComponent::StationaryCameraComponent(Actor* ownerP) :
	CameraComponent(ownerP),
	pitch(0),
	yaw(0)
{
}

void StationaryCameraComponent::update(float dt)
{
	Vector3 cameraPosition = owner.getPosition();
	Quaternion q{ owner.getRight(), pitch };
	Vector3 viewForward = Vector3::transform(owner.getForward(), q);
	Vector3 up = Vector3::transform(Vector3::unitZ, q);
	Vector3 target = cameraPosition + viewForward * 100.0f;
	Matrix4 view = Matrix4::createLookAt(cameraPosition, target, up);
	setViewMatrix(view);
}

void StationaryCameraComponent::setPitch(float pitchP)
{
	pitch = pitchP;
}

void StationaryCameraComponent::setYaw(float yawP)
{
	yaw = yawP;
}