#pragma once
#include "Actor.h"
#include "BallMoveComponent.h"
#include "BoxComponent.h"

class BallActor : public Actor
{
public:
	BallActor();

	void updateActor(float dt) override;
	void setPlayer(Actor* player);
	void hitTarget();
	BallMoveComponent* getBallMoveComponent() { return ballMove; }
	BoxComponent* getBoxComponent() { return boxComponent; }

private:
	BallMoveComponent* ballMove;
	BoxComponent* boxComponent;
	float lifetimeSpan;
};

