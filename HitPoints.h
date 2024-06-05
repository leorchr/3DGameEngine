#pragma once
#include "UIScreen.h"
#include "Text.h"
#include <vector>

class HitPoints : public UIScreen
{
public:
	HitPoints();
	~HitPoints() override;
	void draw(class Shader& shader) override;
	void update(float dt) override;

	class Text* text;

private:
	int hp;
	bool onlyOnce;
};