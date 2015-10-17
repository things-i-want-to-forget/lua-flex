#include "lau/lau.h"
#include "../classes/usercmd.h"
#include "../classes/angle.h"
#include "../classes/vector.h"
#include <string.h>
#pragma warning(disable : 4244)

CUserCmd *&GetCUserCmd(lua_State *L, int where = -1) 
{

	return Get<CUserCmd *>(L, "CUserCmd", where);

}
extern bool bSendPacket;

int L_CMD___index(lua_State *L)
{

	CUserCmd *cmd = GetCUserCmd(L, 1);

	const char *str = lua_tostring(L, 2);
	if (!strcmp(str, "command_number")) 
		lua_pushnumber(L, cmd->command_number);

	else if (!strcmp(str, "tickcount")) // beWARE READONLY
		lua_pushnumber(L, cmd->tickcount);

	else if (!strcmp(str, "angles"))    // beWARE READONLY
		LPush(L, cmd->angles, "Angle");

	else if (!strcmp(str, "p"))
		lua_pushnumber(L, cmd->angles.p);

	else if (!strcmp(str, "y"))
		lua_pushnumber(L, cmd->angles.y);

	else if (!strcmp(str, "r"))
		lua_pushnumber(L, cmd->angles.r);

	else if (!strcmp(str, "buttons"))
		lua_pushinteger(L, cmd->buttons());

	else if (!strcmp(str, "mousex"))
		lua_pushnumber(L, cmd->mousex());

	else if (!strcmp(str, "mousey"))
		lua_pushnumber(L, cmd->mousey());

	else if (!strcmp(str, "sidemove"))
		lua_pushnumber(L, cmd->sidemove());

	else if (!strcmp(str, "forwardmove"))
		lua_pushnumber(L, cmd->forwardmove());

	else if (!strcmp(str, "upmove"))
		lua_pushnumber(L, cmd->upmove());

	else if (!strcmp(str, "sendpacket"))
		lua_pushboolean(L, bSendPacket != false);

	else 
	{

		lua_getmetatable(L, 1);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		lua_remove(L, -2); // pop metatable

	}

	return 1;
}

int L_CMD___newindex(lua_State *L)
{
	QAngle &GetAngle(lua_State *L, int where = -1);

	CUserCmd *cmd = GetCUserCmd(L, 1);

	const char *str = luaL_checkstring(L, 2);
	if (!strcmp(str, "angles"))
		cmd->angles = GetAngle(L, 3);

	else if (!strcmp(str, "p"))
		cmd->angles.p = luaL_checknumber(L, 3);

	else if (!strcmp(str, "y"))
		cmd->angles.y = luaL_checknumber(L, 3);

	else if (!strcmp(str, "r"))
		cmd->angles.r = luaL_checknumber(L, 3);

	else if (!strcmp(str, "buttons"))
		cmd->buttons() = luaL_checkinteger(L, 3);

	else if (!strcmp(str, "sendpacket"))
		bSendPacket = 1 == lua_toboolean(L, 3);

	else if (!strcmp(str, "tickcount"))
		cmd->tickcount = luaL_checkinteger(L, 3);

	else if (!strcmp(str, "mousex"))
		cmd->mousex() = luaL_checknumber(L, 3);

	else if (!strcmp(str, "mousey"))
		cmd->mousey() = luaL_checknumber(L, 3);

	else if (!strcmp(str, "sidemove"))
		cmd->sidemove() = luaL_checknumber(L, 3);

	else if (!strcmp(str, "forwardmove"))
		cmd->forwardmove() = luaL_checknumber(L, 3);

	else if (!strcmp(str, "upmove"))
		cmd->upmove() = luaL_checknumber(L, 3);

	else
	{
		lua_pushstring(L, "Unsupported member set in cusercmd!\n");
		lua_error(L);
	}
	return 0;
}

luaL_Reg LuaCMDMetaTable[] = {
	{ "__index", L_CMD___index },
	{ "__newindex", L_CMD___newindex },
	{ 0, 0 }
};