#include "lau.h"
#include "lua.hpp"
#include <io.h>
#include <Windows.h>
#include "classes/color.h"

int L_pcallerrenhance(lua_State *L)
{

	luaL_traceback(L, L, lua_tostring(L, -1), 2);

	return 1;

}

Lau::Lau(void)
{

	luadir = "C:\\aim-flex\\lua\\";

	L = luaL_newstate();

	funcstack = lua_gettop(L);

	lua_pushcfunction(L, &L_pcallerrenhance);

}

//testing purposes
#define _(x) 

long Lau::ReadLuaFile(const char *relpath, char **output)
{
	char temp[MAX_PATH + 1];
	sprintf_s(temp, "%s%s", luadir, relpath);
	_(temp);

	FILE *file;
	errno_t code;
	if ((code = fopen_s(&file, temp, "rb")) != 0)
		return 0;
	_("b");
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	_("c");
	*output = new char[length];

	fread(*output, 1, length, file);

	fclose(file);
	_("d");

	return length;
}

bool Lau::RunLuaFile(const char *relpath, bool safe)
{

	char *contents;
	long length;
	int code;
	bool ret = false;

	if ((length = ReadLuaFile(relpath, &contents)) != 0)
	{

		code = luaL_loadbuffer(L, contents, length, relpath);
		delete[] contents;
		if (code == 0)
		{
			if (safe)
			{

				const char *str = SafeCall(0, 1);
				if (str)
					lua_pushstring(L, str);
				else
					ret = true;

			}
			else
				lua_call(L, 0, 1), ret = true;


		}

	}
	else
		lua_pushstring(L, "File or file contents didn't exist!");

	return ret;

}


const char *Lau::SafeCall(int args, int rets)
{
	if (lua_pcall(L, args, rets, funcstack - lua_gettop(L)) == 0)
		return 0;
	const char *err = lua_tostring(L, -1);
	lua_pop(L, 1);

	return err;
}

bool Lau::PushHookCall(void)
{

	bool ret = true;

	lua_pushglobaltable(L);
	lua_getfield(L, -1, "hook");

	ret = lua_type(L, -1) == LUA_TTABLE; // check for TABLE type
	if (!ret)
	{
		lua_pop(L, 2);
		return false;
	}

	lua_getfield(L, -1, "Call");
	lua_remove(L, -2); // pop hook table
	lua_remove(L, -2); // pop global table

	ret = lua_type(L, -1) == LUA_TFUNCTION;

	if (!ret)
		lua_pop(L, 1);

	return ret;

}

extern luaL_Reg LuaAngleMetaTable[];
extern luaL_Reg LuaVectorMetaTable[];
extern luaL_Reg LuaEntityMetaTable[];
extern luaL_Reg LuaClientClassMetaTable[];

extern luaL_Reg LuaCMDMetaTable[];

extern luaL_Reg GlobalLibrary[];
extern luaL_Reg PredictionLibrary[];
extern luaL_Reg SurfaceLibrary[];
extern luaL_Reg PlayerLibrary[];
extern luaL_Reg UtilLibrary[];
extern luaL_Reg TraceLibrary[];
extern luaL_Reg EngineLibrary[];

void Lau::Init(void)
{
	luaL_openlibs(L);

	luaL_newmetatable(L, "Angle");
	{
		luaL_setfuncs(L, LuaAngleMetaTable, 0);
	}
	lua_pop(L, 1);

	luaL_newmetatable(L, "Vector");
	{
		luaL_setfuncs(L, LuaVectorMetaTable, 0);
	}
	lua_pop(L, 1);

	luaL_newmetatable(L, "Entity");
	{
		luaL_setfuncs(L, LuaEntityMetaTable, 0);
	}
	lua_pop(L, 1);

	luaL_newmetatable(L, "CUserCmd");
	{
		luaL_setfuncs(L, LuaCMDMetaTable, 0);
	}
	lua_pop(L, 1);

	luaL_newmetatable(L, "ClientClass");
	{
		luaL_setfuncs(L, LuaClientClassMetaTable, 0);
	}
	lua_pop(L, 1);
	

	lua_pushglobaltable(L);
	{
		luaL_setfuncs(L, GlobalLibrary, 0);

		lua_newtable(L);
		{
			luaL_setfuncs(L, SurfaceLibrary, 0);
		}
		lua_setfield(L, -2, "surface");

		lua_newtable(L);
		{
			luaL_setfuncs(L, EngineLibrary, 0);
		}
		lua_setfield(L, -2, "engine");

		lua_newtable(L);
		{
			luaL_setfuncs(L, PlayerLibrary, 0);
		}
		lua_setfield(L, -2, "player");

		lua_newtable(L);
		{
			luaL_setfuncs(L, TraceLibrary, 0);
		}
		lua_setfield(L, -2, "trace");

		lua_newtable(L);
		{
			luaL_setfuncs(L, PredictionLibrary, 0);
		}
		lua_setfield(L, -2, "predict");

		lua_newtable(L);
		{
			luaL_setfuncs(L, UtilLibrary, 0);
		}
		lua_setfield(L, -2, "util");
	}
	lua_pop(L, 1);

	if (!RunLuaFile("init.lua", true))
	{
		MessageBoxA(0, lua_tostring(L, -1), "ERROR", 0);
	}
	lua_pop(L, 1);

}