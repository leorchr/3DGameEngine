#pragma once
#include "UIScreen.h"
#include "Text.h"
#include <vector>

class GameOverScreen : public UIScreen
{
public:
	GameOverScreen();
	~GameOverScreen() override;
	void draw(class Shader& shader) override;
	void processInput(const class InputState& inputState) override;
	class Text* text;
};