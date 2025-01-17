#include "SphereActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "CubeActor.h"
#include <iostream>

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
}

void SphereActor::fixCollisions()
{
	float radius = mc->getMesh()->getBox().max.x/2;
	auto sphereCenterPosition = Vector3(position.x + radius,position.y + radius,position.z + radius);
	//
	//
	//
	// auto box = ca->getBox()->getWorldAABB();
	// const float x = Maths::max(box.min.x, Maths::min(spherePosition.x, box.max.x));
	// const float y = Maths::max(box.min.y, Maths::min(spherePosition.y, box.max.y));
	// const float z = Maths::max(box.min.z, Maths::min(spherePosition.z, box.max.z));
	//
	// const float distance = Maths::sqrt(
	//   (x - spherePosition.x) * (x - spherePosition.x) +
	//   (y - spherePosition.y) * (y - spherePosition.y) +
	//   (z - spherePosition.z) * (z - spherePosition.z));

	// Convert the sphere center to OBB local space
	AABB objectAABB = ca->getBox()->getObjectAABB();
	OBB worldOBB = ca->getBox()->getWorldOBB();
	Vector3 localSphereCenter = worldOBB.rotation.toMatrix().getInverse() * (sphereCenterPosition - worldOBB.center);

	float x = Maths::max(objectAABB.min.x, Maths::min(localSphereCenter.x, objectAABB.max.x));
	float y = Maths::max(objectAABB.min.y, Maths::min(localSphereCenter.y, objectAABB.max.y));
	float z = Maths::max(objectAABB.min.z, Maths::min(localSphereCenter.z, objectAABB.max.z));
	Vector3 closestPointLocal(x, y, z);
	Vector3 closestPointWorld = worldOBB.rotation.toMatrix() * closestPointLocal + worldOBB.center;
	
	float distance = (closestPointWorld - sphereCenterPosition).length();
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