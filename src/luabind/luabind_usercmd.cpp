#include "lua.hpp"
#include "../classes/usercmd.h"
#include "../classes/angle.h"
#include "../classes/vector.h"
#include <string.h>
#pragma warning(disable : 4244)

extern bool bSendPacket;

int L_CMD___index(lua_State *L)
{
	CUserCmd *cmd = Get<CUserCmd *>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (!strcmp(str, "command_number")) 
	{
		lua_pushnumber(L, cmd->command_number);
	}
	else if (!strcmp(str, "angles")) // beWARE READONLY
	{
		LPush(L, cmd->angles, "Angle");
	}
	else if (!strcmp(str, "sendpacket"))
	{
		lua_pushboolean(L, bSendPacket);
	}
	else if (!strcmp(str, "p")) 
	{
		lua_pushnumber(L, cmd->angles.p);
	}
	else if (!strcmp(str, "y"))
	{
		lua_pushnumber(L, cmd->angles.y);
	}
	else if (!strcmp(str, "r"))
	{
		lua_pushnumber(L, cmd->angles.r);
	}
	else if (!strcmp(str, "buttons")) 
	{
		lua_pushinteger(L, cmd->buttons());
	}
	else if (!strcmp(str, "mousex")) 
	{
		lua_pushnumber(L, cmd->mousex());
	}
	else if (!strcmp(str, "mousey")) 
	{
		lua_pushnumber(L, cmd->mousey());
	}
	else if (!strcmp(str, "sidemove")) 
	{
		lua_pushnumber(L, cmd->sidemove());
	}
	else if (!strcmp(str, "forwardmove")) 
	{
		lua_pushnumber(L, cmd->forwardmove());
	}
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
	CUserCmd *cmd = Get<CUserCmd *>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (!strcmp(str, "angles")) {
		cmd->angles = Get<QAngle>(L, 3);
	}
	else if (!strcmp(str, "sendpacket"))
	{
		bSendPacket = lua_toboolean(L, 3) != 0;
	}
	else if (!strcmp(str, "p"))
	{
		cmd->angles.p = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "y"))
	{
		cmd->angles.y = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "r"))
	{
		cmd->angles.r = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "buttons")) {
		cmd->buttons() = lua_tointeger(L, 3);
	}
	else if (!strcmp(str, "mousex")) {
		cmd->mousex() = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "mousey")) {
		cmd->mousey() = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "sidemove")) {
		cmd->sidemove() = lua_tonumber(L, 3);
	}
	else if (!strcmp(str, "forwardmove")) {
		cmd->forwardmove() = lua_tonumber(L, 3);
	}
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