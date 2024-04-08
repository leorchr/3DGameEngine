#include "PinMoveComponent.h"
#include "Actor.h"
#include "Vector3.h"
#include "Vector2.h"
#include "LineSegment.h"
#include "Collisions.h"
#include "PhysicsSystem.h"
#include "BallActor.h"
#include "Game.h"
#include "PinActor.h"

PinMoveComponent::PinMoveComponent(Actor* ownerP) : MoveComponent(ownerP), player(nullptr)
{
	dir = owner.getForward();
}

void PinMoveComponent::setPlayer(Actor* playerP)
{
	player = playerP;
}

void PinMoveComponent::update(float dt)
{
	if (owner.getPosition().y > 110 || owner.getPosition().y < -110) dir = Vector3::unitX;

	PhysicsSystem::CollisionInfo info;

	PinActor* pinActor = static_cast<PinActor*>(&owner);

	if (owner.getGame().getPhysicsSystem().boxCast(*pinActor->getBoxComponent(), info))
	{
		PinActor* pinColl = dynamic_cast<PinActor*>(info.actor);
		if (pinColl)
		{
			Vector3 hitDir = pinColl->getPosition() - owner.getPosition();
			hitDir.normalize();
			hitDir.z = 0;
			Vector3 midForce = (pinColl->getMoveComponent()->getVelocity() + getVelocity()) * 0.5f;
			pinColl->getMoveComponent()->addForce(hitDir * midForce.length()/**velocity.length()*/);
			setVelocity(midForce);
			pinColl->onHit();
		}
	}
	MoveComponent::update(dt);
}

void PinMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}