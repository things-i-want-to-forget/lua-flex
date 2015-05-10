#ifndef CLIENTENTITYLIST_H
#define CLIENTENTITYLIST_H
typedef unsigned long CBaseHandle;

class ClientEntity;
class ClientNetworkable;

class CClientEntityList
{
public:
	virtual ClientNetworkable *GetClientNetworkable(int) = 0;
	virtual void *GetClientNetworkableFromHandle(CBaseHandle) = 0;
	virtual void *GetClientUnknownFromHandle(CBaseHandle) = 0;
	virtual ClientEntity *GetClientEntity(int) = 0;
	virtual ClientEntity *GetClientEntityFromHandle(CBaseHandle) = 0;
	virtual void *NumberOfEntities(bool) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void *SetMaxEntities(int) = 0;
	virtual void *GetMaxEntities(void) = 0;
};
#endif