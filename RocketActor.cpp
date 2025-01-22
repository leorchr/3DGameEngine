#include "RocketActor.h"
#include "Assets.h"
#include "MeshComponent.h"
#include "MoveComponent.h"

RocketActor::RocketActor() :
	Actor(),
	moveComponent(nullptr),
	meshComponent(nullptr)
{
	moveComponent = new MoveComponent(this);
	meshComponent = new MeshComponent(this);
	meshComponent->setMesh(Assets::getMesh("Sphere"));

	auto mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Plane"));

	moveComponent->setForwardSpeed(50.0f);
}