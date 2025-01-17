#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"

BoxComponent::BoxComponent(Actor* ownerP, int updateOrderP):
	Component(ownerP, updateOrderP),
	objectAABB(Vector3::zero, Vector3::zero),
	worldAABB(Vector3::zero, Vector3::zero),
	worldOBB(Vector3::zero, Vector3::zero, Quaternion::identity),
	shouldRotate(true)
{
	Game::instance().getPhysicsSystem().addBox(this);
}

BoxComponent::~BoxComponent()
{
	Game::instance().getPhysicsSystem().removeBox(this);
}

void BoxComponent::setObjectBox(const AABB& objectBoxP)
{
	objectAABB = objectBoxP;
}

void BoxComponent::setShouldRotate(bool shouldRotateP)
{
	shouldRotate = shouldRotateP;
}

void BoxComponent::onUpdateWorldTransform()
{
	// Reset to object space box
	worldAABB = objectAABB;

	worldAABB.min *= owner.getScale();
	worldAABB.max *= owner.getScale();
	if (shouldRotate)
	{
		worldAABB.rotate(owner.getRotation());
	}
	worldAABB.min += owner.getPosition();
	worldAABB.max += owner.getPosition();


	const Vector3 currentPosition = owner.getPosition();
	worldOBB.center = Vector3(currentPosition.x + objectAABB.max.x/2,
								currentPosition.y + objectAABB.max.y/2,
								currentPosition.z + objectAABB.max.z/2);
	
	worldOBB.extents = (objectAABB.max - objectAABB.min) * 0.5f * owner.getScale();
	worldOBB.rotation = owner.getRotation();
}
