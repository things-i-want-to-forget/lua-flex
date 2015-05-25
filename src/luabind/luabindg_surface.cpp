#include "lua.hpp"
#include "../classes/structures.h"
#include "../classes/matsystemsurface.h"

#include "../classes/color.h"
#include <Windows.h>
#include <string>

#pragma warning(disable : 4996)

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
	structs.surface->DrawFilledRect(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	return 0;
}
int L_surface_DrawOutlinedRect(lua_State *L)
{
	structs.surface->DrawOutlinedRect(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
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
	structs.surface->DrawSetTextPos(lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}
int L_surface_SetTextColor(lua_State *L)
{
	Color c = GetColor(L, 1);
	structs.surface->DrawSetTextColor(c);
	return 0;
}

static wchar_t *chrtowc(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}

int L_surface_DrawText(lua_State *L)
{
	wchar_t *temp = chrtowc(lua_tostring(L, 1));
	structs.surface->DrawPrintText(temp, lstrlenW(temp));
	delete[] temp;
	return 0;
}

int L_surface_DrawLine(lua_State *L)
{
	structs.surface->DrawLine(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	return 0;
}

int L_surface_GetTextSize(lua_State *L)
{
	int w, h;
	wchar_t *text = chrtowc(lua_tostring(L, 2));
	structs.surface->DrawGetTextSize(lua_tointeger(L, 1), text, w, h);

	delete[] text;

	lua_pushinteger(L, w);
	lua_pushinteger(L, h);

	return 2;
}



luaL_Reg SurfaceLibrary[] = {
	{ "DrawOutlinedRect", L_surface_DrawOutlinedRect },
	{ "SetTextColor", L_surface_SetTextColor },
	{ "SetDrawColor", L_surface_SetDrawColor },
	{ "SetFontGlyph", L_surface_SetFontGlyph },
	{ "SetTextPos", L_surface_SetTextPos },
	{ "CreateFont", L_surface_CreateFont },
	{ "DrawText", L_surface_DrawText },
	{ "DrawRect", L_surface_DrawRect },
	{ "SetFont", L_surface_SetFont },
	{ "DrawLine", L_surface_DrawLine },
	{ "GetTextSize", L_surface_GetTextSize },
	{ 0, 0 }
};
