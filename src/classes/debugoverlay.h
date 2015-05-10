#ifndef DEBUGOVERLAY_H
#define DEBUGOVERLAY_H

#include "vector.h"
#include "engines.h"

extern EngineVersion version;

class CIVDebugOverlay
{
	template <typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:
	inline int ScreenPosition(Vector &point, Vector &screen)
	{
		return get<int(__thiscall *)(void *, Vector &, Vector &)>(version == CSGO ? 11 : 10)(this, point, screen);
	}
};

#endif // DEBUGOVERLAY_H