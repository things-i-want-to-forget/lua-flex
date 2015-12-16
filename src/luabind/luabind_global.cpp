#include "lua.hpp"
#include "../classes/color.h"
#include "../classes/vector.h"
#include "../classes/angle.h"
#include "../classes/structures.h"
#include "../lau/lau.h"
#include "../classes/engineclient.h"
#include "../classes/entities.h"
#include "classes/usercmd.h"
#include "../classes/globals.h"
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

int L_GetAsyncKeyState(lua_State *L)
{

	if (lua_type(L, 1) == LUA_TNUMBER)
		lua_pushinteger(L, GetAsyncKeyState(lua_tonumber(L, 1)));
	else
		lua_pushinteger(L, GetAsyncKeyState(*lua_tostring(L, 1)));

	return 1;

}

int L_print(lua_State *L)
{

	for (int i = 1; i <= lua_gettop(L); i++)
		ConColorMsg(print_color, "%s\t", tostring(L, i));

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

	lua_rawget(L, LUA_REGISTRYINDEX);

	lua_pushvalue(L, 1);

	return 1;

}

int L_IsInGame(lua_State *L)
{

	lua_pushboolean(L, structs.engine->IsInGame());

	return 1;

}

int L_include(lua_State *L)
{

	if (!structs.L->RunLuaFile(lua_tostring(L, 1)))
		lua_error(L);

	return 1;

}

int L_LocalPlayer(lua_State *L)
{

	LPush<CBaseHandle>(L, 
		structs.entity_list->GetClientEntity(
			structs.engine->GetLocalPlayer()
		)->GetRefEHandle(), "Entity");
	
	return 1;

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

int L_GetScreenSize(lua_State *L)
{
	int w, h;

	structs.engine->GetScreenSize(&w, &h);

	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2;
}

int L_GenerateUserCmd(lua_State *L)
{

	CUserCmd *cmd = (CUserCmd *)malloc(0x300);
	LPush(L, cmd, "CUserCmd");

	return 1;

}

int L_DestroyUserCmd(lua_State *L)
{

	free(Get<CUserCmd *>(L, "CUserCmd", 1));

	return 0;

}

int L_RunConsoleCommand(lua_State *L)
{
	structs.engine->ClientCmd_Unrestricted(luaL_checkstring(L, 1));

	return 0;
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
	{ "LocalPlayer", L_LocalPlayer },
	{ "GetAsyncKeyState", L_GetAsyncKeyState },
	{ "GetScreenSize", L_GetScreenSize },
	{ "GenerateUserCmd", L_GenerateUserCmd },
	{ "DestroyUserCmd", L_DestroyUserCmd },
	{ "RunConsoleCommand", L_RunConsoleCommand },
	{ 0, 0 }
};
