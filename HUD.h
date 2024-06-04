#pragma once
#include "UIScreen.h"
#include "WallComponent.h"

class HUD : public UIScreen
{
public:
	HUD();
	~HUD();

	void update(float dt) override;
	void draw(class Shader& shader) override;

	void addTargetComponent(class TargetComponent* tc);
	void removeTargetComponent(class TargetComponent* tc);
	void addWallComponent(class WallComponent* wc);
	void removeWallComponent(class WallComponent* wc);


protected:
	void updateCrosshair(float deltaTime);
	void updateRadar(float deltaTime);

	class Texture* crosshair;
	class Texture* crosshairEnemy;
	class Texture* radar;
	class Texture* blipTex;
	class Texture* wallTex;
	class Texture* radarArrow;

	std::vector<class TargetComponent*> targetComponents;
	std::vector<class WallComponent*> wallComponents;
	bool isTargetingEnemy;
	vector<Vector2> blips;
	vector<Vector2> walls;
	float radarRange;
	float radarRadius;
};