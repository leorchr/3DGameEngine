#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "Assets.h"


class ArrowActor : public Actor
{
public:
	ArrowActor(float speedP);

	void updateActor(float dt) override;
	void setVisible(bool isVisible);

private:
	MeshComponent* mc;
	float speed;
};

