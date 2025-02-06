#pragma once
#include "UIScreen.h"

class SpaceshipUi : public UIScreen
{
public:
	SpaceshipUi(class SpaceshipActor& spaceshipActor);
	~SpaceshipUi() override;
	void draw(class Shader& shader) override;
	void updateText() const;

private:
	class SpaceshipActor& spaceshipActor;
	class Text* text;
};