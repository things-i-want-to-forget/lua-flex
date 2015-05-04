#include "engines.h"
#include <Windows.h>
#include <string>

int GetAppID(void)
{
	char file_name[MAX_PATH];

	DWORD size = GetModuleFileNameA(NULL, file_name, MAX_PATH);

	while (size--)
	{
		if (*(file_name + size) == '/' || *(file_name + size) == '\\')
		{
			*(file_name + size) = 0;
			break;
		}
	}

	sprintf_s(file_name, "%s/steam_appid.txt", file_name);

	int number = -1;
	FILE *appid;
	fopen_s(&appid, file_name, "rb");
	if (appid != 0)
	{
		int amount = fscanf_s(appid, "%i", &number);
		fclose(appid);
		if (amount == 0) return -1;
	}
	return number;
}


EngineVersion GetEngineVersion(int appid)
{
	switch (appid)
	{
	case 730:	// csgo
		return CSGO;
	case 4000:	// gmod
		return GARRYSMOD;
	default:
		return EngineVersion(-1);
	}
}