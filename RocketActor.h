#pragma once
#include "Actor.h"

class RocketActor : public Actor
{
public:
	RocketActor();

	virtual void updateActor(float dt) override;
	
	class MeshComponent* getMeshComponent();
	
private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
	class RocketCollisionComponent* collisionComponent;

	const float startLifeSpan = 2.0f;
	float lifeTimeRemaining;
};