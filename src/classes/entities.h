#ifndef ENTITIES_H
#define ENTITIES_H

#include "clientclass.h"
#include "vector.h"
#include "structures.h"
#include "cliententitylist.h"
#include "globals.h"
#include "sigscan/sigscan.h"
#include "math.h"

#include <string.h>



#ifdef _WIN32

#include <Windows.h>

#endif


struct FileWeaponInfo_t
{
	void *shit;
	bool done;
	bool _unk07;
	char class_name[80];
	char printname[80];
	char viewmodel[80];
	char playermodel[80];
	char _unk326[32];
	char anim_prefix[16];
	inline float &penetration(void)
	{
		float *a = (float *)(0x7C4 + (char *)this);
		return *a;
	}
	inline int &damage(void)
	{
		int *a = (int *)(0x7C8 + (char *)this);
		return *a;
	}
	inline float &range(void)
	{
		float *a = (float *)(0x7CC + (char *)this);
		return *a;
	}
	inline float &range_modifier(void)
	{
		float *a = (float *)(0x7D0 + (char *)this);
		return *a;
	}
};

extern Structures structs;


#ifndef fuck 
#define fuck(a) MessageBoxA(0, #a, #a, 0);
#endif

class Vector;
class ClientClass;
typedef struct mstudiobbox_t // stolen 
{
	int	bone;
	int	group; // intersection group
	Vector bbmin; // bounding box
	Vector bbmax;
	int	szhitboxnameindex; // offset to the name of the hitbox.
	int	unused[8];
	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";
		return ((const char*)this) + szhitboxnameindex;
	}
} hitbox_t;
struct mstudiohitboxset_t
{
	int	sznameindex;
	inline char * const	GetName(void) const { return ((char *)this) + sznameindex; }
	int	numhitboxes;
	int	hitboxindex;
	inline mstudiobbox_t *pHitbox(int i) const { return (mstudiobbox_t *)(((byte *)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	int id;
	int version;
	long checksum;
	char name[64];
	int length;
	Vector eyepos;
	Vector illumpos;
	Vector hull_min;
	Vector hull_max;
	Vector view_bbmin;
	Vector view_bbmax;
	int flags;
	int numbones;
	int boneindex;
	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;

	inline mstudiohitboxset_t	*pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};
};
class CStudioHdr
{
public:
	studiohdr_t *studio;
};
class CBaseAnimating
{
#ifdef _WIN32
	const int offset = 0;
#else
	const int offset = 1;
#endif
	template<typename t>
	t get(unsigned short which)
	{
		return t((*(char ***)(this))[which + offset]);
	}
public:
	inline bool SetupBones(matrix3x4_t *bones, float time)
	{
		auto setupbones = get<bool(__thiscall *)(void *, matrix3x4_t *, int bones, int flag, float time)>(version == CSGO ? 13 : 16); // gmod
		return setupbones(this, bones, 128, 256, time);
	}
};

class ClientNetworkable
{
#ifdef _WIN32
	const int offset = 0;
#else
	const int offset = 1;
#endif
	template<typename t>
	t get(unsigned short which)
	{
		return t((*(char ***)(this))[which + offset]);
	}
public:
	inline ClientClass *GetClientClass(void)
	{
		return get<ClientClass *(__thiscall *)(void *)>(2)(this);
	}
	inline int getvar(const char *name, RecvTable *override = 0)
	{
		RecvTable *current = override;
		if (!current)
		{
			auto cl = GetClientClass();
			current = cl->table;
		}
		for (int i = 0; i < current->nprops; i++)
		{
			auto prop = current->props[i];
			if (prop.datatable)
			{
				auto offset = getvar(name, prop.datatable);
				if (offset != 0)
					return prop.offset + offset;
			}
			else if (!strcmp(prop.name, name))
				return prop.offset;
		}
		return 0;
	}
};

class ClientEntity
{
#ifdef _WIN32
	const int offset = 0;
#else
	const int offset = 1;
#endif

	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which + offset]);
	}
	inline int getoffset(const char *name)
	{
		return GetNetworkable()->getvar(name);
	}
public:
	template <typename t>
	inline t &getvar(const char *name, int *stored = 0)
	{
		char *base = (char *)this;
		if (stored && *stored != -1)
			return *(t *)(base + *stored);
		auto offset = GetNetworkable()->getvar(name);
		if (stored)
			*stored = offset;
		return *(t *)(base + offset);
	}
	inline ClientNetworkable *GetNetworkable(void)
	{
		return (ClientNetworkable *)((char *)(this) + 8);
	}
	inline Vector &GetAbsOrigin(void) // TODO: get real vtable index and fix this so no lag
	{
		return get<Vector &(__thiscall *)(void *)>(version == CSGO ? 10 : 9)(this);
	}

	inline CStudioHdr *GetStudioHdr(void)
	{
		CStudioHdr **ret = 0;
		if (version == GARRYSMOD)
			ret = (CStudioHdr **)(getoffset("m_pBoneManipulator") - 0x9C + (char *)this);
		else
			ret = (CStudioHdr **)(getoffset("m_bSuppressAnimSounds") + 0x2 + (char *)this);
		return *ret;
	}

	inline int GetHitBoxSet(void)
	{
		static int hitbox_off = -1;
		int ret = getvar<int>("m_nHitboxSet", &hitbox_off);
		return ret;
	}

	int GetNumHitboxes(void)
	{
		auto hitbox_set = GetHitboxSet();
		if (!hitbox_set)
			return 0;
		return hitbox_set->numhitboxes;
	}

	mstudiohitboxset_t *GetHitboxSet(void)
	{
		auto hdr = GetStudioHdr();
		if (!hdr) return 0;
		auto s = hdr->studio;
		if (!s) return 0;
		auto hitbox_set = s->pHitboxSet(GetHitBoxSet());
		return hitbox_set;
	}

	hitbox_t *GetHitbox(int which)
	{
		auto hitbox_set = GetHitboxSet();
		if (!hitbox_set)
			return 0;
		return hitbox_set->pHitbox(which);
	}

	CBaseAnimating *GetAnimating(void)
	{
		return (CBaseAnimating *)(4 + (char *)this);
	}

	//pls use matrix3x4_t bones[128];
	inline bool SetupBones(matrix3x4_t *bones, float time)
	{
		return GetAnimating()->SetupBones(bones, time);
	}
	bool IsOnGround(void)
	{
		static int flags_off = -1;
		return this->getvar<int>("m_fFlags", &flags_off) & 1;
	}

	bool IsDormant(void)
	{
		static int dormant = 0xE9;
		return this->getvar<bool>("m_bDormant", &dormant);
	}

	ClientEntity *GetActiveWeapon(void)
	{
		static int weapon_off = -1;
		return structs.entity_list->GetClientEntityFromHandle(this->getvar<CBaseHandle>("m_hActiveWeapon", &weapon_off));
	}

	bool CanShoot(float time)
	{
		ClientEntity *weapon = GetActiveWeapon();
		if (!weapon)
			return false;
		static int nexttime_off = -1;
		if (weapon->getvar<float>("m_flNextPrimaryAttack", &nexttime_off) > time)
			return false;
		static int ammo_off = -1;
		return this->getvar<int>("m_iAmmo", &ammo_off) > 0;
	}
	FileWeaponInfo_t *GetCSWpnData(void)
	{
		typedef FileWeaponInfo_t *(__thiscall *func)(void *);
		static func fn = 0;
		if (!fn)
			fn = (func)SigScan(
				"\x55"
				"\x8B\xEC"
				"\x81\xEC????"
				"\xB8????"
				"\x57"
				"\x8B\xF9"
				"\x66\x39\x87????"
				"\x0F\x85", GetModuleHandleA("client.dll"));
		return fn(this);
	}
	virtual ~ClientEntity() = 0;
private:
	virtual void *SetRefEHandle(unsigned long) = 0;
public:
	virtual unsigned long &GetRefEHandle(void) = 0;
};


#endif