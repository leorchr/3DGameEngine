#pragma once
#include "Actor.h"
class SphereActor : public Actor
{
public:
	SphereActor();

	void fixCollisions();
	void updateActor(float dt) override;
protected:
	class MeshComponent* mc;
	class CubeActor* ca;
};

