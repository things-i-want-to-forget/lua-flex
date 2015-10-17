#include "lua.hpp"
#include "../classes/structures.h"
#include "../classes/entities.h"
#include "../classes/engineclient.h"
#include "../classes/prediction.h"
#include "../classes/gamemovement.h"

#pragma warning(disable : 4244)

class CUserCmd;

int L_predict_Predict(lua_State *L)
{
	ClientEntity *GetEntity(lua_State *L, int where = -1);
	CUserCmd *&GetCUserCmd(lua_State *L, int where = -1);

	ClientEntity *ent;
	CUserCmd *cmd = GetCUserCmd(L, 1);
	float frametime = luaL_checknumber(L, 2);
	if (lua_type(L, 3) == LUA_TUSERDATA)
		ent = GetEntity(L, 3);
	else
		ent = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());



	static char movedata[1024];
	memset(movedata, 0, sizeof(movedata));

	float old_time = structs.globals->curtime();
	float old_frame = structs.globals->frametime();

	structs.globals->curtime() = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer())
		->getvar<int>("m_nTickBase") * structs.globals->tickinterval();
	structs.globals->frametime() = frametime;

	structs.prediction->SetupMove(ent, cmd, 0, movedata);
	structs.game_movement->ProcessMovement(ent, movedata);
	structs.prediction->FinishMove(ent, cmd, movedata);

	structs.globals->curtime()   = old_time;
	structs.globals->frametime() = old_frame;

	return 0;
}

luaL_Reg PredictionLibrary[] = {
	{ "Predict", L_predict_Predict },
	{ 0, 0 }
};