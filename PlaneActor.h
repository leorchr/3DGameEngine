#pragma once
#include "Actor.h"
class PlaneActor : public Actor
{
public:
	PlaneActor();
	~PlaneActor();
	class BoxComponent* getBox() { return box; }
	class MeshComponent* getMesh() { return mc; }


private:
	class BoxComponent* box;
	class MeshComponent* mc;
};