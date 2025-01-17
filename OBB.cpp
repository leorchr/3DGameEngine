#include "OBB.h"

OBB::OBB(const Vector3& center, const Vector3& extents, const Quaternion& rotation)
	: center(center), extents(extents), rotation(rotation){}