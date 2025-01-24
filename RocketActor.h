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

	const float startLifeSpan = 2.0f;
	float lifeTimeRemaining;
	
	const float radius = 2;

	std::vector <class EnemyActor*>& enemies;
};