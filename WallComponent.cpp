#include "WallComponent.h"
#include "Game.h"

WallComponent::WallComponent(Actor* owner) : Component(owner)
{
	Game::instance().getHUD()->addWallComponent(this);
}

WallComponent::~WallComponent()
{
	Game::instance().getHUD()->removeWallComponent(this);
}