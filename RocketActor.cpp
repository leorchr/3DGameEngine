#include "RocketActor.h"
#include "Assets.h"
#include "EnemyActor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "RocketCollisionComponent.h"

RocketActor::RocketActor() :
	Actor(),
	moveComponent(nullptr),
	meshComponent(nullptr),
	collisionComponent(nullptr),
	lifeTimeRemaining(startLifeSpan),
	enemies(game.getEnemies())
{
	moveComponent = new MoveComponent(this);
	meshComponent = new MeshComponent(this);
	meshComponent->setMesh(Assets::getMesh("Sphere"));

	moveComponent->setForwardSpeed(forwardSpeed);
	collisionComponent = new RocketCollisionComponent(this, 10);
}

void RocketActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	if(lifeTimeRemaining > 0.0f) lifeTimeRemaining -= dt;
	else setState(ActorState::Dead);
	checkCollisions();
}

const float RocketActor::getRadius() const
{
	return scale.x * radius;
}

void RocketActor::checkCollisions()
{
	for(auto enemy : enemies)
	{
		if (enemy)
		{
			float distance = (enemy->getPosition()-position).length();
			if(distance <= enemy->getRadius() + radius)
			{
				enemy->onHit(50.0f);
				setState(ActorState::Dead);
			}
		}
	}
}

MeshComponent* RocketActor::getMeshComponent()
{
	return meshComponent;
}