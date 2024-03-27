#pragma once
#include "Actor.h"
class BallActor : public Actor
{
public:
	BallActor();

	void updateActor(float dt) override;
	void setPlayer(Actor* player);
	void hitTarget();


private:
	class BallMoveComponent* ballMove;
	float lifetimeSpan;
};

