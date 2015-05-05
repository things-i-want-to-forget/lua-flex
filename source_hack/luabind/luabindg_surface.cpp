#include "lua.hpp"
#include "../structures.h"
#include "../matsystemsurface.h"

#include "../color.h"
#include <Windows.h>
#include <string>

#pragma warning(disable : 4244)

inline Color GetColor(lua_State *state, int where = -1)
{
	Color ret(0, 0, 0, 255);
	if (lua_type(state, where) == LUA_TNUMBER)
	{
		ret.r = (unsigned char)lua_tointeger(state, where);
		ret.g = (unsigned char)lua_tointeger(state, where + 1);
		ret.b = (unsigned char)lua_tointeger(state, where + 2);
		if (lua_type(state, where + 3) == LUA_TNUMBER)
			ret.a = (unsigned char)lua_tointeger(state, where + 3);
	}
	else
	{
		lua_getfield(state, where, "r");
		ret.r = (unsigned char)lua_tointeger(state, -1);
		lua_pop(state, 1);
		lua_getfield(state, where, "g");
		ret.g = (unsigned char)lua_tointeger(state, -1);
		lua_pop(state, 1);
		lua_getfield(state, where, "b");
		ret.b = (unsigned char)lua_tointeger(state, -1);
		lua_pop(state, 1);
		lua_getfield(state, where, "a");
		ret.a = (unsigned char)lua_tointeger(state, -1);
		lua_pop(state, 1);
	}
	return ret;
}


int L_surface_SetDrawColor(lua_State *L)
{
	structs.surface->DrawSetColor(GetColor(L, 1));
	return 0;
}
int L_surface_DrawRect(lua_State *L)
{
	structs.surface->DrawFilledRect(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4));
	return 0;
}
int L_surface_DrawOutlinedRect(lua_State *L)
{
	structs.surface->DrawOutlinedRect(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4));
	return 0;
}
int L_surface_CreateFont(lua_State *L)
{
#ifdef CreateFont
#undef CreateFont
#endif
	lua_pushinteger(L, structs.surface->CreateFont());
	return 1;
}
int L_surface_SetFontGlyph(lua_State *L)
{
	structs.surface->SetFontGlyphSet(lua_tointeger(L, 1), lua_tostring(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), 0, 0, lua_tointeger(L, 5));
	return 0;
}

int L_surface_SetFont(lua_State *L)
{
	structs.surface->DrawSetTextFont(lua_tointeger(L, 1));
	return 0;
}

int L_surface_SetTextPos(lua_State *L)
{
	structs.surface->DrawSetTextPos(lua_tointeger(L, 1), lua_tointeger(L, 2));
	return 0;
}

int L_surface_DrawText(lua_State *L)
{
	const char *str = lua_tostring(L, 1);
	wchar_t *temp = new wchar_t[strlen(str)];
	int len = wsprintfW(temp, L"%S", str);
	structs.surface->DrawPrintText(temp, len);
	delete[] temp;
	return 0;
}

luaL_Reg SurfaceLibrary[] = {
	{ "SetFontGlyph", L_surface_SetFontGlyph },
	{ "DrawOutlinedRect", L_surface_DrawOutlinedRect },
	{ "CreateFont", L_surface_CreateFont },
	{ "DrawRect", L_surface_DrawRect },
	{ "SetDrawColor", L_surface_SetDrawColor },
	{ "SetTextPos", L_surface_SetTextPos },
	{ "DrawText", L_surface_DrawText },
	{ "SetFont", L_surface_SetFont },
	{ 0, 0 }
};