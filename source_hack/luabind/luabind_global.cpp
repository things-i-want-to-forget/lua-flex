#include "lua.hpp"
#include "../color.h"
#include "../vector.h"
#include "../angle.h"
#include "../structures.h"
#include "../lau/lau.h"
#include "../engineclient.h"
#include "../globals.h"
#include <Windows.h>
#pragma comment(lib, "tier0.lib")

#pragma warning(disable : 4244)

Color print_color = Color(255, 255, 0, 255);

__declspec(dllimport) void __cdecl ConColorMsg(const Color &, const char *, ...);

inline const char *tostring(lua_State *L, int stk)
{
	lua_pushvalue(L, stk);
	lua_pushglobaltable(L);
	lua_getfield(L, -1, "tostring");
	lua_pushvalue(L, -3);
	lua_call(L, 1, 1);
	const char *ret = lua_tostring(L, -1);
	lua_pop(L, 3);
	return ret;

}

int L_print(lua_State *L)
{
	for (int i = 1; i <= lua_gettop(L); i++)
	{
		ConColorMsg(print_color, "%s\t", tostring(L, i));
	}
	ConColorMsg(print_color, "\n");
	return 0;
}

int L_Vector(lua_State *L)
{
	LPush(L, Vector(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3)), "Vector");
	return 1;
}

int L_Angle(lua_State *L)
{
	LPush(L, QAngle(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3)), "Angle");
	return 1;
}

int L_FindMetaTable(lua_State *L)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, structs.L->MetatableIndex());
	lua_pushvalue(L, 1);
	lua_rawget(L, -2);
	lua_remove(L, -2);
	return 1;
}
int L_IsInGame(lua_State *L)
{
	lua_pushboolean(L, structs.engine->IsInGame());
	return 1;
}

int L_include(lua_State *L)
{

	return structs.L->RunLuaFile(lua_tostring(L, 1));

}

int L_CurTime(lua_State *L)
{
	lua_pushnumber(L, structs.globals->curtime());
	return 1;
}

int L_FrameTime(lua_State *L)
{
	lua_pushnumber(L, structs.globals->frametime());
	return 1;
}

int L_TickInterval(lua_State *L)
{
	lua_pushnumber(L, structs.globals->tickinterval());
	return 1;
}

int L_MaxClients(lua_State *L)
{
	lua_pushnumber(L, structs.globals->maxclients());
	return 1;
}

luaL_Reg GlobalLibrary[] = {
	{ "print", L_print },
	{ "Vector", L_Vector },
	{ "Angle", L_Angle },
	{ "FindMetaTable", L_FindMetaTable },
	{ "include", L_include },
	{ "IsInGame", L_IsInGame },
	{ "CurTime", L_CurTime },
	{ "FrameTime", L_FrameTime },
	{ "TickInterval", L_TickInterval },
	{ "MaxClients", L_MaxClients },
	{ 0, 0 }
};