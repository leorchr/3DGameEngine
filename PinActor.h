#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "MoveComponent.h"

class PinActor : public Actor
{
public:
	PinActor();
	void updateActor(float dt) override;
	MoveComponent* getMoveComponent() { return movec; }
	bool getOnlyOnce() { return onlyOnce; }
	void setOnlyOnce(bool onceP);
	
private:
	MeshComponent* mc;
	MoveComponent* movec;
	BoxComponent* bc;
	bool onlyOnce;
};