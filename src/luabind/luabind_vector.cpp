#include "lau/lau.h"
#include "classes/vector.h"
#include "classes/angle.h"
#pragma warning(disable : 4244)

Vector &GetVector(lua_State *L, int where = -1)
{

	return Get<Vector>(L, "Vector", where);

}

int L_Vector___index(lua_State *L) {
	Vector &vector = GetVector(L, 1);

	const char *str = lua_tostring(L, 2);

	if (*str && *(str + 1) == 0)

		switch (*str) 
		{
			default:
			case 'x':
				lua_pushnumber(L, vector.x);
				break;
			case 'y':
				lua_pushnumber(L, vector.y);
				break;
			case 'z':
				lua_pushnumber(L, vector.z);
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

int L_Vector_Transform(lua_State *L)
{

	LPush(L, GetVector(L, 1).Transform(Get<matrix3x4_t>(L, "Matrix", 2)), "Vector");

	return 1;

}

int L_Vector___newindex(lua_State *L)
{
	Vector &vector = GetVector(L, 1);

	if (lua_type(L, 2) == LUA_TNUMBER) 
	{

		lua_Integer which = lua_tointeger(L, 2);

		if (which > 2 || which < 0) 
			luaL_error(L, "Invalid index passed to Vector:__newindex\n");

		lua_pushnumber(L, (lua_Number)vector[which]);
		return 1;

	} 
	else if (lua_type(L, 2) == LUA_TSTRING)
	{

		const char *str = lua_tostring(L, 2);

		switch (*str) 
		{
			default:
				luaL_error(L, "Invalid index passed to Vector:__newindex");

			case 'p':
			case 'x':
				lua_pushnumber(L, vector.x);
				break;

			case 'y':
				lua_pushnumber(L, vector.y);
				break;

			case 'r':
			case 'z':
				lua_pushnumber(L, vector.z);
				break;

		}

		return 1;

	}

	return 0;

}

int L_Vector__add(lua_State *L) 
{

	Vector &vector = GetVector(L, 1);
	Vector &vector2 = GetVector(L, 2);

	LPush(L, vector + vector2, "Vector");
	return 1;

}

int L_Vector__sub(lua_State *L)
{

	Vector &vector = GetVector(L, 1);
	Vector &vector2 = GetVector(L, 2);

	LPush(L, vector - vector2, "Vector");
	return 1;

}

int L_Vector__mul(lua_State *L)
{

	Vector &vector = GetVector(L, 1);

	LPush(L, vector * lua_tonumber(L, 2), "Vector");
	return 1;

}

int L_Vector__div (lua_State *L)
{

	LPush(L, GetVector(L, 1) / lua_tonumber(L, 2), "Vector");

	return 1;

}

int L_Vector__eq(lua_State *L)
{

	lua_pushboolean(L, GetVector(L, 1) == GetVector(L, 2));

	return 1;

}

int L_Vector_ToScreen(lua_State *L)
{

	Vector &vector = GetVector(L, 1);
	Vector newVector = Vector();

	bool onscreen = vector.ToScreen(newVector);

	LPush(L, newVector, "Vector");
	lua_pushboolean(L, onscreen);
	return 2;

}

int L_Vector_GetNormalized(lua_State *L) 
{

	LPush(L, GetVector(L, 1).GetNormalized(), "Vector");

	return 1;

}

int L_Vector_Normalize(lua_State *L) 
{

	GetVector(L, 1).Normalize();

	return 1;

}

int L_Vector_Dot(lua_State *L) 
{

	Vector &vector = GetVector(L, 1);
	Vector &vector2 = GetVector(L, 2);

	lua_pushnumber(L, vector.Dot(vector2));

	return 1;

}

int L_Vector_Rotate(lua_State *L) 
{

	QAngle &GetAngle(lua_State *L, int where = -1);

	Vector &vector = GetVector(L, 1);
	QAngle &angle = GetAngle(L, 2);

	vector.Rotate(angle);
	lua_pushvalue(L, 1);
	return 1;

}

int L_Vector_Angle(lua_State *L)
{

	Vector &v = GetVector(L, 1);

	QAngle a(0,0,0);

	LPush(L, v.Angle(a), "Angle");
	return 1;

}

int L_Vector_LengthSqr(lua_State *L) 
{

	Vector &vector = GetVector(L, 1);

	lua_pushnumber(L, vector.LengthSqr());
	return 1;

}

int L_Vector_Length(lua_State *L) 
{

	Vector &vector = GetVector(L, 1);

	lua_pushnumber(L, vector.Length());
	return 1;

}

int L_Vector_Distance(lua_State *L) 
{

	lua_pushnumber(L, GetVector(L, 1).Distance(GetVector(L, 2)));

	return 1;

}

luaL_Reg LuaVectorMetaTable[] = {
	{ "__index", L_Vector___index },
	{ "__newindex", L_Vector___newindex },
	{ "__add", L_Vector__add },
	{ "__sub", L_Vector__sub },
	{ "__mul", L_Vector__mul },
	{ "__div", L_Vector__div },
	{ "__eq", L_Vector__eq },
	{ "ToScreen", L_Vector_ToScreen },
	{ "GetNormalized", L_Vector_GetNormalized },
	{ "Transform", L_Vector_Transform },
	{ "Normalize", L_Vector_Normalize },
	{ "Dot", L_Vector_Dot },
	{ "Rotate", L_Vector_Rotate },
	{ "Angle", L_Vector_Angle },
	{ "LengthSqr", L_Vector_LengthSqr },
	{ "Length", L_Vector_Length },
	{ "Distance", L_Vector_Distance },
	{ 0, 0 }
};
