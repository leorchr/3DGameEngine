#pragma once
#include "Actor.h"
#include "MeshActor.h"

class CollectibleActor : public MeshActor
{
public:
	CollectibleActor();
	virtual ~CollectibleActor() override;

	void updateActor(float dt) override;
	const float getRadius() const;

	void onHit();

private:
	const float radius = 1;
	const int healAmount = 20;
};