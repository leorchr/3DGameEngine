#include "EnemyRocketActor.h"
#include "Game.h"
#include "SpaceshipActor.h"

EnemyRocketActor::EnemyRocketActor()
{
	player = dynamic_cast<SpaceshipActor*>(getGame().getPlayer());
}

void EnemyRocketActor::updateActor(float dt)
{
	RocketActor::updateActor(dt);
	checkCollisions();
}

const float EnemyRocketActor::getRadius() const
{
	return radius;
}

void EnemyRocketActor::checkCollisions()
{
	if(player)
	{
		float distance = (player->getPosition()-position).length();
		if(distance <= player->getRadius() + radius)
		{
			player->onHit(50.0f);
			setState(ActorState::Dead);
		}
	}
}