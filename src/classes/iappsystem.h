#ifndef IAPPSYSTEM_H
#define IAPPSYSTEM_H

class IAppSystem
{
public:
	virtual void *Connect(void * (*)(char  const*, int *)) = 0;
	virtual void *Disconnect(void) = 0;
	virtual void *QueryInterface(char  const*) = 0;
	virtual void *Init(void) = 0;
	virtual void *Shutdown(void) = 0;
	virtual void *GetDependencies(void) = 0;
	virtual void *GetTier(void) = 0;
	virtual void *Reconnect(void * (*)(char  const*, int *), char  const*) = 0;
	virtual void *unk1(void) = 0;
};

#endif // IAPPSYSTEM_H