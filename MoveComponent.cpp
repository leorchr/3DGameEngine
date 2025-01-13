#include "MoveComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Window.h"

MoveComponent::MoveComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP), forwardSpeed(0.0f), yawSpeed(0.0f), strafeSpeed(0.0f)
{
	velocity = Vector3(0, 0, 0);
}

void MoveComponent::setForwardSpeed(float forwardSpeedP)
{
	forwardSpeed = forwardSpeedP;
}

void MoveComponent::setUpSpeed(float upSpeedP)
{
	upSpeed = upSpeedP;
}

void MoveComponent::setYawSpeed(float yawSpeedP)
{
	yawSpeed = yawSpeedP;
}

void MoveComponent::setPitchSpeed(float pitchSpeedP)
{
	pitchSpeed = pitchSpeedP;
}
void MoveComponent::setRollSpeed(float rollSpeedP)
{
	rollSpeed = rollSpeedP;
}

void MoveComponent::setStrafeSpeed(float strafeSpeedP)
{
	strafeSpeed = strafeSpeedP;
}

void MoveComponent::setVelocity(Vector3 velocityP)
{
	velocity = velocityP;
}

void MoveComponent::addForce(Vector3 force)
{
	velocity += force;
}

void MoveComponent::update(float dt)
{
	velocity *= friction;

	if (!Maths::nearZero(yawSpeed) || !Maths::nearZero(pitchSpeed) || !Maths::nearZero(rollSpeed))
	{
		Quaternion newRotation = owner.getRotation();
		float yawAngle = yawSpeed * dt;
		Quaternion increment(owner.getUp(), yawAngle);
		newRotation = Quaternion::concatenate(newRotation, increment);
		//owner.setRotation(newRotation);
		
		float pitchAngle = pitchSpeed * dt;
		Vector3 right = Vector3::transform(Vector3::unitY, newRotation);
		Quaternion increment2(right, pitchAngle);
		newRotation = Quaternion::concatenate(newRotation, increment2);
		//owner.setRotation(newRotation);
		
		float angle = rollSpeed * dt;
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
		Quaternion increment3(forward, angle);
		newRotation = Quaternion::concatenate(newRotation, increment3);
		owner.setRotation(newRotation);
	}

	
	if (!Maths::nearZero(forwardSpeed) || !Maths::nearZero(upSpeed) || !Maths::nearZero(strafeSpeed))
	{
		Vector3 newPosition = owner.getPosition();
		newPosition += owner.getForward() * forwardSpeed * dt;
		newPosition += owner.getUp() * upSpeed * dt;
		newPosition += owner.getRight() * strafeSpeed * dt;
		owner.setPosition(newPosition);
	}
}