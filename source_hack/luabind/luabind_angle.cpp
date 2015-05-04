#include "lua.hpp"
#include "../angle.h"
#pragma warning(disable : 4244)

int L_Angle___index(lua_State *L)
{
	QAngle *angle = (QAngle *)lua_touserdata(L, 1);

	if (lua_type(L, 2) == LUA_TNUMBER)
	{
		lua_Integer which = lua_tointeger(L, 2);
		lua_pushnumber(L, (lua_Number)(*angle)[which]);
		return 1;
	}
	else if (lua_type(L, 2) == LUA_TSTRING)
	{
		const char *str = lua_tostring(L, 2);
		switch (*str)
		{
		default:
		case 'p':
			lua_pushnumber(L, angle->p);
			break;
		case 'y':
			lua_pushnumber(L, angle->y);
			break;
		case 'r':
			lua_pushnumber(L, angle->r);
			break;
		}
		return 1;
	}

	lua_pushstring(L, "unsupported type in Angle:__index");
	lua_error(L);
	return 0;
}

int L_Angle___newindex(lua_State *L)
{
	QAngle *angle = (QAngle *)lua_touserdata(L, 1);
	lua_Number value = lua_tonumber(L, 3);

	if (lua_type(L, 2) == LUA_TNUMBER)
	{
		lua_Integer which = lua_tointeger(L, 2);
		(*angle)[which] = value;
		return 1;
	}
	else if (lua_type(L, 2) == LUA_TSTRING)
	{
		const char *str = lua_tostring(L, 2);
		switch (*str)
		{
		default:
		case 'p':
			angle->p = value;
			break;
		case 'y':
			angle->y = value;
			break;
		case 'r':
			angle->r = value;
			break;
		}
		return 1;
	}


	return 0;

}

int L_Angle__IsZero(lua_State *L)
{
	lua_pushboolean(L, ((QAngle *)lua_touserdata(L, 1))->IsZero());
	return 1;
}
int L_Angle__Zero(lua_State *L)
{
	((QAngle *)lua_touserdata(L, 1))->Zero();
	return 1;
}
luaL_Reg LuaAngleMetaTable[] = {
	{ "__index", L_Angle___index },
	{ "__newindex", L_Angle___newindex },
	{ "IsZero", L_Angle__IsZero },
	{ "Zero", L_Angle__Zero },
	{ 0, 0 }
};