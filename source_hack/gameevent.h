#ifndef GAMEEVENT_H
#define GAMEEVENT_H

class CGameEvent
{
public:
	virtual ~CGameEvent() = 0;
	virtual const char *GetName(void) = 0;
	virtual bool IsReliable(void) = 0;
	virtual bool IsLocal(void) = 0;
	virtual bool IsEmpty(char const*) = 0;
	virtual bool GetBool(char const*, bool) = 0;
	virtual int GetInt(char const*, int) = 0;
	virtual float GetFloat(char const*, float) = 0;
	virtual const char *GetString(char const*, char const*) = 0;
	virtual void SetBool(char const*, bool) = 0;
	virtual void SetInt(char const*, int) = 0;
	virtual void SetFloat(char const*, float) = 0;
	virtual void SetString(char const*, char const*) = 0;
	virtual void *GetKeyValues(void) = 0;
};


class CGameEventManager
{
	template<typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)(this))[which]);
	}
public:
	CGameEvent *CreateEvent(const char *name, bool a = false)
	{
		return get<CGameEvent *(__thiscall *)(void *, const char *, bool)>(7)(this, name, a);
	}
	void SendEvent(CGameEvent *_event)
	{
		return get<void(__thiscall *)(void *, CGameEvent *)>(9)(this, _event);
	}
};

#endif 