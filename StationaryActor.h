#pragma once
#include "Actor.h"
#include "Vector3.h"

class StationaryActor : public Actor
{
public:
	StationaryActor();

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;
	void shoot();

	void setVisible(bool isVisible);

private:
	class MeshComponent* meshComponent;
	class StationaryCameraComponent* cameraComponent;
	class Actor* StationaryModel;
	class BoxComponent* boxComponent;
	const Vector3 MODEL_OFFSET = Vector3(10.0f, 10.0f, -10.0f);
};
