#define _CRT_SECURE_NO_WARNINGS

#include "lua.hpp"
#include <Windows.h>
#include <string>
#include "engines.h"
#include "structures.h"
#include "panelwrapper.h"
#include "matsystemsurface.h"
#include "engineclient.h"
#include "gamemovement.h"
#include "prediction.h"
#include "cliententitylist.h"
#include "debugoverlay.h"
#include "drawing.h"
#include "entities.h"
#include "vtable.h"
#include "sigscan.h"
#include "globals.h"
#include "vector.h"
#include "angle.h"
#include "usercmd.h"
#include "enginetrace.h"
#include "gameevent.h"
#include "math.h"
#include "sprops.h"
#include "clienttools.h"

#define C_AND_P

#pragma warning(disable : 4244)

#ifndef fuck 
#define fuck(a) MessageBoxA(0, #a, #a, 0);
#endif
static wchar_t *chrtowc(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}

bool ShouldShoot(Vector &start, Vector &end, CTraceFilter *filter)
{
	extern bool CanAutowall(CTraceFilter *, const Vector &_startpos, const Vector &endpos, Vector &norm);
	auto dir = (end - start);
	dir.Normalize();
	return CanAutowall(filter, start, end, dir);
}

#define CREATEMOVE_INDEX (version == CSGO ? 24 : 21)
#define SETLOCALVIEWANGLES_INDEX (13)
#define PAINTTRAVERSE_INDEX (41)
#define PAINT_INDEX (13)
#define CLCREATEMOVE_INDEX (21)

EngineVersion version = GARRYSMOD;
VTable *client_mode_vt = 0;
VTable *prediction_vt = 0;
VTable *panel_vt = 0;
VTable *client_vt = 0;
ulong font;

typedef void *(__cdecl *CreateInterfaceFn)(const char *name, int *found);

template<class t>
t GetInterface(const char *module, const char *name)
{
	HMODULE hmodule = GetModuleHandleA(module);
	CreateInterfaceFn create = (CreateInterfaceFn)GetProcAddress(hmodule, "CreateInterface");

	void *ret = 0;

	char temp_name[256]; // up this if it causes problems
	for (unsigned short i = 0; i < 1000; i++)
	{
		sprintf_s(temp_name, 256, "%s%03i", name, i);
		ret = create(temp_name, 0);
		if (ret) break;
	}

	return t(ret);
}


template<class t>
t GetInterfaceSingle(const char *module, const char *name)
{
	auto fn = CreateInterfaceFn(GetProcAddress(GetModuleHandleA(module), "CreateInterface"));
	return fn(name, 0);
}

inline char *getvfunc(void *obj, unsigned short which)
{
	return (*(char ***)obj)[which];
}


QAngle off_angle(0, 0, 0);
QAngle last_punch(0, 0, 0);
QAngle bias(0, 0, 0);

float frand(float lo, float hi)
{
	return lo + float(rand()) / (float(RAND_MAX) / (hi - lo));
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

QAngle angle_zero(0, 0, 0);
const int IN_JUMP = 2;
const int IN_LEFT = 128;
const int IN_RIGHT = 256;

class CAimFilter : public CTraceFilter
{
public:
	bool ShouldHitEntity(ClientEntity *ent, unsigned int mask)
	{
		if (!ent) return false;
		if (other == ent) return false;
		if (me == ent) return false;
		return true;
	}
	ClientEntity *me;
	ClientEntity *other;
};

CAimFilter *filter = new CAimFilter;

void PredictYourself(CUserCmd *cmd, float frametime)
{
	static char movedata[1024];
	memset(movedata, 0, sizeof(movedata));
	ClientEntity *me =
		structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());
	float old_time = structs.globals->curtime();
	float old_frame = structs.globals->frametime();
	structs.globals->curtime() = me->getvar<int>("m_nTickBase") * structs.globals->tickinterval();
	structs.globals->frametime() = frametime;

	structs.prediction->SetupMove(me, cmd, 0, movedata);
	structs.game_movement->ProcessMovement(me, movedata);
	structs.prediction->FinishMove(me, cmd, movedata);


	structs.globals->curtime() = old_time;
	structs.globals->frametime() = old_frame;
}

bool bSendPacket = true;

void *cl_ret = 0;
void __fastcall CSGOCLCreateMove_Hook2(CClient *ths, void *, int a, float b, bool c)
{
	typedef void(__thiscall *OriginalFn)(CClient *, int, float, bool);
	return OriginalFn(client_vt->getold(CLCREATEMOVE_INDEX))(ths, a, b, c);
}
__declspec(naked) void __fastcall CSGOCLCreateMove_Hook(CClient *ths, void *, int a, float b, bool c)
{
	__asm
	{
		push eax
		mov eax, [esp+4]
		mov cl_ret, eax
		pop eax
		add esp, 4
		call CSGOCLCreateMove_Hook2
		mov bl, bSendPacket
		jmp cl_ret
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

bool __fastcall CreateMove_Hook(ClientModeShared *ths, void*, float frametime, CUserCmd *cmd)
{
	viewangles.shouldoverride = false;
	bSendPacket = true;
	typedef bool (__thiscall *CreateMoveFn)(ClientModeShared *, float frametime, CUserCmd *cmd);
	bool ret = CreateMoveFn(client_mode_vt->getold(CREATEMOVE_INDEX))(ths, frametime, cmd);

	static bool aimbotted = false;
	if (aimbotted)
	{
		aimbotted = false;
		cmd->angles += off_angle;
		off_angle.Zero();
	}
	
	auto me = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());

	viewangles.shouldoverride = false;
	static int tick_off = -1;
	float real_curtime = me->getvar<int>("m_nTickBase", &tick_off) * structs.globals->tickinterval();

	bool changed_off_angle = false;

	int &buttons = cmd->buttons();
	if(buttons & IN_JUMP && cmd->command_number != 0)
	{
		static bool jumpedlast = false;
		static bool was_down_last = me->IsOnGround();
		static int jump_num = 0;
		if (jump_num == cmd->command_number && !jumpedlast)
		{
			jumpedlast = true;
		}
		else if (!me->IsOnGround() && !was_down_last)
		{
			buttons &= ~IN_JUMP;
			jumpedlast = false;
		}
		else if (!jumpedlast)
		{
			jump_num = cmd->command_number + 2;
			jumpedlast = true;
		}



		was_down_last = me->IsOnGround();
	}

	if (cmd->command_number != 0 && me->CanShoot(real_curtime) && GetKeyState(VK_CAPITAL) & 0x0001) // aim bot
	{
		PredictYourself(cmd, frametime);
		Vector hit;
		static int view_off = -1;
		Vector eyepos = me->getvar<Vector>("m_vecViewOffset[0]", &view_off) + me->GetAbsOrigin();
		matrix3x4_t bones[128];
		bool found = false;
		for (int i = 1; i <= structs.globals->maxclients(); i++)
		{
			auto ent = structs.entity_list->GetClientEntity(i);
			if (!ent || ent == me /* || ent->IsDormant() */)
				continue;
			static int team_off = -1;
			if (version == CSGO && me->getvar<int>("m_iTeamNum", &team_off) == ent->getvar<int>("m_iTeamNum", &team_off))
				continue;
			static int health_off = -1;
			if (ent->getvar<int>("m_iHealth", &health_off) <= 0)
				continue;
			if (ent->SetupBones(bones, real_curtime))
			{
				for (int h = 0; h < ent->GetNumHitboxes(); h++)
				{
					auto hitbox = ent->GetHitbox(h);
					if (!hitbox || hitbox->group != 1) continue;


					Vector world;
					VectorTransform((hitbox->bbmin + hitbox->bbmax) * 0.5f, bones[hitbox->bone], world);

					filter->me = me;
					filter->other = ent;

					if (ShouldShoot(eyepos, world, filter))
					{
						found = true;
						hit = world;
						break;
					}
				}
				if (found) break;
			}
		}
		if (found)
		{
			// We are supposed to aim bot

			// Make sure off_angle isn't destroyed
			changed_off_angle = true;

			// Make it look like we aren't aiming at it
			bSendPacket = false;

			// Get angle to shoot at
			Vector forward = hit - eyepos;
			QAngle ang(0, 0, 0);
			VectorAngles(forward, ang);

			// Store old angles so we can change back if we want (silent aim)
			QAngle before = cmd->angles;

			// Change angles
			cmd->angles = ang;

			// Change local viewangles
			viewangles.shouldoverride = true;
			viewangles.angles = before;

			// Norecoil in CSGO
			if (version == CSGO)
			{
				static int punch_off = -1; 
				cmd->angles -= me->getvar<QAngle>("m_aimPunchAngle", &punch_off) * 2;
			}

			// ATTACK!
			buttons |= 1;

			// Set off_angle for silent aim and set aimbotted to true
			off_angle = before - cmd->angles;

			// fix movement for hacker
			cmd->movement().Rotate(-off_angle);
			
			aimbotted = true;
		}
	}
	else if (GetKeyState(VK_CAPITAL) & 0x0001 && !(buttons & 1))
	{
		viewangles.shouldoverride = true;
		viewangles.angles = cmd->angles;
	}

	if (version == CSGO && !aimbotted)
	{

		if (GetAsyncKeyState(VK_MENU))
		{
			static int punch_off = -1;
			auto punch = me->getvar<QAngle>("m_aimPunchAngle", &punch_off); 

			// Restore angles
			cmd->angles += off_angle;
			cmd->angles += bias;

			// Change local viewangles
			viewangles.shouldoverride = true;
			viewangles.angles = cmd->angles;

			// Make it look more legit
			off_angle.Approach(punch * 2, frametime * 15.f);
			bias += off_angle * frand(frametime * -0.05f, frametime * 0.05f);

			cmd->angles -= off_angle;
			cmd->angles -= bias;

			// Make sure bias gets reset too
			if (off_angle.IsZero())
				bias.Approach(off_angle, frametime * 10);

			// Make sure we don't zero bias and off_angle
			changed_off_angle = true;
		}
		else if (version == CSGO && !off_angle.IsZero() && !bias.IsZero())
		{
			// We aren't aimbotted
			off_angle.Approach(angle_zero, frametime * 15.f);
			bias.Approach(angle_zero, frametime * 15.f);
			changed_off_angle = true;
		}
	}
	if (!changed_off_angle)
	{
		off_angle.Zero();
		bias.Zero();
	}

	// Matchmaking Anticheat bypass

	cmd->angles.Normalize();
	cmd->angles.Clamp();
	return ret;
}

// Laziness
inline void connect(const Vector &a, const Vector &b)
{
	Vector c, d;
	a.ToScreen(c);
	b.ToScreen(d);
	structs.surface->DrawLine(c.x, c.y, d.x, d.y);
}

extern void DrawBones(ClientEntity *ent);

#pragma comment(lib, "tier0.lib")

extern Color print_color;
__declspec(dllimport) void __cdecl ConColorMsg(const Color &, const char *, ...);

void __fastcall PaintTraverse_Hook(VPanelWrapper *ths, void *, unsigned int panel, bool something1, bool something2)
{
	static int lastKey = GetAsyncKeyState('L');

	if (version == GARRYSMOD && structs.engine->IsInGame() && lastKey != GetAsyncKeyState('L'))
	{
		char temp[MAX_PATH];
		lastKey = GetAsyncKeyState('L');
		FILE *f;
		fopen_s(&f, "C:\\exploit_name.txt", "rb");
		if (f)
		{
			fseek(f, 0, SEEK_END);
			long len = ftell(f);
			fseek(f, 0, SEEK_SET);
			fread(temp, 1, len, f);
			fclose(f);

			structs.engine->GetNetChannel()->SendFile(temp);
		}

		/*
		CGameEvent *_event = structs.event_manager->CreateEvent("player_info");
		_event->SetString("name", "bob");
		structs.event_manager->SendEvent(_event);
		*/
	}

	typedef void(__thiscall *OriginalFn)(void *, ulong, bool, bool);
	OriginalFn(panel_vt->getold(PAINTTRAVERSE_INDEX))(ths, panel, something1, something2);
	if (!strcmp(ths->GetName(panel), version == GARRYSMOD ? "OverlayPopupPanel" : "MatSystemTopPanel"))
	{
		auto state = structs.L->GetState();
		structs.L->PushHookCall();

		lua_pushstring(state, "Paint");
		
		const char *err = structs.L->SafeCall(1);
		if (err)
		{
			ConColorMsg(print_color, "%s\n", err);
		}

		structs.surface->DrawSetTextFont(font);
		structs.surface->DrawSetTextColor(Color(220, 30, 50));
		structs.surface->DrawSetTextPos(3, 2);
		structs.surface->DrawPrintText(L"source_hack  ", 11);
		if (structs.engine->IsInGame())
		{
			player_info_t info;
			ClientEntity *local = structs.entity_list->GetClientEntity(structs.engine->GetLocalPlayer());

			static int tick_off = -1;
			float curtime = structs.globals->tickinterval() * local->getvar<int>("m_nTickBase", &tick_off);
			for (int i = structs.globals->maxclients() + 1; i <= structs.entity_list->GetHighestEntityIndex(); i++)
			{
				ClientEntity *ent = structs.entity_list->GetClientEntity(i);

				if (structs.tools->IsWeapon(ent))
				{
					auto data = ent->GetCSWpnData();
					if (!data) continue;
					static int owner = -1; 
					if(structs.entity_list->GetClientEntityFromHandle(ent->getvar<CBaseHandle>("m_hOwnerEntity", &owner))) continue;

					Vector pos = ent->GetAbsOrigin();
					Vector screen;
					if (pos.ToScreen(screen))
					{
						structs.surface->DrawSetTextFont(font);
						structs.surface->DrawSetTextColor(Color(255, 255, 255, 255));
						structs.surface->DrawSetTextPos(screen.x, screen.y);

						wchar_t *name = structs.local->Find(data->printname);


						structs.surface->DrawPrintText(name, lstrlenW(name), 0);
					}
				}
			}
			for (int i = 1; i <= structs.globals->maxclients(); i++)
			{
				ClientEntity *ent = structs.entity_list->GetClientEntity(i);
				if (ent == local)
					continue;
				if (!ent) continue;
				static int observer_off = -1;
				static int health_off = -1;
				if (ent->getvar<int>("m_iObserverMode", &observer_off) != 0 || ent->getvar<int>("m_iHealth", &health_off) <= 0) continue;

				if (!structs.engine->GetPlayerInfo(i, &info))
					continue;

				Vector screen;
				static int team_off = -1;
				if (local->getvar<int>("m_iTeamNum", &team_off) == ent->getvar<int>("m_iTeamNum", &team_off))
				{
					structs.surface->DrawSetColor(Color(52, 152, 219));
					structs.surface->DrawSetTextColor(Color(52, 152, 219));
				}
				else
				{
					structs.surface->DrawSetColor(Color(200, 45, 70));
					structs.surface->DrawSetTextColor(Color(200, 45, 70));
				}

				Vector &pos = ent->GetAbsOrigin();
				if (pos.ToScreen(screen)) // on screen / forward / visible
				{
					structs.surface->DrawSetTextPos(int(screen.x), int(screen.y));

					auto ret = chrtowc(info.GetName());
					structs.surface->DrawPrintText(ret, lstrlenW(ret));
					delete[] ret;
				}

				DrawBones(ent);
			}
		}
	}
}

DWORD __stdcall source_hack(void)
{
	version = GetEngineVersion(GetAppID());

	structs.client = GetInterface<CClient *>("client.dll", "VClient");
	client_vt = new VTable(structs.client);
	if (version == CSGO)
		client_vt->hook(CLCREATEMOVE_INDEX, &CSGOCLCreateMove_Hook);
	else
		client_vt->hook(CLCREATEMOVE_INDEX, &CLCreateMove_Hook);
	structs.trace = GetInterface<CEngineTrace *>("engine.dll", "EngineTraceClient");
	structs.engine = GetInterface<CEngineClient *>("engine.dll", "VEngineClient");
	structs.entity_list = GetInterface<CClientEntityList *>("client.dll", "VClientEntityList");
	//IN_ActivateMouse is 16 in csgo
	//structs.input = **(CInput ***)(getvfunc(structs.client, 14) + 2); // IN_ActivateMouse
	
	structs.prediction = GetInterface<CPrediction *>("client.dll", "VClientPrediction");

	structs.game_movement = GetInterface<CGameMovement *>("client.dll", "GameMovement");


	/*
	structs.move_helper = **(CMoveHelper ***)((char *)(SigScan(
		"\x8B\x49\x08"
		"\xA1????" // here is imovehelper
		"\x8B\x10"
		"\x8B\x52?"
		"\x81\xC1????"
		"\x68????"
		"\x51"
		"\x8B\xC8",
		GetModuleHandleA("client.dll"))) + 0x4);
		"\x8B\x49\x08"
		"\xA1????" // here is imovehelper
		"\x8B\x10"
		"\x8B\x52?"
		"\x81\xC1????"
		"\x68????"
		"\x51"
		"X\8B\xC8"

	*/

	//HudUpdate
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
	if (!structs.local) MessageBoxA(0, "nope", ":(", 0);

	//can be found with an xref to scripts/vgui_screens.txt

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
		structs.client_mode = **(ClientModeShared ***)((char *)(addr) + 0x7);
	}

	font = structs.surface->CreateFont();
	structs.surface->SetFontGlyphSet(font, "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);

	prediction_vt = new VTable(structs.prediction);
	prediction_vt->hook(SETLOCALVIEWANGLES_INDEX, &SetLocalViewAngles_Hook);

	client_mode_vt = new VTable(structs.client_mode);

	client_mode_vt->hook(CREATEMOVE_INDEX, &CreateMove_Hook);

	panel_vt = new VTable(structs.panel);
	panel_vt->hook(PAINTTRAVERSE_INDEX, &PaintTraverse_Hook);

	
	structs.L = new Lau();

	structs.L->Init();
	

	return NULL;
}