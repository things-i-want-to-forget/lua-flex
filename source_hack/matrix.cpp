#include "matrix.h"
#include "vector.h"

bool FrustumTransform(VMatrix &matrix, Vector &point, Vector &screen)
{
	// UNDONE: Clean this up some, handle off-screen vertices
	float w;
	screen.x = matrix[0][0] * point[0] + matrix[0][1] * point[1] + matrix[0][2] * point[2] + matrix[0][3];
	screen.y = matrix[1][0] * point[0] + matrix[1][1] * point[1] + matrix[1][2] * point[2] + matrix[1][3];
	// z = worldToSurface[2][0] * point[0] + worldToSurface[2][1] * point[1] + worldToSurface[2][2] * point[2] + worldToSurface[2][3];
	w = matrix[3][0] * point[0] + matrix[3][1] * point[1] + matrix[3][2] * point[2] + matrix[3][3];
	// Just so we have something valid here
	screen.z = 0.0f;
	bool behind;
	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}
	return behind;
}