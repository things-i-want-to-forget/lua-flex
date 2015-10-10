#include "lau/lau.h"
#include "offset.h"
#include "classes/structures.h"
#include "classes/entities.h"
#include "classes/sprops.h"
#include "classes/engineclient.h"
#include "classes/enginetrace.h"
#include "sigscan/sigscan.h"
#include "classes/clienttools.h"
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
	static bool(__thiscall *BulletHandler)(ClientEntity *me, float &DistanceLeft, int &SurfaceMat, int *usestaticvalues, trace_t *ray, Vector *normaldelta, 
		float _setto0f_8, float surfacepenetration, float damagemultiplier, int unknown, int _setto0x1002_12, float penetration2, int *hitsleft,
		Vector *ResultPos, float hitx, float hity, float *damage) = 0;
	//static bool (__fastcall *BulletHandler)(Vector *buffer, Ray_t *bufferray, Vector, Vector, trace_t *result)
	//
	if (!BulletHandler)
		BulletHandler = (decltype(BulletHandler))SigScan(
			"\x55\x8B\xEC\x83\xEC\x70\x8B\x55?\x56\x8B\x75?\x89\x4D?\x32\xC9\x8B\x12\x57\x8A\x46?",
			GetModuleHandleA("client.dll")
		);

	surfacedata_t *entersurf = structs.sprops->GetSurfaceData(ntr.surface.surfaceProps);
	int usestaticvalues = 0;

	Vector normal = (endpos - _startpos).GetNormalized();
	float lengthleft = (endpos - _startpos).Length();

	int material = entersurf->game.gamematerial;

	int hitsleft = 4;
	Vector result(_startpos);
	float tempdamage;

	do
	{

		ray.Init(result, endpos);

		structs.trace->TraceRay(ray, 0x4600400B, 0, ntr);

		if (ntr.hitgroup != 0)
			return ntr.hitent == other;


		tempdamage = 0.f;
		if ( BulletHandler(me, lengthleft, material, &usestaticvalues, &ntr, &normal,
			0.f, entersurf->game.penetration, entersurf->game.damagepenetration, 0, 0x1002, data->penetration(), &hitsleft,
			&result, 0.f, 0.f, &tempdamage) && (tempdamage == 0.f || (*damage - tempdamage) < 1.f) )

			break;

		*damage -= tempdamage;


	} while (hitsleft > 0);


	return false;

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