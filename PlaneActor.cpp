#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Game.h"
#include "BoxComponent.h"

PlaneActor::PlaneActor() : Actor()
{
	setScale(Vector3(1.0f,1.0f,1.0f));
	mc = new MeshComponent(this);
	Mesh* mesh = &Assets::getMesh("Mesh_Plane");
	mc->setMesh(*mesh);
	box = new BoxComponent(this);

	getGame().addPlane(this);
}

PlaneActor::~PlaneActor()
{
	getGame().removePlane(this);
}