#pragma once
#include "MoveComponent.h"
#include "Vector3.h"

class PinMoveComponent : public MoveComponent
{
public:
	PinMoveComponent(class Actor* ownerP);

	void setPlayer(class Actor* playerP);

	void update(float dt) override;
	void setDir(Vector3 dirP);
	Vector3* getDir() { return &dir; }

protected:
	class Actor* player;
	Vector3 dir;
}; 