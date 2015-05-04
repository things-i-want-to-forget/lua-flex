#define WIN32_LEAN_AND_MEAN
#define VS_EXTRA_LEAN
#include <Windows.h>

extern DWORD __stdcall source_hack(LPVOID);

BOOL APIENTRY DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, &source_hack, 0, 0, 0);
	}

	return TRUE;
}