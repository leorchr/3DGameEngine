#pragma once
#include "Quaternion.h"
#include "Vector3.h"

struct OBB
{
	OBB(const Vector3& center, const Vector3& extents, const Quaternion& rotation);
	
	Vector3 center;
	Vector3 extents;
	Quaternion rotation;
};
