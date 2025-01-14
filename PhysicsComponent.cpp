#include "PhysicsComponent.h"
#include "Actor.h"
#include "BoxComponent.h"
#include "Collisions.h"
#include "CubeActor.h"
#include "Game.h"
#include "PlaneActor.h"

PhysicsComponent::PhysicsComponent(Actor* ownerP, BoxComponent& boxComponent) : Component(ownerP), boxComponent(boxComponent){}

void PhysicsComponent::update(float dt)
{
	Component::update(dt);

	// Need to recompute world transform to update world box
	owner.computeWorldTransform();

	const AABB& playerBox = boxComponent.getWorldBox();
	Vector3 pos = owner.getPosition();

	auto& planes = owner.getGame().getPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->getBox()->getWorldBox();
		if (Collisions::intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.max.x - playerBox.min.x;
			float dx2 = planeBox.min.x - playerBox.max.x;
			float dy1 = planeBox.max.y - playerBox.min.y;
			float dy2 = planeBox.min.y - playerBox.max.y;
			float dz1 = planeBox.max.z - playerBox.min.z;
			float dz2 = planeBox.min.z - playerBox.max.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Maths::abs(dx1) < Maths::abs(dx2) ? dx1 : dx2;
			// Ditto for dy
			float dy = Maths::abs(dy1) < Maths::abs(dy2) ? dy1 : dy2;
			// Ditto for dz
			float dz = Maths::abs(dz1) < Maths::abs(dz2) ? dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Maths::abs(dx) <= Maths::abs(dy) && Maths::abs(dx) <= Maths::abs(dz))
			{
				pos.x += dx;
			}
			else if (Maths::abs(dy) <= Maths::abs(dx) && Maths::abs(dy) <= Maths::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			owner.setPosition(pos);
			boxComponent.onUpdateWorldTransform();
		}
	}

	auto& cubes = owner.getGame().getCubes();
	for (auto ca : cubes)
	{
		// Do we collide with this PlaneActor?
		const AABB& cubeBox = ca->getBox()->getWorldBox();
		if (Collisions::intersect(playerBox, cubeBox))
		{
			// Calculate all our differences
			float dx1 = cubeBox.max.x - playerBox.min.x;
			float dx2 = cubeBox.min.x - playerBox.max.x;
			float dy1 = cubeBox.max.y - playerBox.min.y;
			float dy2 = cubeBox.min.y - playerBox.max.y;
			float dz1 = cubeBox.max.z - playerBox.min.z;
			float dz2 = cubeBox.min.z - playerBox.max.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Maths::abs(dx1) < Maths::abs(dx2) ? dx1 : dx2;
			// Ditto for dy
			float dy = Maths::abs(dy1) < Maths::abs(dy2) ? dy1 : dy2;
			// Ditto for dz
			float dz = Maths::abs(dz1) < Maths::abs(dz2) ? dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Maths::abs(dx) <= Maths::abs(dy) && Maths::abs(dx) <= Maths::abs(dz))
			{
				pos.x += dx;
			}
			else if (Maths::abs(dy) <= Maths::abs(dx) && Maths::abs(dy) <= Maths::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			owner.setPosition(pos);
			boxComponent.onUpdateWorldTransform();
		}
	}
}