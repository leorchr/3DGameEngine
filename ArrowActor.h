#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "Assets.h"


class ArrowActor : public Actor
{
public:
	ArrowActor(float rotationSpeedP, float scaleSpeedP);

	void updateActor(float dt) override;
	void setVisible(bool isVisible);
	float getMaxScaleY() { return maxScaleY; }
	float getMinScaleY() { return minScaleY; }

private:
	MeshComponent* mc;
	float rotationSpeed;
	float scaleSpeed;
	Vector3 baseScale;

	Quaternion newRotation;
	Quaternion increment;
	float angle;
	float yaw;
	float scaleY;
	float maxScaleY;
	float minScaleY;
};

