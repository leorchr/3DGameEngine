#include "SunActor.h"
#include "Assets.h"
#include "MeshComponent.h"

SunActor::SunActor() : SphereActor()
{
	mc->setMesh(Assets::getMesh("Sphere"));
}

void SunActor::updateActor(float dt)
{
	SphereActor::updateActor(dt);
}
