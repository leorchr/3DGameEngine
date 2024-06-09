#pragma once
#include "Actor.h"

class BallActor final: public Actor
{
public:
	BallActor();

	void updateActor(const float dt) override;
	void setPlayer(Actor* player) const;
	void hitTarget();


private:
	class BallMoveComponent* ballMove;
	float timeLeft;
	static const float lifetimeSpan;
};