#pragma once
#include "Component.h"
#include "Vector3.h"

enum class Movement
{
	idle,
	movingPositive,
	movingNegative
};

class SpaceshipMoveComponent : public Component
{
public:
	SpaceshipMoveComponent(Actor* ownerP, int updateOrder = 10); // By default, update before other components
	SpaceshipMoveComponent() = delete;
	SpaceshipMoveComponent(const SpaceshipMoveComponent&) = delete;
	SpaceshipMoveComponent& operator=(const SpaceshipMoveComponent&) = delete;

	float getForwardSpeed() const { return currentForwardSpeed; }
	float getUpSpeed() const { return currentUpSpeed; }
	float getStrafeSpeed() const { return currentStrafeSpeed; }
	float getYawSpeed() const { return yawSpeed; }
	float getPitchSpeed() const { return pitchSpeed; }
	float getRollSpeed() const { return rollSpeed; }
	Vector3 getVelocity() const { return velocity; }
	
	void setYawSpeed(float yawSpeedP);
	void setPitchSpeed(float pitchSpeedP);
	void setRollSpeed(float rollSpeedP);
	void setVelocity(Vector3 velocityP);
	void addForce(Vector3 force);

	void update(float dt) override;

private:

	// Movement Speed

	float currentForwardSpeed;
	const float maxForwardSpeed = 150.0f;
	float currentUpSpeed;
	const float maxUpSpeed = 150.0f;
	float currentStrafeSpeed;
	const float maxStrafeSpeed = 150.0f;
	const float acc = 5.f;
	const float decc = 4.f;


	// Rotation Speed
	
	float yawSpeed;
	float pitchSpeed;
	float rollSpeed;
	const float friction = 0.995f;
	Vector3 velocity;
	

protected:
	enum Movement currentForwardMovement = Movement::idle; 
	enum Movement currentUpMovement = Movement::idle; 
	enum Movement currentStrafeMovement = Movement::idle; 
	
};