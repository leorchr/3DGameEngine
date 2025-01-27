#pragma once
#include "Actor.h"

class RocketActor : public Actor
{
public:
	RocketActor();

	virtual void updateActor(float dt) override;
	const float getRadius() const;
	void checkCollisions();
	
	class MeshComponent* getMeshComponent();
	
private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
	class RocketCollisionComponent* collisionComponent;

	const float forwardSpeed = 700.0f;
	const float startLifeSpan = 2.0f;
	float lifeTimeRemaining;
	
	const float radius = 1;

	std::vector <class EnemyActor*>& enemies;
};