#include "init.h"

#include <windows.h>

#include "lau/lau.h"
#include "util.h"
#include "sigscan/sigscan.h"
#include "classes/structures.h"
#include "classes/panelwrapper.h"
#include "classes/matsystemsurface.h"
#include "msockapi.h"
#include "vtable.h"
#include "hooks.h"

#include "windows/espwind.h"

using namespace util;

EngineVersion version = GARRYSMOD;

unsigned long font;
extern void(__cdecl *CL_Move)(float extra_samples, bool bFinalTick);
extern void(__cdecl *CL_SendMove)(void);


void csgolua::Init() {

	CreateESPWind();


	CL_Move = (decltype(CL_Move))(SigScan("\x55\x8B\xEC\x83\xEC\x4C\x53\x56\x57\x8B\x3D????\x8A\xF9\xF3\x0F\x11\x45?\x83\xBF", GetModuleHandleA("engine.dll")));

	CL_SendMove = (decltype(CL_SendMove))(SigScan("\x55\x8B\xEC\xB8????\xE8????\x8B\x0D????\x56\x57\x8B\x81????\x8B\xB1????\x40\x03\xC6\x83\x3D", GetModuleHandleA("engine.dll")));
	
	version = CSGO; // todo add this back in

	structs.client = GetInterface<CClient *>("client.dll", "VClient");

	structs.trace = GetInterface<CEngineTrace *>("engine.dll", "EngineTraceClient");
	structs.engine = GetInterface<CEngineClient *>("engine.dll", "VEngineClient");
	structs.entity_list = GetInterface<CClientEntityList *>("client.dll", "VClientEntityList");

	structs.prediction = GetInterface<CPrediction *>("client.dll", "VClientPrediction");

	structs.game_movement = GetInterface<CGameMovement *>("client.dll", "GameMovement");

	structs.globals = **(CGlobalVarsBase ***)(getvfunc(structs.client, 11) + (version == CSGO ? 0xA : 5));

	structs.model_info = GetInterface<CModelInfo *>("engine.dll", "VModelInfoClient");
	structs.render = GetInterface<CStudioRenderContext *>("studiorender.dll", "VStudioRender");
	structs.framework = GetInterface<CToolFrameworkInternal *>("engine.dll", "VENGINETOOLFRAMEWORK");
	structs.panel = GetInterface<VPanelWrapper *>("vgui2.dll", "VGUI_Panel");
	structs.surface = GetInterface<CMatSystemSurface *>("vguimatsurface.dll", "VGUI_Surface");
	structs.debug = GetInterface<CIVDebugOverlay *>("engine.dll", "VDebugOverlay");
	structs.materials = GetInterface<CMaterialSystem *>("materialsystem.dll", "VMaterialSystem");
	structs.engine_vgui = GetInterface<CEngineVGui *>("engine.dll", "VEngineVGui");
	structs.event_manager = GetInterface<CGameEventManager *>("engine.dll", "GAMEEVENTSMANAGER");
	structs.sprops = GetInterface<CPhysicsSurfaceProps *>("vphysics.dll", "VPhysicsSurfaceProps");
	structs.tools = GetInterface<CClientTools *>("client.dll", "VCLIENTTOOLS");
	structs.local = GetInterface<CLocalize *>("localize.dll", "Localize_");


	if (version == GARRYSMOD)
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
	}
	else if (version == CSGO)
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
		structs.client_mode = **(ClientModeShared ***)((char *)(addr)+0x7);
	}

	font = structs.surface->CreateFont();
	structs.surface->SetFontGlyphSet(font, "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);

	hook::InitHooks();

	structs.L = new Lau();

	structs.L->Init();

	structs.sock = new MSockAPI();
	structs.sock->Connect("\\\\.\\pipe\\lf_esp");

	if (!structs.sock->connected())
	{
		char temp[256];
		sprintf_s(temp, "%i", GetLastError());
		MessageBoxA(0, "not connected to sock", temp, 0);
	}
	else
		MessageBoxA(0, "omg connected xd", "connect", 0);

}