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
	if (owner.getPosition().y > 110 || owner.getPosition().y < -110) setVelocity(Vector3::unitX * 50);
	PhysicsSystem::CollisionInfo info;

	PinActor* pinActor = static_cast<PinActor*>(&owner);

	if (owner.getGame().getPhysicsSystem().boxCast(*pinActor->getBoxComponent(), info))
	{
		PinActor* pinColl = dynamic_cast<PinActor*>(info.actor);
		if (pinColl)
		{
			if (pinActor->getMoveComponent()->getVelocity().length() > 0.0f) {
				Vector3 hitDir = pinColl->getPosition() - owner.getPosition();
				hitDir.z = 0;
				hitDir.normalize();
				Vector3 f = hitDir * getVelocity().length();
				pinActor->getMoveComponent()->setVelocity(pinActor->getMoveComponent()->getVelocity() / 2);
				pinColl->getMoveComponent()->addForce(f / 2.0f);
				pinColl->onHit();
			}
		}
	}
	MoveComponent::update(dt);
}

void PinMoveComponent::setDir(Vector3 dirP)
{
	dir = dirP;
}