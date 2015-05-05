// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef AIM_FLEX
#include "../source_hack/structures.h"
#include "../source_hack/lau/lau.h"

template<typename t>
inline t &Get(lua_State *L, int where = -1)
{
	return *(t*)lua_touserdata(L, where);
}


template<typename t>
inline void LPush(lua_State *L, t what, const char *meta)
{
	*(t *)(lua_newuserdata(L, sizeof(what))) = what;
	lua_rawgeti(L, LUA_REGISTRYINDEX, structs.L->MetatableIndex());
	lua_pushstring(L, meta);
	lua_rawget(L, -2);
	lua_setmetatable(L, -3);
	lua_pop(L, 1);
}

#endif