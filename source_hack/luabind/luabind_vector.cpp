#include "lua.hpp"
#include "../vector.h"
#pragma warning(disable : 4244)

int L_Vector___index(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);

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
	}
	return 1;
}

int L_Vector___newindex(lua_State *L) {
	Vector *vector = &Get<Vector>(L, 1);

	if (lua_type(L, 2) == LUA_TNUMBER) {
		lua_Integer which = lua_tointeger(L, 2);
		lua_pushnumber(L, ( lua_Number ) ( *vector )[which]);
		return 1;
	} else if (lua_type(L, 2) == LUA_TSTRING) {
		const char *str = lua_tostring(L, 2);
		switch (*str) {
			default:
			case 'p':
				lua_pushnumber(L, vector->x);
				break;
			case 'y':
				lua_pushnumber(L, vector->y);
				break;
			case 'r':
				lua_pushnumber(L, vector->z);
				break;
		}
		return 1;
	}


	return 0;

}

int L_Vector__add(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);
	Vector &vector2 = Get<Vector>(L, 2);

	LPush(L, vector + vector2, "Vector");
	return 1;
}

int L_Vector__sub(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);
	Vector &vector2 = Get<Vector>(L, 2);

	LPush(L, vector - vector2, "Vector");
	return 1;
}

int L_Vector__mul(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);

	LPush(L, vector * lua_tonumber(L, 2), "Vector");
	return 1;
}

int L_Vector__eq(lua_State *L)
{
	lua_pushboolean(L, Get<Vector>(L, 1) == Get<Vector>(L, 2));
	return 1;
}

int L_Vector_ToScreen(lua_State *L) {
	Vector vector = Get<Vector>(L, 1);
	Vector newVector = Vector();

	vector.ToScreen(newVector);

	LPush(L, newVector, "Vector");
	return 1;
}

int L_Vector_Normalize(lua_State *L) {
	Get<Vector>(L, 1).Normalize();
	return 1;
}

int L_Vector_Dot(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);
	Vector &vector2 = Get<Vector>(L, 2);

	vector.Dot(vector2);

	LPush(L, vector, "Vector");
	return 1;
}

int L_Vector_Rotate(lua_State *L) {
	Vector &vector = Get<Vector>(L, 1);
	QAngle &angle = Get<QAngle>(L, 1);

	vector.Rotate(angle);
	return 1;
}

int L_Vector_LengthSqr(lua_State *L) {
	Vector *vector = ( Vector * ) lua_touserdata(L, 1);

	lua_pushnumber(L, vector->LengthSqr());
	return 1;
}

int L_Vector_Length(lua_State *L) {
	Vector *vector = ( Vector * ) lua_touserdata(L, 1);

	lua_pushnumber(L, vector->Length());
	return 1;
}

luaL_Reg LuaVectorMetaTable[] = {
	{ "__index", L_Vector___index },
	{ "__newindex", L_Vector___newindex },
	{ "__add", L_Vector__add },
	{ "__sub", L_Vector__sub },
	{ "__mul", L_Vector__mul },
	{ "__eq", L_Vector__eq },
	{ "ToScreen", L_Vector_ToScreen },
	{ "Normalize", L_Vector_Normalize },
	{ "Dot", L_Vector_Dot },
	{ "Rotate", L_Vector_Rotate },
	{ "LengthSqr", L_Vector_LengthSqr },
	{ "Length", L_Vector_Length },
	{ 0, 0 }
};