#pragma once
#include <memory.h>
#include "Vector3.h"
#include "Quaternion.h"

class Matrix3
{
public:
	float mat[3][3];

	Matrix3()
	{
		*this = Matrix3::identity;
	}

	explicit Matrix3(float inMat[3][3])
	{
		memcpy(mat, inMat, 9 * sizeof(float));
	}

	// Cast to a const float pointer
	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&mat[0][0]);
	}

	friend Vector3 operator*(const Matrix3& a, const Vector3& b)
	{
		Vector3 result;

		// Perform matrix-vector multiplication
		result.x = a.mat[0][0] * b.x + a.mat[0][1] * b.y + a.mat[0][2] * b.z;
		result.y = a.mat[1][0] * b.x + a.mat[1][1] * b.y + a.mat[1][2] * b.z;
		result.z = a.mat[2][0] * b.x + a.mat[2][1] * b.y + a.mat[2][2] * b.z;

		return result;
	}
		
	// Matrix multiplication (a * b)
	friend Matrix3 operator*(const Matrix3& a, const Matrix3& b)
	{
		Matrix3 retVal;

		// row 0
		retVal.mat[0][0] =
			a.mat[0][0] * b.mat[0][0] +
			a.mat[0][1] * b.mat[1][0] +
			a.mat[0][2] * b.mat[2][0];

		retVal.mat[0][1] =
			a.mat[0][0] * b.mat[0][1] +
			a.mat[0][1] * b.mat[1][1] +
			a.mat[0][2] * b.mat[2][1];

		retVal.mat[0][2] =
			a.mat[0][0] * b.mat[0][2] +
			a.mat[0][1] * b.mat[1][2] +
			a.mat[0][2] * b.mat[2][2];

		// row 1
		retVal.mat[1][0] =
			a.mat[1][0] * b.mat[0][0] +
			a.mat[1][1] * b.mat[1][0] +
			a.mat[1][2] * b.mat[2][0];

		retVal.mat[1][1] =
			a.mat[1][0] * b.mat[0][1] +
			a.mat[1][1] * b.mat[1][1] +
			a.mat[1][2] * b.mat[2][1];

		retVal.mat[1][2] =
			a.mat[1][0] * b.mat[0][2] +
			a.mat[1][1] * b.mat[1][2] +
			a.mat[1][2] * b.mat[2][2];

		// row 2
		retVal.mat[2][0] =
			a.mat[2][0] * b.mat[0][0] +
			a.mat[2][1] * b.mat[1][0] +
			a.mat[2][2] * b.mat[2][0];

		retVal.mat[2][1] =
			a.mat[2][0] * b.mat[0][1] +
			a.mat[2][1] * b.mat[1][1] +
			a.mat[2][2] * b.mat[2][1];

		retVal.mat[2][2] =
			a.mat[2][0] * b.mat[0][2] +
			a.mat[2][1] * b.mat[1][2] +
			a.mat[2][2] * b.mat[2][2];

		return retVal;
	}

	Matrix3& operator*=(const Matrix3& right)
	{
		*this = *this * right;
		return *this;
	}

	// Invert the matrix - super slow
	Matrix3 getInverse();
	
	static const Matrix3 identity;
};