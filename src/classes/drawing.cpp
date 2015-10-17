#include "structures.h"
#include "engineclient.h"
#include "matrix.h"
#include "math.h"
#include "vector.h"

#pragma warning(disable : 4244)

extern Structures structs;

float fScreenX, fScreenY;

bool ScreenTransform(const Vector& point, Vector& screen)
{

	float buffer[4];


	VMatrix& worldToScreen = structs.engine->WorldToScreenMatrix();
	
	_mm_store_ps(buffer, _mm_mul_ps(
		_mm_set_ps(worldToScreen[0][0], worldToScreen[0][1],
			worldToScreen[0][2], worldToScreen[0][3]),
		_mm_set_ps(point.x, point.y, point.z, 1)
	));

	screen.x = buffer[0] + buffer[1] + buffer[2] + buffer[3];


	_mm_store_ps(buffer, _mm_mul_ps(
		_mm_set_ps(worldToScreen[1][0], worldToScreen[1][1],
			worldToScreen[1][2], worldToScreen[1][3]),
		_mm_set_ps(point.x, point.y, point.z, 1)
	));

	screen.y = buffer[0] + buffer[1] + buffer[2] + buffer[3];

	_mm_store_ps(buffer, _mm_mul_ps(
		_mm_set_ps(worldToScreen[3][0], worldToScreen[3][1],
			worldToScreen[3][2], worldToScreen[3][3]),
		_mm_set_ps(point.x, point.y, point.z, 1)
	));

	float w = buffer[0] + buffer[1] + buffer[2] + buffer[3];

	screen.z = 0.0f;



	bool behind = false;
	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool WorldToScreen(const Vector &origin, Vector &screen)
{
	bool ret = false;

	if (!ScreenTransform(origin, screen))
	{
		int width, height;
		structs.engine->GetScreenSize(&width, &height);

		fScreenX = int(width / 2);
		fScreenY = int(height / 2);

		fScreenX += 0.5f * screen.x * width + 0.5f;
		fScreenY -= 0.5f * screen.y * height + 0.5f;

		screen.x = fScreenX;
		screen.y = fScreenY;

		ret = true;
	}

	return ret;
}
