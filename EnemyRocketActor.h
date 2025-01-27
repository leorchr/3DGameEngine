#pragma once
#include "RocketActor.h"

class EnemyRocketActor : public RocketActor
{
public:
	EnemyRocketActor();

	virtual void updateActor(float dt) override;
	const float getRadius() const;
	void checkCollisions() override;

private:
	class SpaceshipActor* player;
	const float radius = 1.0f;
};