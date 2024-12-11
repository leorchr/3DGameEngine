#include "MeshActor.h"
#include "MeshComponent.h"
#include "Assets.h"

MeshActor::MeshActor() : Actor(), meshComponent(nullptr)
{
	MeshComponent* actorMesh = new MeshComponent(this);
	actorMesh->setMesh(Assets::getMesh("Cube"));
	setPosition(Vector3(0.0f,0.0f,15.0f));
	setScale(Vector3(5.0f,5.0f,5.0f));
}