#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "BoxComponent.h"

class PinActor : public Actor
{
public:
	PinActor();
	void updateActor(float dt) override;
	
private:
	MeshComponent* mc;
	BoxComponent* bc;

};