#ifndef GAMEMOVEMENT_H
#define GAMEMOVEMENT_H

class ClientEntity;

class CGameMovement
{
	template <typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:
	void ProcessMovement(ClientEntity *ent, void *move)
	{
		return get<void(__thiscall *)(void *, ClientEntity *, void*)>(1)(this, ent, move);
	}
};

#endif // GAMEMOVEMENT_H