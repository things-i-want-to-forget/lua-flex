#include "luawatcher.h"
#include "classes/structures.h"
#include "lau/lau.h"

namespace fileChecker {
	void checkDir()
	{
		static OVERLAPPED over;
		static HANDLE hDir = 0;
		if (!hDir)
		{
			hDir = CreateFileA(
				"C:\\aim-flex\\lua",           // pointer to the file name
				FILE_LIST_DIRECTORY,    // access (read/write) mode
				FILE_SHARE_READ         // share mode
				| FILE_SHARE_WRITE
				| FILE_SHARE_DELETE,
				NULL, // security descriptor
				OPEN_EXISTING,         // how to create
				FILE_FLAG_BACKUP_SEMANTICS // file attributes
				| FILE_FLAG_OVERLAPPED,
				NULL);
		}
		char buffer[1024];
		if (ReadDirectoryChangesW(hDir, &buffer, sizeof(buffer), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &over, NULL)) {
			DWORD bytes = 0;
			GetOverlappedResult(hDir, &over, &bytes, FALSE);
			ResetEvent(over.hEvent);
			if (bytes > 0) 
			{
				int offset = 0;
				FILE_NOTIFY_INFORMATION *notify = 0;
				do
				{
					notify = (FILE_NOTIFY_INFORMATION*)(buffer + offset);
					offset += notify->NextEntryOffset;
					if (notify->FileNameLength > 0) {
						structs.L->RunLuaFile("init.lua");
						break;
					}
				} while (notify && notify->NextEntryOffset != 0);
			}
		}
		memset(&over, 0, sizeof(over));
		ResetEvent(over.hEvent);
		CancelIo(hDir);
	}
}