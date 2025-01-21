#include "DoorActor.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "Game.h"
#include "MeshComponent.h"

DoorActor::DoorActor() : mc(nullptr), box(nullptr)
{
	mc = new MeshComponent(this);
	
	Mesh* mesh = &Assets::getMesh("Plane");
	mc->setMesh(*mesh);
	box = new BoxComponent(this);
	box->setObjectBox(mesh->getBox());
	getGame().addDoor(this);
}

void DoorActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void DoorActor::open()
{
	mc->setMesh(Assets::getMesh("Cube"));
}
void DoorActor::close()
{
	mc->setMesh(Assets::getMesh("Plane"));
}