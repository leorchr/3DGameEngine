#include "TPSCameraComponent.h"
#include "Actor.h"

TPSCameraComponent::TPSCameraComponent(Actor* ownerP) : CameraComponent(ownerP), pitch(.0f), pitchSpeed(.0f), yaw(.0f), yawSpeed(.0f)
{
}

void TPSCameraComponent::update(float dt)
{
	CameraComponent::update(dt);

	
	pitch += pitchSpeed * dt;
	pitch = Maths::clamp(pitch, -(Maths::pi/3), Maths::pi/3);
	yaw += yawSpeed * dt;

	Vector3 objPos = owner.getPosition();
	Vector3 posCam = objPos + offset;

	Vector3 dir = posCam - objPos;
	Quaternion q { Vector3::unitY, pitch };
	Quaternion q2 { Vector3::unitZ, yaw};
	Quaternion q3 = Quaternion::concatenate(q,q2);
	Vector3 dirRotated = Vector3::transform(dir, q3);

	//setup forward camera
	Vector3 invertDir = -1*dirRotated;
	setCameraForward(invertDir);
	//end forward
	
	Vector3 outputPosition = dirRotated + objPos;
	
	Vector3 up = Vector3::transform(Vector3::unitZ, q3);
	Matrix4 view = Matrix4::createLookAt(outputPosition + offsetView, owner.getPosition() + offsetView, up);
	setViewMatrix(view);
}

void TPSCameraComponent::setPitchSpeed(float speed)
{
	pitchSpeed = speed;
}

void TPSCameraComponent::setYawSpeed(float speed)
{
	yawSpeed = speed;
}

void TPSCameraComponent::setCameraForward(Vector3 forward)
{
	cameraForward = forward;
}