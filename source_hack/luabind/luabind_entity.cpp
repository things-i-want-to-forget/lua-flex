#include "lua.hpp"
#include "../angle.h"
#include "../vector.h"
#include "../structures.h"
#include "../clienttools.h"
#include "../entities.h"
#include "../engineclient.h"

#pragma warning(disable : 4244)

static ClientEntity *GetEntity(lua_State *L, int where = -1)
{
	CBaseHandle &handle = Get<CBaseHandle>(L, where);

	return structs.entity_list->GetClientEntityFromHandle(handle);
}

#define VALIDATE(e) \
	if (!e) \
	{ \
		lua_pushstring(L, "Used invalid entity"); \
		lua_error(L); \
	}

int L_Entity_IsValid(lua_State *L)
{
	lua_pushboolean(L, GetEntity(L, 1) != 0);
	return 1;
}

int L_Entity_IsPlayer(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushboolean(L, structs.tools->IsPlayer(e));
	return 1;
}

int L_Entity_IsWeapon(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushboolean(L, structs.tools->IsWeapon(e));
	return 1;
}

int L_Entity_GetPos(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	LPush(L, e->GetAbsOrigin(), "Vector");
	return 1;
}

int L_Entity_IsDormant(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushboolean(L, e->IsDormant());
	return 1;
}

int L_Entity_GetHitboxCount(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushinteger(L, e->GetNumHitboxes());
	return 1;
}

int L_Entity_OnGround(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushboolean(L, e->IsOnGround());
	return 1;
}
int L_Entity_GetActiveWeapon(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	LPush<CBaseHandle>(L, e->GetActiveWeapon()->GetRefEHandle(), "Entity");
	return 1;
}

int L_Entity_CanShoot(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushboolean(L, e->CanShoot(structs.globals->curtime()));
	return 1;
}

int L_Entity_GetWeaponName(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	static char temp[512];
	wsprintfA(temp, "%S", structs.local->Find(e->GetCSWpnData()->printname));
	lua_pushstring(L, temp);
	return 1;
}

int L_Entity_GetPenetration(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushnumber(L, e->GetCSWpnData()->penetration());
	return 1;
}

int L_Entity_GetDamage(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushnumber(L, e->GetCSWpnData()->damage());
	return 1;
}

int L_Entity_GetWeaponClass(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	lua_pushstring(L, e->GetCSWpnData()->class_name);

	return 1;
}

int L_Entity_Nick(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);
	player_info_t info;

	structs.engine->GetPlayerInfo(e->GetRefEHandle() & 0xFFF, &info);

	lua_pushstring(L, info.GetName());

	return 1;
}

int L_Entity_OBBMaxs(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	LPush(L, e->GetStudioHdr()->studio->hull_max, "Vector");
	return 1;
}

int L_Entity_OBBMins(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	LPush(L, e->GetStudioHdr()->studio->hull_min, "Vector");
	return 1;
}

int L_Entity_ModelName(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	lua_pushstring(L, e->GetStudioHdr()->studio->name);
	return 1;
}

int L_Entity_EyePos(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	LPush(L, e->GetStudioHdr()->studio->eyepos, "Vector");
	return 1;
}

int L_Entity_GetNWInt(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	int offset = -1;
	if (lua_gettop(L) > 2)
		offset = lua_tointeger(L, 3);

	lua_pushinteger(L, e->getvar<int>(lua_tostring(L, 2), &offset));
	lua_pushinteger(L, offset);


	return 2;
}
int L_Entity_GetNWFloat(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	int offset = -1;
	if (lua_gettop(L) > 2)
		offset = lua_tointeger(L, 3);

	lua_pushnumber(L, e->getvar<float>(lua_tostring(L, 2), &offset));
	lua_pushinteger(L, offset);


	return 2;
}
int L_Entity_GetNWVector(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	int offset = -1;
	if (lua_gettop(L) > 2)
		offset = lua_tointeger(L, 3);

	LPush(L, e->getvar<Vector>(lua_tostring(L, 2), &offset), "Vector");
	lua_pushinteger(L, offset);


	return 2;
}
int L_Entity_GetNWAngle(lua_State *L)
{
	ClientEntity *e = GetEntity(L, 1);
	VALIDATE(e);

	int offset = -1;
	if (lua_gettop(L) > 2)
		offset = lua_tointeger(L, 3);

	LPush(L, e->getvar<QAngle>(lua_tostring(L, 2), &offset), "QAngle");
	lua_pushinteger(L, offset);


	return 2;
}

int L_Entity___index(lua_State *L)
{
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);
	lua_remove(L, -2);
	return 1;
}


luaL_Reg LuaEntityMetaTable[] = {
	{ "IsWeapon", L_Entity_IsWeapon },
	{ "IsValid", L_Entity_IsValid },
	{ "IsPlayer", L_Entity_IsPlayer },
	{ "GetPos", L_Entity_GetPos },
	{ "IsDormant", L_Entity_IsDormant },
	{ "GetHitboxCount", L_Entity_GetHitboxCount },
	{ "OnGround", L_Entity_OnGround },
	{ "IsOnGround", L_Entity_OnGround },
	{ "GetActiveWeapon", L_Entity_GetActiveWeapon },
	{ "GetWeaponName", L_Entity_GetWeaponName },
	{ "GetPenetration", L_Entity_GetPenetration },
	{ "GetDamage", L_Entity_GetPenetration },
	{ "GetWeaponClass", L_Entity_GetWeaponClass },
	{ "Nick", L_Entity_Nick },
	{ "OBBMins", L_Entity_OBBMins },
	{ "OBBMaxs", L_Entity_OBBMaxs },
	{ "EyePos", L_Entity_EyePos },
	{ "ModelName", L_Entity_ModelName },
	{ "GetNWInt", L_Entity_GetNWInt },
	{ "GetNWFloat", L_Entity_GetNWFloat },
	{ "GetNWAngle", L_Entity_GetNWAngle },
	{ "GetNWVector", L_Entity_GetNWVector },
	{ "__index", L_Entity___index },
	{ 0, 0 }
};