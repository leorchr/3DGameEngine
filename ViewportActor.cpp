#include "ViewportActor.h"
#include "MoveComponent.h"
#include "FPSCameraComponent.h"
#include "InputSystem.h"
#include "Game.h"
#include <iostream>

ViewportActor::ViewportActor() :
	Actor(),
	moveComponent(nullptr),
	cameraComponent(nullptr)
{
	Game::instance().getInputSystem().setMouseRelativeMode(false);
	SDL_WarpMouseInWindow(Game::instance().getWindow().getSDLWindow(),WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	moveComponent = new MoveComponent(this);
	cameraComponent = new FPSCameraComponent(this);
	setPosition(Vector3(-100.0f,100.0f,50.0f));
}

void ViewportActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	
	Vector3 cameraForward = cameraComponent->getViewForward();
	cameraForward.normalize();
	cameraForward *= forwardSpeed * dt;
	setPosition(getPosition() + cameraForward);
}

void ViewportActor::actorInput(const InputState& inputState)
{
	forwardSpeed = 0.0f;
	sideSpeed = 0.0f;
	// wasd movement
	if(inputState.mouse.getButtonState(SDL_BUTTON_RIGHT) == ButtonState::Pressed)
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		mousePos = Vector2(mouseX,mouseY);
		Game::instance().getInputSystem().setMouseRelativeMode(true);
	}
	if(inputState.mouse.getButtonState(SDL_BUTTON_RIGHT) == ButtonState::Released)
	{
		Game::instance().getInputSystem().setMouseRelativeMode(false);
		SDL_WarpMouseInWindow(Game::instance().getWindow().getSDLWindow(),mousePos.x, mousePos.y);
		moveComponent->setAngularSpeed(0.0f);
		moveComponent->setForwardSpeed(0.0f);
		moveComponent->setStrafeSpeed(0.0f);
		cameraComponent->setPitchSpeed(0.0f);
		return;
	}
	if(inputState.mouse.getButtonState(SDL_BUTTON_RIGHT) == ButtonState::Held)
	{
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
		{
			forwardSpeed += baseSpeed;
		}
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
		{
			forwardSpeed -= baseSpeed;
		}
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
		{
			sideSpeed -= baseSpeed;
		}
		if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
		{
			sideSpeed += baseSpeed;
		}

		// Set forward speed
		moveComponent->setStrafeSpeed(sideSpeed);
		
		// Mouse mouvement
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
		const float maxPitchSpeed = Maths::pi * 8;
		float pitchSpeed = 0.0f;
		if (y != 0)
		{
			pitchSpeed = y / maxMouseSpeed;
			pitchSpeed *= maxPitchSpeed;
		}
		cameraComponent->setPitchSpeed(pitchSpeed);
	}	
}

void ViewportActor::setBaseSpeed(float speed)
{
	baseSpeed = speed;
}