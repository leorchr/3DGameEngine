#include "SpaceshipMoveComponent.h"
#include "Maths.h"
#include "Actor.h"

SpaceshipMoveComponent::SpaceshipMoveComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP), forwardSpeed(0.0f), upSpeed(0.0f), strafeSpeed(0.0f), yawSpeed(0.0f), pitchSpeed(0.0f), rollSpeed(0.0f)
{
	velocity = Vector3(0, 0, 0);
}

void SpaceshipMoveComponent::setForwardSpeed(float forwardSpeedP)
{
	forwardSpeed = forwardSpeedP;
}

void SpaceshipMoveComponent::setUpSpeed(float upSpeedP)
{
	upSpeed = upSpeedP;
}

void SpaceshipMoveComponent::setYawSpeed(float yawSpeedP)
{
	yawSpeed = yawSpeedP;
}

void SpaceshipMoveComponent::setPitchSpeed(float pitchSpeedP)
{
	pitchSpeed = pitchSpeedP;
}
void SpaceshipMoveComponent::setRollSpeed(float rollSpeedP)
{
	rollSpeed = rollSpeedP;
}

void SpaceshipMoveComponent::setStrafeSpeed(float strafeSpeedP)
{
	strafeSpeed = strafeSpeedP;
}

void SpaceshipMoveComponent::setVelocity(Vector3 velocityP)
{
	velocity = velocityP;
}

void SpaceshipMoveComponent::addForce(Vector3 force)
{
	velocity += force;
}

void SpaceshipMoveComponent::update(float dt)
{
	velocity *= friction;

	if (!Maths::nearZero(yawSpeed) || !Maths::nearZero(pitchSpeed) || !Maths::nearZero(rollSpeed))
	{		
		Quaternion newRotation = owner.getRotation();
		
		float yawAngle = yawSpeed * dt;
		Vector3 up = Vector3::transform(Vector3::unitZ, newRotation);
		Quaternion increment(up, yawAngle);
		newRotation = Quaternion::concatenate(newRotation, increment);
		owner.setRotation(newRotation);
		
		float pitchAngle = pitchSpeed * dt;
		Vector3 right = Vector3::transform(Vector3::unitX, newRotation);
		Quaternion increment2(right, pitchAngle);
		newRotation = Quaternion::concatenate(newRotation, increment2);
		owner.setRotation(newRotation);
		
		float angle = rollSpeed * dt;
		Vector3 forward = Vector3::transform(Vector3::unitY, newRotation);
		Quaternion increment3(forward, angle);
		newRotation = Quaternion::concatenate(newRotation, increment3);
		owner.setRotation(newRotation);
	}

	
	if (!Maths::nearZero(forwardSpeed) || !Maths::nearZero(upSpeed) || !Maths::nearZero(strafeSpeed))
	{
		forwardSpeed *= acc;
		upSpeed *= acc;
		if(forwardSpeed >= maxSpeed) forwardSpeed = maxSpeed;
		if(upSpeed >= maxSpeed) upSpeed = maxSpeed;
		
		Vector3 newPosition = owner.getPosition();
		newPosition += owner.getForward() * forwardSpeed * dt;
		newPosition += owner.getUp() * upSpeed * dt;
		newPosition += owner.getRight() * strafeSpeed * dt;
		owner.setPosition(newPosition);
	}
}