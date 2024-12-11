#pragma once
#include "Actor.h"
#include <memory>

class MeshActor : public Actor
{
public:
	MeshActor();

private:
	std::unique_ptr<class MeshComponent> meshComponent;
};
