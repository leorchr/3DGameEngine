#pragma once
#include "Actor.h"
#include "SphereActor.h"

class SunActor : public SphereActor
{
public:
	SunActor();
	void updateActor(float dt) override;
	
private:
	const float rotationSpeed = 10;
};