#pragma once
#include "UIScreen.h"
#include "Text.h"
#include <vector>

class HitPoints : public UIScreen
{
public:

	static HitPoints& instance()
	{
		static HitPoints inst;
		return inst;
	}

	HitPoints(const HitPoints&) = delete;
	HitPoints& operator=(const HitPoints&) = delete;
	HitPoints(HitPoints&&) = delete;
	HitPoints& operator=(HitPoints&&) = delete;

	HitPoints();
	~HitPoints() override;
	void draw(class Shader& shader) override;
	void update(float dt) override;

	void setHP(int hpP);
	int getHP() { return hp; }

	class Text* text;

private:
	int hp;
	bool onlyOnce;
};