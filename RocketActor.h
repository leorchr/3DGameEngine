#pragma once
#include "Actor.h"

class RocketActor : public Actor
{
public:
	RocketActor();
	
private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
};