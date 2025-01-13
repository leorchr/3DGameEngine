#pragma once
#include "Component.h"
#include "Vector3.h"
class MoveComponent : public Component
{
public:
	MoveComponent(Actor* ownerP, int updateOrder = 10); // By default, update before other components
	MoveComponent() = delete;
	MoveComponent(const MoveComponent&) = delete;
	MoveComponent& operator=(const MoveComponent&) = delete;

	float getForwardSpeed() const { return forwardSpeed; }
	float getUpSpeed() const { return upSpeed; }
	float getYawSpeed() const { return yawSpeed; }
	float getPitchSpeed() const { return pitchSpeed; }
	float getRollSpeed() const { return rollSpeed; }
	float getStrafeSpeed() const { return strafeSpeed; }
	Vector3 getVelocity() const { return velocity; }

	void setForwardSpeed(float forwardSpeedP);
	void setUpSpeed(float upSpeedP);
	void setYawSpeed(float yawSpeedP);
	void setPitchSpeed(float pitchSpeedP);
	void setRollSpeed(float rollSpeedP);
	void setStrafeSpeed(float strafeSpeedP);
	void setVelocity(Vector3 velocityP);
	void addForce(Vector3 force);

	void update(float dt) override;



private:
	float forwardSpeed;
	float upSpeed;
	float strafeSpeed;
	float yawSpeed;
	float pitchSpeed;
	float rollSpeed;
	const float friction = 0.995f;
	Vector3 velocity;
};