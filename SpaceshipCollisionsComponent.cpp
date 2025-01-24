#include "SpaceshipCollisionsComponent.h"
#include "CollectibleActor.h"
#include "Game.h"

SpaceshipCollisionsComponent::SpaceshipCollisionsComponent(Actor* ownerP, float radius) :
PhysicsComponent(ownerP, radius), collectibles(Game::instance().getCollectible()){}

void SpaceshipCollisionsComponent::onHit(Vector3 normal, float distance)
{
	owner.setPosition(owner.getPosition()-normal*(radius-distance));
}

void SpaceshipCollisionsComponent::update(float dt)
{
	PhysicsComponent::update(dt);
	checkCollectiblesCollisions();
}

void SpaceshipCollisionsComponent::checkCollectiblesCollisions()
{
	for(auto collectible : collectibles)
	{
		if (collectible)
		{
			float distance = (owner.getPosition()-collectible->getPosition()).length();
			if(distance <= collectible->getRadius() + radius)
			{
				collectible->onHit();
			}
		}
	}
}

