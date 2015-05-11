#include "hooks.h"
#include "luawatcher.h"
#include "lau/lau.h"
#include "lua.hpp"
#include "classes/structures.h"
#include "classes/color.h"
#include "classes/usercmd.h"
#include "classes/engines.h"
#include "init.h"

#include "classes/cliententitylist.h"
#include "classes/globals.h"
#include "classes/entities.h"
#include "classes/engineclient.h"
#include "classes/matsystemsurface.h"
#include "classes/clienttools.h"
#include "classes/panelwrapper.h"
#include "classes/angle.h"
#include "classes/math.h"

#define CREATEMOVE_INDEX (version == CSGO ? 24 : 21)
#define SETLOCALVIEWANGLES_INDEX (13)
#define PAINTTRAVERSE_INDEX (41)
#define PAINT_INDEX (13)
#define CLCREATEMOVE_INDEX (21)

VTable *hook::client_mode_vt = 0;
VTable *hook::prediction_vt = 0;
VTable *hook::panel_vt = 0;
VTable *hook::client_vt = 0;


extern EngineVersion version;
extern unsigned long font;

using namespace csgolua;
using namespace hook;

bool bSendPacket = true;

void __fastcall CSGOCLCreateMove_Hook2(CClient *ths, void *, int a, float b, bool c)
{
	typedef void(__thiscall *OriginalFn)(CClient *, int, float, bool);
	return OriginalFn(client_vt->getold(CLCREATEMOVE_INDEX))(ths, a, b, c);
}

__declspec(naked) void __fastcall CSGOCLCreateMove_Hook(CClient *ths, void *, int a, float b, bool c)
{
	void *cl_ret;
	__asm
	{
		push eax;
		mov eax, [esp + 4];
		mov cl_ret, eax;
		pop eax;
		add esp, 4;
		call CSGOCLCreateMove_Hook2;
		mov bl, bSendPacket;
		jmp cl_ret;
	}
}

void __fastcall CLCreateMove_Hook(CClient *ths, void *, int a, float b, bool c)
{

	typedef void(__thiscall *OriginalFn)(CClient *, int, float, bool);
	OriginalFn(client_vt->getold(CLCREATEMOVE_INDEX))(ths, a, b, c);
	void *_ebp = 0;
	__asm mov _ebp, ebp
	*(*(unsigned char **)_ebp - 1) = bSendPacket;
}

extern Color print_color;

__declspec(dllimport) void __cdecl ConColorMsg(const Color &, const char *, ...);

bool __fastcall CreateMove_Hook(ClientModeShared *ths, void*, float frametime, CUserCmd *cmd)
{
	typedef bool(__thiscall *CreateMoveFn)(ClientModeShared *, float frametime, CUserCmd *cmd);
	bool ret = CreateMoveFn(client_mode_vt->getold(CREATEMOVE_INDEX))(ths, frametime, cmd);

	auto state = structs.L->GetState();

	structs.L->PushHookCall();

	lua_pushstring(state, "CreateMove");
	LPush(state, cmd, "CUserCmd");

	const char *err = structs.L->SafeCall(2, 0);
	if (err)
	{
		ConColorMsg(print_color, "%s\n", err);
	}

	cmd->angles.Normalize();
	cmd->angles.Clamp();
	return ret;
}


struct {
	bool shouldoverride;
	QAngle angles;
} viewangles = { false, QAngle(0, 0, 0) };


void __fastcall SetLocalViewAngles_Hook(CPrediction *ths, void *, QAngle &ang)
{
	typedef void(__thiscall *OriginalFn)(void *, QAngle &);
	auto me = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());

	if (version == CSGO && me)
	{
		static int view_off = -1;
		static int punch_off = -1;
		if (viewangles.shouldoverride)
			return OriginalFn(prediction_vt->getold(SETLOCALVIEWANGLES_INDEX))(ths, viewangles.angles - me->getvar<QAngle>("m_aimPunchAngle", &punch_off) - me->getvar<QAngle>("m_viewPunchAngle", &view_off));
	}
	return OriginalFn(prediction_vt->getold(SETLOCALVIEWANGLES_INDEX))(ths, ang);
}

void __fastcall PaintTraverse_Hook(VPanelWrapper *ths, void *, unsigned int panel, bool something1, bool something2)
{
	static int lastKey = GetAsyncKeyState('L');

	typedef void(__thiscall *OriginalFn)(void *, ulong, bool, bool);
	OriginalFn(panel_vt->getold(PAINTTRAVERSE_INDEX))(ths, panel, something1, something2);
	if (!strcmp(ths->GetName(panel), version == GARRYSMOD ? "OverlayPopupPanel" : "MatSystemTopPanel"))
	{

		structs.surface->DrawSetTextFont(font);
		structs.surface->DrawSetTextColor(Color(220, 30, 50));
		structs.surface->DrawSetTextPos(3, 2);
		structs.surface->DrawPrintText(L"source_hack", 11);
		auto state = structs.L->GetState();
		structs.L->PushHookCall();

		lua_pushstring(state, "Paint");

		const char *err = structs.L->SafeCall(1);
		if (err)
		{
			ConColorMsg(print_color, "%s\n", err);
		}

		fileChecker::checkDir();
	}
}

void hook::InitHooks() {
	client_vt = new VTable(structs.client);
	if (version == CSGO)
		client_vt->hook(CLCREATEMOVE_INDEX, &CSGOCLCreateMove_Hook);
	else
		client_vt->hook(CLCREATEMOVE_INDEX, &CLCreateMove_Hook);

	prediction_vt = new VTable(structs.prediction);
	prediction_vt->hook(SETLOCALVIEWANGLES_INDEX, &SetLocalViewAngles_Hook);

	client_mode_vt = new VTable(structs.client_mode);

	client_mode_vt->hook(CREATEMOVE_INDEX, &CreateMove_Hook);

	panel_vt = new VTable(structs.panel);
	panel_vt->hook(PAINTTRAVERSE_INDEX, &PaintTraverse_Hook);
}