#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Game.h"
#include "BoxComponent.h"

PlaneActor::PlaneActor() : Actor()
{
	setScale(Vector3(2.0f, 2.0f, 2.0f));
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = &Assets::getMesh("Mesh_Plane");
	mc->setMesh(*mesh);
	getGame().addPlane(this);
}

PlaneActor::~PlaneActor()
{
	getGame().removePlane(this);
}
