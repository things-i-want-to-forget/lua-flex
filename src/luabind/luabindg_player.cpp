#include "lau/lau.h"
#include "../classes/structures.h"
#include "../classes/clienttools.h"
#include "../classes/entities.h"

int L_player_GetAll(lua_State *L)
{
	lua_newtable(L);
	int amt = 1;
	for (int i = 1; i <= structs.globals->maxclients(); i++)
	{
		ClientEntity *e = structs.entity_list->GetClientEntity(i);
		if (e)
		{
			LPush<CBaseHandle>(L, e->GetRefEHandle(), "Entity");
			lua_rawseti(L, -2, amt++);
		}
	}
	return 1;
}

luaL_Reg PlayerLibrary[] = {
	{ "GetAll", L_player_GetAll },
	{ 0, 0 }
};