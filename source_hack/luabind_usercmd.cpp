#include "lua.hpp"
#include "usercmd.h"
#include "angle.h"
#include "vector.h"
#pragma warning(disable : 4244)

int L_CMD___index(lua_State *L)
{
	CUserCmd *cmd = &Get<CUserCmd>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (str == "command_number") {
		lua_pushnumber(L, cmd->command_number);
	}
	else if (str == "command_number") {
		lua_pushnumber(L, cmd->tickcount);
	}
	else if (str == "angles") {
		LPush(L, cmd->angles, "Angle");
	}
	else if (str == "buttons") {
		lua_pushnumber(L, cmd->buttons());
	}
	else if (str == "mousex") {
		lua_pushnumber(L, cmd->mousex());
	}
	else if (str == "mousey") {
		lua_pushnumber(L, cmd->mousey());
	}
	else if (str == "sidemove") {
		lua_pushnumber(L, cmd->sidemove());
	}
	else if (str == "forwardmove") {
		lua_pushnumber(L, cmd->forwardmove());
	}
	else if (str == "movement") {
		LPush(L, cmd->movement(), "Vector");
	} 
	else {
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
	if (str == "command_number") {
		cmd->command_number = lua_tonumber(L, 3);
	}
	else if (str == "command_number") {
		cmd->tickcount = lua_tonumber(L, 3);
	}
	else if (str == "angles") {
		cmd->angles = Get<QAngle>(L, 3);
	}
	return 1;
}

luaL_Reg LuaCMDMetaTable[] = {
	{ "__index", L_CMD___index },
	{ "__newindex", L_CMD___newindex },
	{ 0, 0 }
};