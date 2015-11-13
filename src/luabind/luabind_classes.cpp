#include "classes/clientclass.h"
#include "classes/engineclient.h"
#include "classes/structures.h"
#include "lau/lau.h"

ClientClass *GetClientClass(lua_State *L, int where, bool checkvalid = true)
{
	
	ClientClass *ret = Get<ClientClass *>(L, "ClientClass", where);

	if (checkvalid && ret == 0)
		luaL_error(L, "Invalid ClientClass!");

	return ret;

}

int L_ClientClass_GetName(lua_State *L)
{

	lua_pushstring(L, GetClientClass(L, 1)->name);

	return 1;

}

int L_ClientClass_GetID(lua_State *L)
{

	lua_pushinteger(L, GetClientClass(L, 1)->classid);

	return 1;

}

int L_ClientClass_Next(lua_State *L)
{

	if (GetClientClass(L, 1)->next)
		LPush<ClientClass *>(L, GetClientClass(L, 1)->next, "ClientClass");
	else
		lua_pushnil(L);

	return 1;

}

int L_ClientClass___index(lua_State *L)
{

	lua_getmetatable(L, 1);

	lua_pushvalue(L, 2);

	lua_rawget(L, -2);

	lua_remove(L, -2);

	return 1;

}

luaL_Reg LuaClientClassMetaTable[] = {
	{ "GetID", &L_ClientClass_GetID },
	{ "GetName", &L_ClientClass_GetName },
	{ "Next", &L_ClientClass_Next },
	{ "__index", &L_ClientClass___index },
	{ 0, 0 }
};