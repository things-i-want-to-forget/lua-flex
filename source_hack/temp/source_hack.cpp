#include <Windows.h>
#include <string>
#include "engines.h"
#include "structures.h"
#include "panelwrapper.h"
#include "matsystemsurface.h"
#include "vtable.h"
#include "sigscan.h"

#define CREATEMOVE_INDEX (version == CSGO ? 24 : 21)
#define PAINTTRAVERSE_INDEX (41)

EngineVersion version = GARRYSMOD;
VTable *client_mode_vt = 0;
VTable *panel_vt = 0;
ulong font;

typedef void *(__cdecl *CreateInterfaceFn)(const char *name, int *found);

template<class t>
t GetInterface(const char *module, const char *name)
{
	HMODULE hmodule = GetModuleHandleA(module);
	CreateInterfaceFn create = (CreateInterfaceFn)GetProcAddress(hmodule, "CreateInterface");

	void *ret = 0;

	int found = 1;

	char temp_name[256]; // up this if it causes problems
	for (unsigned short i = 0; i < 1000; i++)
	{
		sprintf_s(temp_name, "%s%03i", name, i);
		ret = create(temp_name, &found);
		if (found == 0 || ret) break;
	}

	return t(ret);
}

inline char *getvfunc(void *obj, unsigned short which)
{
	return (*(char ***)obj)[which];
}

bool __fastcall CreateMove_Hook(ClientModeShared *ths, void*, float frametime, char *cmd)
{
	typedef bool (__thiscall *CreateMoveFn)(ClientModeShared *, float frametime, char *cmd);
	bool ret = CreateMoveFn(client_mode_vt->getold(CREATEMOVE_INDEX))(ths, frametime, cmd);

	float *pitch = (float *)(cmd + 0xC);
	*pitch = 0;

	return ret;
}

void __fastcall PaintTraverse_Hook(VPanelWrapper *ths, void *, unsigned int panel, bool a, bool b)
{
	typedef void(__thiscall *PaintTraverseFn)(VPanelWrapper *, unsigned int, bool, bool);
	PaintTraverseFn(panel_vt->getold(PAINTTRAVERSE_INDEX))(ths, panel, a, b);
	if (ths->GetName(panel) && !strcmp(ths->GetName(panel), "HudCrosshair"))
	{
		structs.surface->DrawSetColor(Color(255, 255, 255, 255));
		structs.surface->DrawFilledRect(10, 10, 50, 50);
	}
}

DWORD __stdcall source_hack(LPVOID)
{
	version = GetEngineVersion(GetAppID());

	structs.client = GetInterface<CClient *>("client.dll", "VClient");
	structs.trace = GetInterface<CEngineTrace *>("engine.dll", "VEngineTrace");
	//IN_ActivateMouse is 16 in csgo
	//structs.input = **(CInput ***)(getvfunc(structs.client, 14) + 2); // IN_ActivateMouse
	
	structs.prediction = GetInterface<CPrediction *>("client.dll", "VClientPrediction");

	structs.panel = GetInterface<VPanelWrapper *>("vgui2.dll", "VGUI_Panel");
	structs.surface = GetInterface<CMatSystemSurface *>("vguimatsurface.dll", "VGUI_Surface");

	/*
	font = structs.surface->CreateFont();
	structs.surface->SetFontGlyphSet(font, "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);
	*/

	char temp[256];
	sprintf_s(temp, 256, "surface: %p", structs.surface);
	MessageBoxA(0, temp, temp, 0);

	//can be found with an xref to scripts/vgui_screens.txt

	switch (version)
	{
	case GARRYSMOD:
		{
			structs.client_mode = **(ClientModeShared ***)((char *)(SigScan(
				"\xE8????"
				"\xA3????" // here
				"\xE8????"
				"\x8B\x10"
				"\x8B\xC8"
				"\x8B\x02"
				"\x68????"
				"\xFF\xD0"
				"\xC3",
				GetModuleHandleA("client.dll")
				)) + 0x6);
			break;
		}
	case CSGO:
		{
			auto addr = SigScan(
				"\xA1????"
				"\xC7\x05????????" // first dword of ?
				"\xA8\x01"
				"\x75?"
				"\x83\xC8\x01"
				"\xA3????"
				"\xE8????"
				"\x68????"
				"\xE8????"
				"\x83\xC4\x04"
				"\xA1????"
				"\xB9????"
				"\x68????"
				"\xFF\x10"
				"\xC3",
				GetModuleHandleA("client.dll")
				);
			structs.client_mode = **(ClientModeShared ***)((char *)(addr) + 0x7);

			char temp[256];
			sprintf_s(temp, 256, "%p %p", structs.client_mode, addr);
			MessageBoxA(0, temp, temp, 0);

			break;
		}
	}


	//client_mode_vt = new VTable(structs.client_mode);

	//client_mode_vt->hook(CREATEMOVE_INDEX, &CreateMove_Hook);

	panel_vt = new VTable(structs.panel);

	panel_vt->hook(PAINTTRAVERSE_INDEX, &PaintTraverse_Hook);
	return NULL;
}