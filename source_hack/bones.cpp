#include "structures.h"
#include <string>
typedef unsigned char byte;
#include "entities.h"
#include "math.h"
#include "matsystemsurface.h"
#pragma warning(disable : 4244)

void DrawBones(ClientEntity *ent)
{
	matrix3x4_t bones[128];

	Vector corners[8];
	Vector screens[8];
	if (ent->SetupBones(bones, structs.globals->curtime()))
	{
		for (int i = 0; i < ent->GetNumHitboxes(); i++)
		{
			auto hitbox = ent->GetHitbox(i);
			if (!hitbox) continue;

			matrix3x4_t &bone = bones[hitbox->bone];

			Vector &min = hitbox->bbmin;
			Vector &max = hitbox->bbmax;


			VectorTransform(min, bone, corners[0]);	// m m m
			VectorTransform(Vector(min.x, max.y, min.z), bone, corners[1]);
			VectorTransform(Vector(max.x, max.y, min.z), bone, corners[2]);
			VectorTransform(Vector(max.x, min.y, min.z), bone, corners[3]);
			VectorTransform(Vector(min.x, min.y, max.z), bone, corners[4]);
			VectorTransform(Vector(min.x, max.y, max.z), bone, corners[5]);
			VectorTransform(max, bone, corners[6]);
			VectorTransform(Vector(max.x, min.y, max.z), bone, corners[7]);

			Vector topp;
			Vector bottom;

			for (int i = 0; i < 8; i++)
			{
				if (corners[i].ToScreen(screens[i]))
					continue;
				goto killbreak;
			}

			for (int i = 0; i < 8; i++)
			{
				if (i < 4) // z axis
				{
					structs.surface->DrawLine(screens[i].x, screens[i].y, screens[i + 4].x, screens[i + 4].y);
				}
				if ((i + 1) % 4 != 0) // x and y axis
				{
					structs.surface->DrawLine(screens[i].x, screens[i].y, screens[i + 1].x, screens[i + 1].y);
				}
				else
					structs.surface->DrawLine(screens[i].x, screens[i].y, screens[i - 3].x, screens[i - 3].y);

			}
			continue;
		killbreak:
			break;
		}
	}
}