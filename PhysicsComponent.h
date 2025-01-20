#pragma once
#include "Component.h"
#include "CubeActor.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Actor* ownerP, class BoxComponent& boxComponent, float radius);
	PhysicsComponent() = delete;
	PhysicsComponent(const PhysicsComponent&) = delete;
	PhysicsComponent& operator=(const PhysicsComponent&) = delete;
	
	void update(float dt) override;

private:
	class BoxComponent& boxComponent;

	float radius;
};
