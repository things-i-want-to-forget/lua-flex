#include "structures.h"
#include "engineclient.h"
#include "matrix.h"
#include "vector.h"

#pragma warning(disable : 4244)

extern Structures structs;

float fScreenX, fScreenY;

bool ScreenTransform(const Vector& point, Vector& screen)
{
	VMatrix& worldToScreen = structs.engine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point.x + worldToScreen[0][1] * point.y 
		+ worldToScreen[0][2] * point.z
			+ worldToScreen[0][3];


	screen.y = worldToScreen[1][0] * point.x 
		+ worldToScreen[1][1] * point.y 
			+ worldToScreen[1][2] * point.z
				+ worldToScreen[1][3];

	float w = worldToScreen[3][0] * point.x + worldToScreen[3][1] * point.y + worldToScreen[3][2] * point.z + worldToScreen[3][3];
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

		fScreenX = (int)(width / 2);
		fScreenY = (int)(height / 2);

		fScreenX += 0.5f * screen.x * width + 0.5f;
		fScreenY -= 0.5f * screen.y * height + 0.5f;

		screen.x = fScreenX;
		screen.y = fScreenY;

		ret = true;
	}

	return ret;
}
