#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"

BoxComponent::BoxComponent(Actor* ownerP, int updateOrderP):
	Component(ownerP, updateOrderP),
	baseObjectAABB(Vector3::zero, Vector3::zero),
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
	baseObjectAABB = objectBoxP;
}

void BoxComponent::setShouldRotate(bool shouldRotateP)
{
	shouldRotate = shouldRotateP;
}

void BoxComponent::onUpdateWorldTransform()
{
	objectAABB = baseObjectAABB;
	objectAABB.min *= owner.getScale();
	objectAABB.max *= owner.getScale();
	worldAABB = objectAABB;
	if (shouldRotate)
	{
		worldAABB.rotate(owner.getRotation());
	}
	worldAABB.min += owner.getPosition();
	worldAABB.max += owner.getPosition();


	const Vector3 currentPosition = owner.getPosition();
	worldOBB.center = Vector3(	currentPosition.x,
								currentPosition.y,
								currentPosition.z);
	
	worldOBB.extents = (baseObjectAABB.max - baseObjectAABB.min) * 0.5f * owner.getScale();
	worldOBB.rotation = owner.getRotation();
}
