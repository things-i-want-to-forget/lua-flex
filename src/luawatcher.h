#ifndef LUA_WATCHER
#define LUA_WATCHER
#include <Windows.h>

class CFileWatcher
{

private:

	HANDLE handle;
	OVERLAPPED overlapped;

public:
	CFileWatcher(const char *file);
	bool Query(void);

};


#endif