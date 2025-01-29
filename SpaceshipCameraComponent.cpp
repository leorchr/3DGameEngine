#include "SpaceshipCameraComponent.h"
#include "Maths.h"

SpaceshipCameraComponent::SpaceshipCameraComponent(Actor* ownerP):
	CameraComponent(ownerP),
	pitchSpeed(0.0f),
	maxPitch(Maths::pi / 3.0f),
	pitch(0.0f),
	timeElapsed(0.0f)
{
}

void SpaceshipCameraComponent::update(float dt)
{
	CameraComponent::update(dt);
	timeElapsed+=dt;
	

	float offset = Maths::sin(timeElapsed * 2) * 4	;
	
	Vector3 cameraPosition = owner.getPosition() + owner.getUp() * offset;
	
	Quaternion q { owner.getRight(), 0 };
	viewForward = Vector3::transform(owner.getForward(), q);
	Vector3 target = cameraPosition + viewForward * 100.0f;
	
	Matrix4 view = Matrix4::createLookAt(cameraPosition, target, owner.getUp());
	setViewMatrix(view);
}

void SpaceshipCameraComponent::setPitchSpeed(float speed)
{
	pitchSpeed = speed;
}

void SpaceshipCameraComponent::setMaxPitch(float pitch)
{
	maxPitch = pitch;
}
