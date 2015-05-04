#ifndef CLIENTTOOLS_H
#define CLIENTTOOLS_H

class CClientTools
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}
public:
	bool IsWeapon(void *w)
	{
		return get<bool(__thiscall *)(void *, void *)>(46)(this, w);
	}
};

#endif