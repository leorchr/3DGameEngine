#pragma once
#include "Actor.h"
#include "BallMoveComponent.h"
class BallActor : public Actor
{
public:
	BallActor();

	void updateActor(float dt) override;
	void setPlayer(Actor* player);
	void hitTarget();
	BallMoveComponent* getBallMoveComponent() { return ballMove; }

private:
	BallMoveComponent* ballMove;
	float lifetimeSpan;
};

