#ifndef CCLIENT_H
#define CCLIENT_H

class ClientClass;

class CClient
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}

public:

	ClientClass *GetAllClasses(void)
	{

		return get<ClientClass *(__thiscall *)(void *)>(8)(this);

	}

};

#endif