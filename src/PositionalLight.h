#pragma once
#include "Vector3.h"
struct PositionalLight
{
	Vector3 position;
	Vector3 diffuseColor;
	Vector3 specColor;
};

struct DirectionalLight
{
	Vector3 direction;
	Vector3 diffuseColor;
	Vector3 specColor;
};