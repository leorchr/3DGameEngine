#pragma once
#include "Actor.h"
#include "MeshActor.h"
#include <string>

class EnemyActor : public MeshActor
{
public:
	EnemyActor();
	virtual ~EnemyActor() override;
	EnemyActor(const Actor&) = delete;
	EnemyActor& operator=(const EnemyActor&) = delete;

	void updateActor(float dt) override;

private:
	const float baseShootingIntervals = 2.0f;
	float currentShootIntervals;
};
