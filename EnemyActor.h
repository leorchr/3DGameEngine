#pragma once
#include "MeshActor.h"

class EnemyActor : public MeshActor
{
public:
	EnemyActor();
	virtual ~EnemyActor() override;
	EnemyActor(const Actor&) = delete;
	EnemyActor& operator=(const EnemyActor&) = delete;

	void updateActor(float dt) override;

	void shoot();

private:
	const float baseShootingIntervals = 2.0f;
	float currentShootIntervals;
};
