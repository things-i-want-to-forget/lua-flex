#include "luawatcher.h"
#include "classes/structures.h"
#include "lau/lau.h"
#include <Windows.h>

CFileWatcher::CFileWatcher(const char *dir, NotifyFileChanged_t changed)
{
	handle = CreateFileA(
		dir,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);

	this->changed = changed;
}

bool CFileWatcher::Query(void)
{
	char buffer[1024];
	if (ReadDirectoryChangesW(handle, &buffer, sizeof(buffer), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &overlapped, NULL))
	{
		
		DWORD bytes = 0;
		GetOverlappedResult(handle, &overlapped, &bytes, FALSE);
		ResetEvent(overlapped.hEvent);
		if (bytes <= 0)
			return;

		int offset = 0;
		FILE_NOTIFY_INFORMATION *notify = 0;
		do
		{
			notify = (FILE_NOTIFY_INFORMATION *)&buffer[offset];
			offset += notify->NextEntryOffset;
			if (notify->FileNameLength > 0) 
			{
				changed();
			}
		} while (notify && notify->NextEntryOffset != 0);
	}
	memset(&overlapped, 0, sizeof(overlapped));

	ResetEvent(overlapped.hEvent);
	CancelIo(handle);
}