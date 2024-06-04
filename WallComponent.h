#pragma once
#include "Component.h"
class WallComponent : public Component
{
public:
	WallComponent(class Actor* owner);
	~WallComponent();
};