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
	bool getOnlyOnce() { return onlyOnce; }
	BoxComponent* getBoxComponent() { return bc; }
	void onHit();
	
private:
	MeshComponent* mc;
	PinMoveComponent* pc;
	BoxComponent* bc;
	bool onlyOnce;
	float lifetimeSpan;
};