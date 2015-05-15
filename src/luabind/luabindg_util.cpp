#include "lua.hpp"
#include <Windows.h>
#include <string>
#include "../offset.h"
#include "../classes/structures.h"
#include "../classes/entities.h"
#include "../classes/sprops.h"
#include "../classes/engineclient.h"
#include "../classes/enginetrace.h"

#pragma warning(disable : 4244)

extern Structures structs;

class CTraceFilterAutowall : public CTraceFilter
{
public:
	CTraceFilterAutowall(ClientEntity *ent, ClientEntity *other)
	{
		this->ent = ent;
		this->other = other;
	}
	bool ShouldHitEntity(ClientEntity *ent)
	{
		return ent == this->ent || other == this->ent;
	}

public:
	ClientEntity *ent;
	ClientEntity *other;
};
//trace_t &tr = argument
//Vector &norm = argument

bool CanAutowall(ClientEntity *other, const Vector &_startpos, const Vector &endpos)
{
	CTraceFilterAutowall filter(structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer()), other);



	int first_contents = 0;
	trace_t ntr;
	Ray_t ray;

	ray.Init(_startpos, endpos);

	structs.trace->TraceRay(ray, 0x4600400B, &filter, ntr);
	if (ntr.fraction >= 0.999f)
		return true;
	Vector vEnd = ntr.endpos;
	ray.Init(endpos, _startpos);

	structs.trace->TraceRay(ray, 0x4600400B, &filter, ntr);
	float len = (vEnd - ntr.endpos).Length();


	surfacedata_t *prop = structs.sprops->GetSurfaceData(ntr.surface.surfaceProps);
	if (!prop)
		return false;

	auto v38 = prop->game.material;
	auto v37 = (ntr.bone & (1 << 7));
	float v39, v40;
	if (v38 == 70) // MAT_FLESH
	{
		return false;
	}
	else if (v38 == 71 || v38 == 89)
	{
		v39 = 3.0f;
		v40 = 0.05f;
	}
	else
	{
		v40 = 0.16f;
		v39 = 0.5f;
	}

	if (prop->game.material == prop->gamematerial())
	{
		if ((prop->gamematerial() & 0xFFFD) == 85) // MAT_GRASS
		{
			v39 = 3.0f;
		}
		else if (prop->gamematerial() == 19) //  ??????????
		{
			v39 = 2.0f;
		}
	}

	float distend = (ntr.endpos - ntr.endpos).Length();
	auto v56 = fmaxf(1 / v39, 0.0f);
	ClientEntity *wep = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer())->GetActiveWeapon();
	auto a13 = wep->GetCSWpnData()->penetration();
	auto v58 = v56 * ((double)(3.0) * fmax(0.0, 3.0f / a13)) * 1.25 + v40;

	auto v57 = wep->GetCSWpnData()->penetration() * 25.0f * prop->penetration();

	auto v59 = (float)(distend * distend) * v56 / 24 + v58;

	auto v61 = v57 - fmaxf(0.0f, v59);

	return (v61 - len) > 0;
}

// entity, startpos, endpos
int L_util_CanAutowall(lua_State *L)
{
	ClientEntity *e = structs.entity_list->GetClientEntityFromHandle(Get<CBaseHandle>(L, 1));
	Vector &start = Get<Vector>(L, 2);
	Vector &endpos = Get<Vector>(L, 3);

	lua_pushboolean(L, CanAutowall(e, start, endpos));
	return 1;
}


luaL_Reg UtilLibrary[] = {
	{ "CanAutowall", L_util_CanAutowall },
	{ 0, 0 }
};