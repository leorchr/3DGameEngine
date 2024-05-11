#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "PinMoveComponent.h"

class PinActor : public Actor
{
public:
	PinActor();
	void updateActor(float dt) override;
	PinMoveComponent* getMoveComponent() { return pc; }
	BoxComponent* getBoxComponent() { return bc; }
	void onHit();

private:
	MeshComponent* mc;
	PinMoveComponent* pc;
	BoxComponent* bc;
	float lifetimeSpan;
	bool alreadyDone;
};