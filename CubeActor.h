#pragma once
#include "Actor.h"
#include "BoxComponent.h"
#include "MeshComponent.h"

class CubeActor : public Actor
{
public:
	CubeActor();
	~CubeActor();
	class BoxComponent* getBox() { return box; }

protected:
	class BoxComponent* box;
	class MeshComponent* mc;
};

