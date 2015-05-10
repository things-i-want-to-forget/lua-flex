#ifndef GLOBALS_H
#define GLOBALS_H

#include "engines.h"

extern EngineVersion version;

class CGlobalVarsBase
{
public:
	float realtime; // 00
	int framecount; // 04
	float absframetime; // 08
	// some float here in csgo // 0c

	inline float &curtime()
	{
		return *(float *)((version == CSGO ? 0x10 : 0x0C) + (char *)(this));
	}
	inline float &tickinterval(void)
	{
		return *(float *)((version == CSGO ? 0x20 : 0x1C) + (char *)(this));
	}
	inline float &frametime(void)
	{
		return *(float *)((version == CSGO ? 0x14 : 0x10) + (char *)(this));
	}
	float curtime_dontuse; // 10
	float frametime_dontuse; // 14
	int maxclients_dontuse; // 18
	int tickcount_dontuse; // 1c
	int tickinterval_dontuse; // 20
	float interpolation_dontuse;

	inline int &maxclients()
	{
		return *(int *)((version == CSGO ? 0x18 : 0x14) + (char *)(this));
	}
};

#endif