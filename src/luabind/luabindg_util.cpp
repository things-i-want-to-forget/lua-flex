#include "lau/lau.h"
#include "offset.h"
#include "classes/structures.h"
#include "classes/entities.h"
#include "classes/sprops.h"
#include "classes/engineclient.h"
#include "classes/enginetrace.h"
#include "sigscan/sigscan.h"
#include "classes/color.h"

#pragma warning(disable : 4244)

extern Structures structs;

class CAutowallFilter : public CTraceFilter
{
public:

	bool ShouldHitEntity(ClientEntity *ent, unsigned int mask)
	{

		return ent->GetRefEHandle() != this->other;

	}

public:

	CBaseHandle other;

};



bool CanAutowall(ClientEntity *other, const Vector &_startpos, const Vector &endpos, float *damage)
{
	Ray_t ray;
	trace_t ntr, ntrexit;

	ClientEntity *me =
		structs.entity_list->GetClientEntity(
			structs.engine->GetLocalPlayer()
			);

	FileWeaponInfo_t *data = me->GetActiveWeapon()->GetCSWpnData();

	static bool(__fastcall *TraceToExit)(Vector *end, trace_t *enter, Vector start, Vector dir, trace_t *exit) = 0;

	if (!TraceToExit)
		TraceToExit = (decltype(TraceToExit))SigScan(
			"\x55"
			"\x8B\xEC"
			"\x83\xEC?"
			"\xF3\x0F???"
			"\x33\xC0"
			"\xF3\x0F",
			GetModuleHandleA("client.dll")
			);

	//returns false if wallbangable
	//static bool(__thiscall *BulletHandler)(ClientEntity *me, float &penetration1, int &SurfaceMat, int *usestaticvalues, trace_t *ray, Vector *normaldelta, 
	//	float _setto0f_8, float surfacepenetration, float rangemodifier, int unknown, int _setto0x1002_12, float penetration2, int *hitsleft,
	//	Vector *ResultPos, float hitx, float hity, float *damage) = 0;
	//static bool (__fastcall *BulletHandler)(Vector *buffer, Ray_t *bufferray, Vector, Vector, trace_t *result)
	//
	//if (!BulletHandler)
	//	BulletHandler = (decltype(BulletHandler))SigScan(
	//		"\x55\x8B\xEC\x83\xEC\x70\x8B\x55?\x56\x8B\x75?\x89\x4D?\x32\xC9\x8B\x12\x57\x8A\x46?",
	//		GetModuleHandleA("client.dll")
	//	);

	ray.Init(_startpos, endpos);

	structs.trace->TraceRay(ray, 0x4600400B, 0, ntr);

	if (ntr.fraction >= 0.99f || ntr.hitent == other)
		return true;

	Vector dir = (endpos - _startpos).GetNormalized();

	int max = 5;
	float distanceleft = data->range();
	float distancetotal = 0.f;

	while (max--)
	{

		distancetotal += ntr.fraction * distanceleft;
		distanceleft -= ntr.fraction * distanceleft;
		*damage *= powf(data->range_modifier(), distancetotal * 0.002f);


		Vector endpoint;

		if (!TraceToExit(&endpoint, &ntr, ntr.endpos, dir, &ntrexit))
			return false;
		__asm add esp, 01Ch;

		float entermod, exitmod;

		surfacedata_t *entersurf = structs.sprops->GetSurfaceData(ntr.surface.surfaceProps);

		entermod = entersurf->game.penetration;
		exitmod = structs.sprops->GetSurfaceData(ntrexit.surface.surfaceProps)->game.penetration;

		float mod = 0.f;
		float mod2 = 0.16f;

		if (ntr.contents & 8 || entersurf->game.gamematerial == 89 || entersurf->game.gamematerial == 71)
		{

			mod2 = 0.05f;
			mod = 3.f;

		}
		else
			mod = (entermod + exitmod) / 2.f;

		if (ntr.surface.surfaceProps == ntrexit.surface.surfaceProps)
		{

			if (ntr.surface.surfaceProps == 87 || ntr.surface.surfaceProps == 85)
				mod = 3.f;

			else if (ntr.surface.surfaceProps == 76)
				mod = 2.f;

		}

		float length = ntr.endpos.Distance(ntrexit.endpos);

		float inversemod = fmaxf(0.f, 1.f / mod);

		length *= length;

		length *= inversemod;

		length /= 24;



		*damage -= fmaxf(0.f, length + (*damage * mod2) + inversemod * 3.f * fmaxf(0.f, 3.f / data->penetration() * 1.25f));

		if (*damage < 1.f)
			return false;

		if (ntrexit.endpos.Distance(endpos) == 0.f)
			break;

		if (ntrexit.hitent && ntrexit.hitent == other)
			break;

		ray.Init(endpoint, endpos);

		structs.trace->TraceRay(ray, 0x4600400B, 0, ntr);


	}

	return *damage >= 1;

}


// entity, startpos, endpos
int L_util_CanAutowall(lua_State *L)
{
	Vector &GetVector(lua_State *L, int where = -1);

	ClientEntity *e = structs.entity_list->GetClientEntityFromHandle(Get<CBaseHandle>(L, "Entity", 1));
	Vector &start = GetVector(L, 2);
	Vector &endpos = GetVector(L, 3);

	float damage = e->GetActiveWeapon()->GetCSWpnData()->damage();

	lua_pushboolean(L, CanAutowall(e, start, endpos, &damage));
	lua_pushnumber(L, damage);
	return 2;
}


luaL_Reg UtilLibrary[] = {
	{ "CanAutowall", L_util_CanAutowall },
	{ 0, 0 }
};