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

int L_ClientClass_GetRecvTable(lua_State *L)
{

	LPush(L, GetClientClass(L, 1)->table, "RecvTable");

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
	{ "GetRecvTable", &L_ClientClass_GetRecvTable },
	{ "__index", &L_ClientClass___index },
	{ 0, 0 }
};



RecvTable *GetRecvTable(lua_State *L, int where, bool checkvalid = true)
{

	RecvTable *ret = Get<RecvTable *>(L, "RecvTable", where);

	if (checkvalid && ret == 0)
		luaL_error(L, "Invalid RecvTable!");

	return ret;

}

int L_RecvTable_GetPropertyCount(lua_State *L)
{

	lua_pushinteger(L, GetRecvTable(L, 1)->nprops);

	return 1;

}

int L_RecvTable_GetProperty(lua_State *L)
{

	LPush(L, &GetRecvTable(L, 1)->props[luaL_checkinteger(L, 2)], "RecvProp");

	return 1;

}

int L_RecvTable_GetName(lua_State *L)
{

	lua_pushstring(L, GetRecvTable(L, 1)->name);

	return 1;

}

int L_RecvTable___index(lua_State *L)
{

	lua_getmetatable(L, 1);

	lua_pushvalue(L, 2);

	lua_rawget(L, -2);

	lua_remove(L, -2);

	return 1;

}


luaL_Reg LuaRecvTableMetaTable[] = {
	{ "GetPropertyCount", &L_RecvTable_GetPropertyCount },
	{ "GetProperty", &L_RecvTable_GetProperty },
	{ "GetName", &L_RecvTable_GetName },
	{ "__index", &L_RecvTable___index },
	{ 0, 0 }
};


RecvProp *GetRecvProp(lua_State *L, int where, bool checkvalid = true)
{

	RecvProp *ret = Get<RecvProp *>(L, "RecvProp", where);

	if (checkvalid && ret == 0)
		luaL_error(L, "Invalid RecvProp!");

	return ret;

}


int L_RecvProp_GetName(lua_State *L)
{

	lua_pushstring(L, GetRecvProp(L, 1)->name);

	return 1;

}

int L_RecvProp_GetDatatable(lua_State *L)
{

	if (GetRecvProp(L, 1)->datatable)
		LPush(L, GetRecvProp(L, 1)->datatable, "RecvTable");

	else
		return 0;

	return 1;

}

int L_RecvProp_GetOffset(lua_State *L)
{

	lua_pushinteger(L, GetRecvProp(L, 1)->offset);

	return 1;

}

int L_RecvProp_GetType(lua_State *L)
{

	lua_pushinteger(L, GetRecvProp(L, 1)->type);

	return 1;

}

int L_RecvProp___index(lua_State *L)
{

	lua_getmetatable(L, 1);

	lua_pushvalue(L, 2);

	lua_rawget(L, -2);

	lua_remove(L, -2);

	return 1;

}

luaL_Reg LuaRecvPropMetaTable[] = {
	{ "GetType", &L_RecvProp_GetType },
	{ "GetOffset", &L_RecvProp_GetOffset },
	{ "GetName", &L_RecvProp_GetName },
	{ "GetDatatable", &L_RecvProp_GetDatatable },
	{ "__index", &L_RecvProp___index },
	{ 0, 0 }
};
