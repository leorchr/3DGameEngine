#include "PinActor.h"
#include "Assets.h"

PinActor::PinActor() : Actor()
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));
	bc = new BoxComponent(this);
	bc->setObjectBox(Assets::getMesh("Mesh_Cube").getBox());
}

void PinActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}