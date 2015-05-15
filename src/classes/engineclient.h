#ifndef ENGINECLIENT_H
#define ENGINECLIENT_H

#include "engines.h"

extern EngineVersion version;
class VMatrix;

class CNetChannel
{
	template <typename t>
	t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:
	bool SendFile(const char *filename, unsigned int transferID = 0xFFDDEEFF)
	{
		return get<bool(__thiscall *)(void *, const char *, unsigned int)>(42)(this, filename, transferID);
	}
};

struct player_info_t
{
	// scoreboard information
	char data[512];
	inline const char *GetName(void)
	{
		char *base = (char *)this;
		if (version == CSGO)
			return base + 0x10;
		return base;
	}
};

class CEngineClient
{
	template <typename t>
	t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:
	void GetScreenSize(int *w, int *h)
	{
		return get<void(__thiscall *)(void *, int *, int *)>(5)(this, w, h);
	}
	int GetLocalPlayer(void)
	{
		return get<int(__thiscall *)(void *)>(12)(this);
	}

	bool GetPlayerInfo(int index, player_info_t *info)
	{
		return get<bool(__thiscall *)(void *, int, player_info_t *)>(8)(this, index, info);
	}

	bool IsInGame(void)
	{
		return get<bool(__thiscall *)(void *)>(26)(this);
	}

	VMatrix& WorldToScreenMatrix(void)
	{
		return get< VMatrix &(__thiscall *)(void *)>(version == CSGO ? 37 : 36)(this);
	}

	CNetChannel *GetNetChannel(void)
	{
		return get<CNetChannel *(__thiscall *)(void *)>(78)(this);
	}
};

#endif // ENGINECLIENT_H