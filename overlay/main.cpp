#include <Windows.h>
#include <string>

typedef void *(__cdecl *CreateInterfaceFn)(const char *name, int *found);

template<class t>
inline t GetInterface(const char *module, const char *name)
{
	HMODULE hmodule = GetModuleHandleA(module);
	CreateInterfaceFn create = (CreateInterfaceFn)GetProcAddress(hmodule, "CreateInterface");

	void *ret = 0;

	char temp_name[256]; // up this if it causes problems
	for (unsigned short i = 0; i < 1000; i++)
	{
		sprintf_s(temp_name, "%s%03i", name, i);
		ret = create(temp_name, 0);
		if (ret) break;
	}

	return t(ret);
}

struct VGUI_Setup
{
	inline struct VGUI_Surface *GetSurface()
	{
		struct VGUI_Surface *ret = 0;
		auto func = (*(VGUI_Surface *(__thiscall ***)(void *ths, const char *name))(this))[4];

		char temp_name[64]; // up this if it causes problems
		for (unsigned short i = 0; i < 1000; i++)
		{
			sprintf_s(temp_name, "VGUI_Surface%03i", i);
			ret = func(this, temp_name);
			if (ret) break;
		}

		return ret;
	}
} *input;


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason != DLL_PROCESS_ATTACH) return TRUE;
	input = GetInterface<VGUI_Setup *>("vgui2_s.dll", "VGUI_Setup");

	if (!input)
		MessageBoxA(0, "NO SETUP", "xd", 0);
	else
	{
		char temp[256];
		sprintf_s(temp, "VGUI_Setup = %p", input);

		MessageBoxA(0, temp, temp, 0);

		auto surface = input->GetSurface();

		sprintf_s(temp, "VGUI_Surface = %p", surface);

		MessageBoxA(0, temp, temp, 0);



	}

	return TRUE;
}