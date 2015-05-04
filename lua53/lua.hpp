// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

template<typename t>
inline t &Get(lua_State *L, int where = -1)
{
	return *(t*)lua_touserdata(L, where);
}

template<typename t>
inline void Push(lua_State *L, t what)
{
	*(t *)(lua_newuserdata(L, sizeof(what))) = what;
}

