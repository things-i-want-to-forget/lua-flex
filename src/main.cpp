#define WIN32_LEAN_AND_MEAN
#define VS_EXTRA_LEAN
#include <Windows.h>
#include "init.h"


BOOL APIENTRY DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		
		csgolua::Init();

	}

	return TRUE;
}