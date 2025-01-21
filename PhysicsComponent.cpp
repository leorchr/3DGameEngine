#include "PhysicsComponent.h"

#include <iostream>

#include "Actor.h"
#include "Assets.h"
#include "BoxComponent.h"
#include "Collisions.h"
#include "CubeActor.h"
#include "Game.h"
#include "PlaneActor.h"

PhysicsComponent::PhysicsComponent(Actor* ownerP, float radius) : Component(ownerP), radius(radius){}

void PhysicsComponent::update(float dt)
{
	Component::update(dt);

	// Need to recompute world transform to update world box
	owner.computeWorldTransform();
	const Vector3& position = owner.getPosition();
	auto sphereCenterPosition = Vector3(position.x,position.y,position.z);

	auto& planes = owner.getGame().getPlanes();
	for (auto pa : planes)
	{
	
		// Convert the sphere center to OBB local space
		AABB objectAABB = pa->getBox()->getObjectAABB();
		OBB worldOBB = pa->getBox()->getWorldOBB();
		Vector3 localSphereCenter = worldOBB.rotation.toMatrix().getInverse() * (sphereCenterPosition - worldOBB.center);
	
		float x = Maths::max(objectAABB.min.x, Maths::min(localSphereCenter.x, objectAABB.max.x));
		float y = Maths::max(objectAABB.min.y, Maths::min(localSphereCenter.y, objectAABB.max.y));
		float z = objectAABB.min.z;
		Vector3 closestPointLocal(x, y, z);
		Vector3 closestPointWorld = worldOBB.rotation.toMatrix() * closestPointLocal + worldOBB.center;
	
		float distance = (closestPointWorld - sphereCenterPosition).length();

		Vector3 normal = closestPointWorld - sphereCenterPosition;
		normal.normalize();
		
		if(distance < radius && distance > 0)
		{
			owner.setPosition(owner.getPosition()-normal*(radius-distance));
		}
	}

	auto& cubes = owner.getGame().getCubes();
	for (auto ca : cubes)
	{
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

		Vector3 normal = closestPointWorld - sphereCenterPosition;
		normal.normalize();
		
		if(distance < radius && distance > 0)
		{
			owner.setPosition(owner.getPosition()-normal*(radius-distance));
		}
	}
}