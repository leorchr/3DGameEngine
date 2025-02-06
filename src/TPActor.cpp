#include "TPActor.h"
#include "Assets.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "TPSCameraComponent.h"

TPActor::TPActor() :
moveComponent(nullptr),
meshComponent(nullptr),
cameraComponent(nullptr),
lerpValue(0),
cameraForwardRaw(Vector3::zero),
baseRotation(Quaternion::identity),
cameraLock(false)
{
	Game::instance().getInputSystem().setMouseRelativeMode(true);
	SDL_WarpMouseInWindow(Game::instance().getWindow().getSDLWindow(),WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	moveComponent = new MoveComponent(this);
	meshComponent = new MeshComponent(this);
	cameraComponent = new TPSCameraComponent(this);
	meshComponent->setMesh(Assets::getMesh("Mesh_Moto"));
	baseRotation = getRotation();
	moveComponent->setForwardSpeed(baseSpeed);
}

TPActor::~TPActor()
{
	Actor::~Actor();
}

void TPActor::updateActor(float dt)
{
	Actor::updateActor(dt);

	if(!cameraLock)
	{
		Vector3 newCameraForward = cameraComponent->getCameraForward();
		if(newCameraForward != cameraForwardRaw)
		{
			cameraForwardRaw = cameraComponent->getCameraForward();
			lerpValue = 0.0f;
			baseRotation = getRotation();
		}
		
		Vector3 cameraForward = cameraForwardRaw;
		cameraForward.z = 0.0f;
		cameraForward.normalize();
	
		if(lerpValue<1.0f)
		{
			lerpValue += dt * rotationSpeed;
			if(lerpValue>1.0f) lerpValue=1.0f;
		}
 		rotateToNewForward(cameraForward, baseRotation, lerpValue);
	}
}

void TPActor::actorInput(const InputState& inputState)
{
	Actor::actorInput(inputState);

	float strafeSpeed = 0.0f;
	moveComponent->setStrafeSpeed(strafeSpeed);
	
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	float y = mousePosition.y;
	const int maxMouseSpeed = 500;
	const float maxCameraRotationSpeed = Maths::pi * 8;
	
	float cameraAngularSpeed = 0.0f;
	if (x != 0)
	{
		cameraAngularSpeed = x / maxMouseSpeed;
		cameraAngularSpeed *= maxCameraRotationSpeed;
	}
	
	cameraComponent->setYawSpeed(cameraAngularSpeed);
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	cameraComponent->setPitchSpeed(pitchSpeed);
	
	inputState.mouse.getButtonState(SDL_BUTTON_RIGHT) == ButtonState::Held ? cameraLock = true : cameraLock = false;
}
