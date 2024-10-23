#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Game.h"
#include "BoxComponent.h"

PlaneActor::PlaneActor() : Actor()
{
	setScale(Vector3(2.0f,2.0f,2.0f));
	mc = new MeshComponent(this);
	BasicMesh* mesh = &Assets::getMesh("Mesh_Monkey");
	mc->setMesh(*mesh);
	box = new BoxComponent(this);

	getGame().addPlane(this);
}

PlaneActor::~PlaneActor()
{
	getGame().removePlane(this);
}