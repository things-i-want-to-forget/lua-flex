#ifndef STRUCTURES_H
#define STRUCTURES_H

class Lau;
class CClient;
class CInput;
class CPrediction;
class CClientTools;
class ClientModeShared;
class CEngineTrace;
class VPanelWrapper;
class CMatSystemSurface;
class CEngineClient;
class CClientEntityList;
class CIVDebugOverlay;
class CGlobalVarsBase;
class CStudioRenderContext;
class CToolFrameworkInternal;
class CMaterialSystem;
class CGameMovement;
class CEngineVGui;
class CMoveHelper;
class CGameEventManager;
class MSockAPI;

struct model_t;
class CStudioHdr;
class CModelInfo
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}
public:
	inline CStudioHdr *GetStudiomodel(model_t *model)
	{ // 28
		return get<CStudioHdr *(__thiscall *)(void *, model_t *)>(28)(this, model);
	}
};

class CLocalize
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}
public:

	inline wchar_t *Find(char const *tokenName)
	{
		return get<wchar_t *(__thiscall *)(void *, const char *)>(10)(this, tokenName);
	}
};
class CPhysicsSurfaceProps;

struct Structures
{
	Lau *L;
	CLocalize *local;               // Localize_
	CClientTools *tools;            // VCLIENTTOOLS
	CPhysicsSurfaceProps *sprops;   // VPhysicsSurfaceProps
	CMoveHelper *move_helper;
	CGameMovement *game_movement;
	CModelInfo *model_info;
	CClient *client;
	CInput *input;
	CPrediction *prediction;
	ClientModeShared *client_mode;
	CEngineTrace *trace;
	VPanelWrapper *panel;
	CMatSystemSurface *surface;
	CEngineClient *engine;
	CClientEntityList *entity_list;
	CGlobalVarsBase *globals;
	CIVDebugOverlay *debug;
	CStudioRenderContext *render;
	CToolFrameworkInternal *framework;
	CMaterialSystem *materials;
	CEngineVGui *engine_vgui;
	CGameEventManager *event_manager;
	MSockAPI *sock;
};

extern Structures structs;

#endif