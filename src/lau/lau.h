#ifndef LAU_H
#define LAU_H

#include "lua.hpp"
#include "classes/structures.h"

struct lua_State;

class Lau
{
public:

	Lau(void);
	~Lau(void) { };

	lua_State *GetState(void)
	{
		return L;
	}

	const char *GetLuaDir(void)
	{
		return luadir;
	}

	long ReadLuaFile(const char *relpath, char **output);

	// returns the amount of returns from running the file
	// or -1 on error
	bool RunLuaFile(const char *relpath, bool safe = false);
	bool PushHookCall(void);

	const char *SafeCall(int args, int rets = 0);

	void Init(void);

private:
	lua_State *L;
	char *luadir;
	int funcstack;
};

template <typename t>
inline t &Get(lua_State *L, const char *name, int where = -1)
{

	return *(t *)luaL_checkudata(L, where, name);

}

template <typename t>
inline void LPush(lua_State *L, t what, const char *meta)
{

	*(t *)lua_newuserdata(L, sizeof(what)) = what;

	luaL_newmetatable(L, meta);
	lua_setmetatable(L, -2);

}

#endif