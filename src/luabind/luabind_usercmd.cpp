#include "lua.hpp"
#include "../classes/usercmd.h"
#include "../classes/angle.h"
#include "../classes/vector.h"
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
	else if (str == "buttons") {
		cmd->buttons() = lua_tonumber(L, 3);
	}
	else if (str == "mousex") {
		cmd->mousex() = lua_tonumber(L, 3);
	}
	else if (str == "mousey") {
		cmd->mousey() = lua_tonumber(L, 3);
	}
	else if (str == "sidemove") {
		cmd->sidemove() = lua_tonumber(L, 3);
	}
	else if (str == "forwardmove") {
		cmd->forwardmove() = lua_tonumber(L, 3);
	}
	else if (str == "movement") {
		cmd->movement() = Get<Vector>(L, 3);
	}
	return 0;
}

luaL_Reg LuaCMDMetaTable[] = {
	{ "__index", L_CMD___index },
	{ "__newindex", L_CMD___newindex },
	{ 0, 0 }
};