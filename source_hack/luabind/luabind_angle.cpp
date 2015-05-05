#include "lua.hpp"
#include "../angle.h"
#include "../vector.h"
#pragma warning(disable : 4244)

int L_Angle___index(lua_State *L)
{
	QAngle *angle = &Get<QAngle>(L, 1);

	const char *str = lua_tostring(L, 2);
	if (*str && *(str + 1) == 0)
		switch (*str)
		{
		default:
		case 'p':
		case 'x':
			lua_pushnumber(L, angle->p);
			break;
		case 'y':
			lua_pushnumber(L, angle->y);
			break;
		case 'r':
		case 'z':
			lua_pushnumber(L, angle->r);
			break;
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

int L_Angle___newindex(lua_State *L)
{
	QAngle *angle = &Get<QAngle>(L, 1);
	lua_Number value = lua_tonumber(L, 3);
	
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

int L_Angle_IsZero(lua_State *L)
{
	lua_pushboolean(L, Get<QAngle>(L, 1).IsZero());
	return 1;
}
int L_Angle_Zero(lua_State *L)
{
	Get<QAngle>(L, 1).Zero();
	return 1;
}

int L_Angle_Normalize(lua_State *L)
{
	Get<QAngle>(L, 1).Normalize();
	return 1;
}

int L_Angle_Max(lua_State *L)
{
	lua_pushnumber(L, Get<QAngle>(L, 1).Max());
	return 1;
}

int L_Angle_Vector(lua_State *L)
{
	Vector v;
	LPush(L, Get<QAngle>(L, 1).ToVector(v), "Angle");
	return 1;
}

int L_Angle_Difference(lua_State *L)
{
	LPush(L, Get<QAngle>(L, 1).Difference(Get<QAngle>(L, 2)), "Angle");
	return 1;
}

int L_Angle_Clamp(lua_State *L)
{
	Get<QAngle>(L, 1).Clamp();
	return 1;
}

int L_Angle___add(lua_State *L)
{
	LPush(L, Get<QAngle>(L, 1) + Get<QAngle>(L, 2), "Angle");
	return 1;
}

int L_Angle___sub(lua_State *L)
{
	LPush(L, Get<QAngle>(L, 1) - Get<QAngle>(L, 2), "Angle");
	return 1;
}

int L_Angle___div(lua_State *L)
{
	LPush(L, Get<QAngle>(L, 1) / lua_tonumber(L, 2), "Angle");
	return 1;
}

int L_Angle___mul(lua_State *L)
{
	LPush(L, Get<QAngle>(L, 1) * lua_tonumber(L, 2), "Angle");
	return 1;
}

int L_Angle___eq(lua_State *L)
{
	lua_pushboolean(L, Get<QAngle>(L, 1) == Get<QAngle>(L, 2));
	return 1;
}


luaL_Reg LuaAngleMetaTable[] = {
	{ "__index", L_Angle___index },
	{ "__newindex", L_Angle___newindex },
	{ "IsZero", L_Angle_IsZero },
	{ "Zero", L_Angle_Zero },
	{ "Normalize", L_Angle_Normalize },
	{ "Max", L_Angle_Max },
	{ "Vector", L_Angle_Vector },
	{ "Difference", L_Angle_Difference },
	{ "Clamp", L_Angle_Clamp },
	{ "__div", L_Angle___div },
	{ "__mul", L_Angle___mul },
	{ "__add", L_Angle___add },
	{ "__sub", L_Angle___sub },
	{ "__eq", L_Angle___eq },
	{ 0, 0 }
};