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
	float getAngularSpeed() const { return angularSpeed; }
	float getStrafeSpeed() const { return strafeSpeed; }
	float getGravitySpeed() const { return gravitySpeed; }
	Vector3 getVelocity() const { return velocity; }

	void setForwardSpeed(float forwardSpeedP);
	void setAngularSpeed(float angularSpeedP);
	void setStrafeSpeed(float strafeSpeedP);
	void setGravitySpeed(float gravitySpeedP);
	void setVelocity(Vector3 velocityP);
	void addForce(Vector3 force);
	void setEnableGravity(bool enableGravityP) { enableGravity = enableGravityP; }

	void update(float dt) override;



private:
	float forwardSpeed;
	float angularSpeed;
	float strafeSpeed;
	float gravitySpeed;
	const float friction = 0.995f;
	Vector3 velocity;
	bool enableGravity;
};