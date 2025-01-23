#include "EnemyRocketActor.h"
#include "Game.h"
#include "SpaceshipActor.h"
#include <iostream>
#include <ostream>

EnemyRocketActor::EnemyRocketActor()
{
	player = dynamic_cast<SpaceshipActor*>(getGame().getPlayer());
}

void EnemyRocketActor::updateActor(float dt)
{
	RocketActor::updateActor(dt);
	if(player)
	{
		float distance = (player->getPosition()-position).length();
		if(distance <= player->getRadius() + radius)
		{
			player->onHit(4.0f);
			setState(ActorState::Dead);
		}
	}
}

const float EnemyRocketActor::getRadius() const
{
	return radius;
}