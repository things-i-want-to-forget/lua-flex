#ifndef LUA_WATCHER
#define LUA_WATCHER
#include <Windows.h>

typedef void(*NotifyFileChanged_t)();

class CFileWatcher
{

private:

	HANDLE handle;
	OVERLAPPED overlapped;
	NotifyFileChanged_t changed;

public:
	CFileWatcher(const char *file, NotifyFileChanged_t changed);
	bool Query(void)

};


#endif