#include "Quaternion.h"

const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion(float xP, float yP, float zP, float wP)
{
	set(xP, yP, zP, wP);
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float scalar = Maths::sin(angle / 2.0f);
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = Maths::cos(angle / 2.0f);
}

void Quaternion::set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

void Quaternion::conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

void Quaternion::normalize()
{
	float len = length();
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Vector3 axis = Vector3(x,y,z);
	Vector3 axisQ = Vector3(q.x,q.y,q.z);
	float scalar= w*q.w - Vector3::dot(axis, axisQ);

	Vector3 imaginary=axisQ*w + axis*q.w + Vector3::cross(axis,axisQ);

	return Quaternion(imaginary,scalar);
}