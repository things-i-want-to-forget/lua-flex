#include "lau.h"
#include "lua.hpp"
#include <io.h>
#include <Windows.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

Lau::Lau(void)
{
	if (!luadir)
	{
		luadir = new char[MAX_PATH + 1];

		DWORD length = GetModuleFileNameA((HMODULE)&__ImageBase, luadir, sizeof(MAX_PATH));


		while (length-- && *(luadir + length) != '/' && *(luadir + length) != '\\')
			*(luadir + length) = 0;

		strcat_s(luadir, MAX_PATH + 1, "lua/");

	}
	L = luaL_newstate();

}

long Lau::ReadLuaFile(const char *relpath, char **output)
{
	char temp[MAX_PATH + 1];
	sprintf_s(temp, "%s%s", luadir, relpath);

	FILE *file;
	errno_t code;
	if ((code = fopen_s(&file, temp, "rb")) == 0)
		return code;
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	*output = new char[length];

	fread(*output, 1, length, file);

	fclose(file);

	return length;
}

int Lau::LoadBuffer(const char *contents, int content_length, const char *name)
{

	return luaL_loadbuffer(GetState(), contents, content_length, name);

}

int Lau::RunLuaFile(const char *relpath)
{

	int top;
	char *contents;
	long length;
	int code;

	top = lua_gettop(L);

	if ((length = ReadLuaFile(relpath, &contents)) != 0)
	{
		code = LoadBuffer(contents, length, relpath);
		delete[] contents;
		if (code == 0)
		{
			lua_call(L, 0, LUA_MULTRET);
			return lua_gettop(L) - top;
		}

		lua_error(L);

		return -1;
	}

	return -1;

}

extern luaL_Reg LuaAngleMetaTable[];
extern luaL_Reg LuaVectorMetaTable[];

void Lau::Init(void)
{

	lua_newtable(L);
	{
		lua_newtable(L); // value
		{
			luaL_setfuncs(L, LuaAngleMetaTable, 0);
		}
		lua_setfield(L, -2, "Angle");

		lua_newtable(L); // value
		{
			luaL_setfuncs(L, LuaVectorMetaTable, 0);
		}
		lua_setfield(L, -2, "Vector");
	}
	metatables = luaL_ref(L, LUA_REGISTRYINDEX);

	RunLuaFile("init.lua");

}