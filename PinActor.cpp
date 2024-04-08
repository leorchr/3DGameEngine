#include "PinActor.h"
#include "Assets.h"
#include "Game.h"

PinActor::PinActor() : Actor(), lifetimeSpan(1.0f)
{
	mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));
	bc = new BoxComponent(this);
	bc->setObjectBox(Assets::getMesh("Mesh_Cube").getBox());
	pc = new PinMoveComponent(this);
}

void PinActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void PinActor::onHit()
{
	//getGame().setScore(getGame().getScore() + 1);
}