#include "SphereActor.h"
#include "MeshComponent.h"
#include "Assets.h"

SphereActor::SphereActor() : Actor()
{
	mc = new MeshComponent(this);
	Mesh* mesh = &Assets::getMesh("Sphere");
	mc->setMesh(*mesh);
	setName("Sphere");
	setScale(Vector3(1.0f,1.0f,1.0f));
	mc->getMesh()->setTexture(0, &Assets::getTexture("ButtonYellow"));
}

void SphereActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}