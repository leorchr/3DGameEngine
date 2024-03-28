#include "StationaryCameraComponent.h"

StationaryCameraComponent::StationaryCameraComponent(Actor* ownerP) :
	CameraComponent(ownerP),
	pitch(0.0f),
	yaw(0.0f)
{
}

void StationaryCameraComponent::recomputeRotation()
{
	Vector3 cameraPosition = owner.getPosition();

	Quaternion yawQ{ Vector3::unitZ, yaw };
	Vector3 forward = Vector3::transform(Vector3::unitX, yawQ);
	forward.normalize();
	Vector3 right = Vector3::cross(Vector3::unitZ, forward);
	right.normalize();
	Quaternion pitchQ{ right, pitch };

	Vector3 viewForward = Vector3::transform(forward, pitchQ);
	Vector3 up = Vector3::cross(viewForward, right);

	Vector3 target = cameraPosition + viewForward * 100.0f;
	Matrix4 view = Matrix4::createLookAt(cameraPosition, target, up);
	setViewMatrix(view);
}

void StationaryCameraComponent::setPitch(float pitchP)
{
	pitch = pitchP;
	recomputeRotation();
}

void StationaryCameraComponent::setYaw(float yawP)
{
	yaw = yawP;
	recomputeRotation();
}