#include "lau/lau.h"
#include "classes/structures.h"
#include "classes/enginetrace.h"
#include "classes/entities.h"
#include "classes/sprops.h"

#pragma warning(disable : 4244) // NICE ONE LUA

class LuaFilter : public CTraceFilter
{
public:
	inline void Init(lua_State *L, lua_Integer func)
	{
		this->L = L;
		reg = func;
	}
	inline void Die(void)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, reg);
	}

	virtual bool ShouldHitEntity(ClientEntity *ent, unsigned int mask)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, reg);
		LPush(L, ent->GetRefEHandle(), "Entity");
		bool ret = false;
		if (lua_pcall(L, 1, 1, 0) == 0)
		{
			ret = 0 != lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
		return ret;
	}

	lua_State *L;
	lua_Integer reg;

};

int Ltrace_Ray(lua_State *L)
{
	Ray_t ray;
	LuaFilter filter;
	trace_t result;
	CTraceFilter *passed_filter = 0; 
	
	Vector &GetVector(lua_State *L, int where = -1);
	ray.Init(GetVector(L, 1), GetVector(L, 2));

	int mask = (int)lua_tonumber(L, 3);

	if (lua_gettop(L) >= 4)
	{
		lua_pushvalue(L, 4);
		
		filter.Init(L, luaL_ref(L, LUA_REGISTRYINDEX));

		passed_filter = &filter;
	}

	structs.trace->TraceRay(ray, mask, passed_filter, result);

	if (lua_gettop(L) >= 4)
	{
		filter.Die();
	}

	lua_newtable(L);
	{
		lua_pushboolean(L, result.allsolid);
		lua_setfield(L, -2, "allsolid");
		lua_pushboolean(L, result.startsolid);
		lua_setfield(L, -2, "startsolid");
		lua_pushinteger(L, result.bone);
		lua_setfield(L, -2, "bone");

		surfacedata_t *dat = structs.sprops->GetSurfaceData(result.surface.surfaceProps);
		lua_pushinteger(L, dat->game.gamematerial);// yes this is right  - it's overloaded
		lua_setfield(L, -2, "material");

		lua_pushinteger(L, result.disp_flags);
		lua_setfield(L, -2, "disp_flags");

		LPush(L, result.endpos, "Vector");
		lua_setfield(L, -2, "endpos");
		LPush(L, result.startpos, "Vector");
		lua_setfield(L, -2, "startpos");

		LPush(L, result.plane.normal, "Vector");
		lua_setfield(L, -2, "normal");

		lua_pushnumber(L, result.fraction);
		lua_setfield(L, -2, "fraction");


		lua_pushnumber(L, result.fraction_left_solid);
		lua_setfield(L, -2, "fractionleft");

		lua_pushinteger(L, result.contents);
		lua_setfield(L, -2, "contents");

		lua_pushinteger(L, result.hitbox);
		lua_setfield(L, -2, "hitbox");

		if (result.hitent)
		{
			LPush(L, result.hitent->GetRefEHandle(), "Entity");
			lua_setfield(L, -2, "hitent");
		}

		lua_pushinteger(L, result.hitgroup);
		lua_setfield(L, -2, "hitgroup");

		lua_pushstring(L, result.surface.name);
		lua_setfield(L, -2, "matname");

		lua_pushinteger(L, result.surface.surfaceProps);
		lua_setfield(L, -2, "surfaceprop");
	}

	return 1;
}

luaL_Reg TraceLibrary[] = {
	{ "Ray", Ltrace_Ray },
	{ 0, 0 }
};