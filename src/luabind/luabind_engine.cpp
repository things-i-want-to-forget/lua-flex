#include "lau/lau.h"
#include "classes/structures.h"
#include "classes/client.h"
#include "classes/clientclass.h"


int L_engine_GetAllClasses(lua_State *L)
{

	LPush<ClientClass *>(L, structs.client->GetAllClasses(), "ClientClass");

	return 1;

}

luaL_Reg EngineLibrary[] = {
	{ "GetAllClasses", &L_engine_GetAllClasses },
	{ 0, 0 }
};