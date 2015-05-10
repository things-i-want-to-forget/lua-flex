#include "lua.hpp"
#include "../classes/usercmd.h"
#include "../classes/angle.h"
#include "../classes/vector.h"
#include <string.h>
#pragma warning(disable : 4244)

extern bool bSendPacket;

int L_CMD___index(lua_State *L)
{
	CUserCmd *cmd = &Get<CUserCmd>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (!strcmp(str, "command_number")) 
	{
		lua_pushnumber(L, cmd->command_number);
	}
	else if (!strcmp(str, "sendpacket"))
	{
		lua_pushboolean(L, bSendPacket);
	}
	else if (!strcmp(str, "angles")) 
	{
		LPush(L, cmd->angles, "Angle");
	}
	else if (!strcmp(str, "buttons")) 
	{
		lua_pushnumber(L, cmd->buttons());
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
	else if (!strcmp(str, "movement")) 
	{
		LPush(L, cmd->movement(), "Vector");
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
	CUserCmd *cmd = &Get<CUserCmd>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (!strcmp(str, "angles")) {
		cmd->angles = Get<QAngle>(L, 3);
	}
	else if (!strcmp(str, "buttons")) {
		cmd->buttons() = lua_tonumber(L, 3);
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
	else if (!strcmp(str, "movement")) {
		cmd->movement() = Get<Vector>(L, 3);
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