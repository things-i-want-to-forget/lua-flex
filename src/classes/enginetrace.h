#ifndef ENGINETRACE_H
#define ENGINETRACE_H

#include "vector.h"
#include "engines.h"
#include "../offset.h"
#include <Windows.h>

extern EngineVersion version;

struct Ray_t;
class CTraceFilter;
struct trace_t;
class ClientEntity;
class CEngineTrace
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}
public:
	inline void TraceRay(Ray_t const &ray, unsigned int mask, CTraceFilter *filter, trace_t &trace)
	{
		return get<void(__thiscall *)(void *, Ray_t const &, unsigned int, CTraceFilter *, trace_t &)>(version == CSGO ? 5 : 4)(this, ray, mask, filter, trace);
	}
	inline int GetPointContents(const Vector &point, int mask, ClientEntity **ent = 0)
	{
		return get<int(__thiscall *)(void *, const Vector &, int, ClientEntity **)>(0)(this, point, mask, ent);
	}
};

class CTraceFilter
{
public:
	virtual bool ShouldHitEntity(ClientEntity *ent, unsigned int mask)
	{
		return true;
	};
	virtual int GetTraceType() const 
	{
		return 3;
	};
};

struct Ray_t
{
	VectorAligned m_Start; // starting point, centered within the extents
	VectorAligned m_Delta; // direction + length of the ray
	VectorAligned m_StartOffset; // Add this to m_Start to get the actual ray start
	VectorAligned m_Extents; // Describes an axis aligned box extruded along a ray
private:

	bool	m_IsRay_; // are the extents zero?
	bool	m_IsSwept_; // is delta != 0?
	float **matrix3x4_; // csgo only
public:

	bool &m_IsRay(void)
	{
		return *(bool *)(_offset(m_IsRay_) + (version == CSGO ? 4 : 0) + (char *)this);
	}
	bool &m_IsSwept(void)
	{
		return *(bool *)(_offset(m_IsSwept_) + (version == CSGO ? 4 : 0) + (char *)this);
	}

	void Init(Vector const& start, Vector const& end)
	{
		ZeroMemory(this, sizeof(*this));
		Vector delta = end - start;
		m_Delta.x = delta.x;
		m_Delta.y = delta.y;
		m_Delta.z = delta.z;
		m_Delta.w = 0;
		m_IsSwept() = !(delta.x == 0 && delta.y == 0 && delta.z == 0);
		m_Extents.x = 0;
		m_Extents.y = 0;
		m_Extents.z = 0;
		m_Extents.w = 0;
		m_IsRay() = true;
		// Offset m_Start to be in the center of the box
		m_StartOffset.x = 0;
		m_StartOffset.y = 0;
		m_StartOffset.z = 0;
		m_StartOffset.w = 0;
		m_Start.x = start.x;
		m_Start.y = start.y;
		m_Start.z = start.z;
		m_Start.w = 0;
	}
};

struct csurface_t
{
	const char	*name; // 3C
	short	surfaceProps; // 40
	unsigned short	flags; // 42
};
struct cplane_t
{
	Vector normal; // 0
	float	dist;  // C
	byte type;     // 10
	byte signbits; // signx + (signy<<1) + (signz<<1)
	byte pad[2];
	               // 14
};
class ClientEntity;
struct trace_t
{
	//CBaseTrace
	Vector startpos; // 0
	Vector endpos;   // C
	cplane_t plane;  // 18 (0x14)
	float fraction;  // 2C
	int contents;    // 30
	unsigned short disp_flags; // 34
	bool allsolid;
	bool startsolid;

	//CGameTrace
	float fraction_left_solid; // 38
	csurface_t surface; // 3C
	int hitgroup; // 44
	short bone; // 48
	unsigned short somethingidk; // 4A
	ClientEntity *hitent; // 4C
	int hitbox;

	char extra[256];
};




#endif