#include "TPActor.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "TPSCameraComponent.h"
#include "Game.h"

TPActor::TPActor() :
moveComponent(nullptr),
meshComponent(nullptr),
cameraComponent(nullptr)
{
	moveComponent = new MoveComponent(this);
	meshComponent = new MeshComponent(this);
	cameraComponent = new TPSCameraComponent(this);
	meshComponent->setMesh(Assets::getMesh("Mesh_Cube"));
}

TPActor::~TPActor()
{
	Actor::~Actor();
}

void TPActor::updateActor(float dt)
{
	Actor::updateActor(dt);
}

void TPActor::actorInput(const InputState& inputState)
{
	Actor::actorInput(inputState);

	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		forwardSpeed += 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		forwardSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		strafeSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += 400.0f;
	}
	moveComponent->setForwardSpeed(forwardSpeed);
	moveComponent->setStrafeSpeed(strafeSpeed);
	
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	float y = mousePosition.y;
	const int maxMouseSpeed = 500;
	const float maxAngularSpeed = Maths::pi * 8;
	
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		angularSpeed = x / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	moveComponent->setAngularSpeed(angularSpeed);
	cameraComponent->setYawSpeed(angularSpeed);
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	cameraComponent->setPitchSpeed(pitchSpeed);
}
