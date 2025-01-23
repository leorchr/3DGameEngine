#pragma once
#include "RocketActor.h"

class EnemyRocketActor : public RocketActor
{
public:
	EnemyRocketActor();

	virtual void updateActor(float dt) override;
	const float getRadius() const;

private:
	class SpaceshipActor* player;
	const float radius = 2;
};