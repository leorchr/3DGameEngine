#pragma once
#include "Actor.h"
#include "Vector3.h"
#include <unordered_map>

class FPSActor : public Actor
{
public:
	FPSActor();

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;
	
	void setVisible(bool isVisible);
	void fixCollisions();

private:
	class MoveComponent* moveComponent;
	class MeshComponent* meshComponent;
	class FPSCameraComponent* cameraComponent;
	class Actor* FPSModel;
	class BoxComponent* boxComponent;
};

const Vector3 MODEL_OFFSET = Vector3(10.0f, 10.0f, -10.0f);