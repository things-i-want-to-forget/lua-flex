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
	CUserCmd *cmd = Get<CUserCmd *>(L, 1);
	float frametime = lua_tonumber(L, 2);

	static char movedata[1024];
	memset(movedata, 0, sizeof(movedata));
	ClientEntity *me =
		structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());
	float old_time = structs.globals->curtime();
	float old_frame = structs.globals->frametime();
	structs.globals->curtime() = me->getvar<int>("m_nTickBase") * structs.globals->tickinterval();
	structs.globals->frametime() = frametime;

	structs.prediction->SetupMove(me, cmd, 0, movedata);
	structs.game_movement->ProcessMovement(me, movedata);
	structs.prediction->FinishMove(me, cmd, movedata);

		structs.globals->curtime() = old_time;
	structs.globals->frametime() = old_frame;

	return 0;
}

luaL_Reg PredictionLibrary[] = {
	{ "Predict", L_predict_Predict },
	{ 0, 0 }
};