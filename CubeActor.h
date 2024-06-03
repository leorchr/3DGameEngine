#pragma once
#include "Actor.h"
#include "BoxComponent.h"
class CubeActor : public Actor
{
public:
	CubeActor();
	~CubeActor();
	class BoxComponent* getBox() { return box; }
private:
	class BoxComponent* box;
};

