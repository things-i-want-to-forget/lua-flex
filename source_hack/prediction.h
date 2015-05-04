#ifndef PREDICTION_H
#define PREDICTION_H

#include "engines.h"

class QAngle;
class CUserCmd;
class ClientEntity;

extern EngineVersion version;

class CPrediction
{
	template <typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:

	void SetLocalViewAngles(QAngle &ang)
	{
		get<void(__thiscall *)(void *, QAngle &)>(12)(this, ang);
	}
	bool InPrediction(void)
	{
		return get<bool(__thiscall *)(void *)>(14)(this);
	}
	void SetupMove(ClientEntity *ent, CUserCmd *cmd, void *movehelper, void *movedata)
	{
		return get<void(__thiscall *)(void *, ClientEntity *, CUserCmd *, void *, void *)>
			(version == CSGO ? 20 : 18)(this, ent, cmd, movehelper, movedata);
	}
	void FinishMove(ClientEntity *ent, CUserCmd *cmd, void *movedata)
	{
		return get<void(__thiscall *)(void *, ClientEntity *, CUserCmd *, void *)>
			(version == CSGO ? 21 : 19)(this, ent, cmd, movedata);
	}
};

#endif // PREDICTION_H