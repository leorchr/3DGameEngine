#include "Matrix3.h"

static float m3Ident[3][3] =
{
	{ 1.0f, 0.0f, 0.0f},
	{ 0.0f, 1.0f, 0.0f},
	{ 0.0f, 0.0f, 1.0f}
};

const Matrix3 Matrix3::identity(m3Ident);

Matrix3 Matrix3::getInverse()
{
	float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
				mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
				mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

	float invDet = 1.0f / det;

	// Calcul des cofacteurs et transposition
	Matrix3 result;
	result.mat[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * invDet;
	result.mat[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invDet;
	result.mat[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invDet;

	result.mat[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invDet;
	result.mat[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invDet;
	result.mat[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) * invDet;

	result.mat[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * invDet;
	result.mat[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) * invDet;
	result.mat[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * invDet;

	return result;
}