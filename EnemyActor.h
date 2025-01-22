#pragma once
#include "Actor.h"
#include "MeshActor.h"
#include <string>

class EnemyActor : public MeshActor
{
public:
	EnemyActor(const std::string meshName);
	EnemyActor();
	virtual ~EnemyActor() override;
	EnemyActor(const Actor&) = delete;
	EnemyActor& operator=(const EnemyActor&) = delete;
};
