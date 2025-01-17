#include "SphereActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "CubeActor.h"
#include <iostream>

#include "PlaneActor.h"

SphereActor::SphereActor() : Actor()
{
	mc = new MeshComponent(this);
	Mesh* mesh = &Assets::getMesh("Sphere");
	mc->setMesh(*mesh);
	setName("Sphere");

	setScale(Vector3(1.0f,1.0f,1.0f));
	
	ca = new CubeActor();
	ca->setScale(Vector3(1.0f,1.0f,1.0f));
	ca->setPosition(Vector3(10.0f,10.0f,10.0f));

	auto plane = new PlaneActor();
	plane->setScale(Vector3(.2f,.2f,.2f));
	plane->setPosition(Vector3(10.0f,10.0f,10.0f));

}

void SphereActor::fixCollisions()
{
	float radius = (mc->getMesh()->getBox().max.x - mc->getMesh()->getBox().min.x)/2 * scale.x;

	// Convert the sphere center to OBB local spaces
	AABB objectAABB = ca->getBox()->getObjectAABB();
	OBB worldOBB = ca->getBox()->getWorldOBB();
	Vector3 localSphereCenter = worldOBB.rotation.toMatrix().getInverse() * (position - worldOBB.center);

	float x = Maths::max(objectAABB.min.x, Maths::min(localSphereCenter.x, objectAABB.max.x));
	float y = Maths::max(objectAABB.min.y, Maths::min(localSphereCenter.y, objectAABB.max.y));
	float z = Maths::max(objectAABB.min.z, Maths::min(localSphereCenter.z, objectAABB.max.z));
	Vector3 closestPointLocal(x, y, z);
	Vector3 closestPointWorld = worldOBB.rotation.toMatrix() * closestPointLocal + worldOBB.center;
	
	float distance = (closestPointWorld - position).length();
	if(distance < radius)
	{
		mc->setTexture(0, &Assets::getTexture("ButtonYellow"));
	}
	else
	{
		mc->setTexture(0, &Assets::getTexture("ButtonBlue"));
	}
	std::cout << distance << std::endl;
}

void SphereActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	fixCollisions();
}