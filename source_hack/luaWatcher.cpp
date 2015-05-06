#include "luaWatcher.h"

namespace fileChecker {
	OVERLAPPED over;
	HANDLE hDir = CreateFile(
		"C:\\aim-flex\\lua",           // pointer to the file name
		FILE_LIST_DIRECTORY,    // access (read/write) mode
		FILE_SHARE_READ         // share mode
		| FILE_SHARE_WRITE
		| FILE_SHARE_DELETE,
		NULL, // security descriptor
		OPEN_EXISTING,         // how to create
		FILE_FLAG_BACKUP_SEMANTICS // file attributes
		| FILE_FLAG_OVERLAPPED,
		NULL);                 // file with attributes to copy
	void checkDir() {
		char buffer[1024];
		if (ReadDirectoryChangesW(hDir, &buffer, sizeof(buffer), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &over, NULL)) {
			DWORD bytes = 0;
			GetOverlappedResult(hDir, &over, &bytes, FALSE);
			ResetEvent(over.hEvent);
			if (bytes > 0) {
				int offset = 0;
				for (;;) {
					FILE_NOTIFY_INFORMATION* notify = (FILE_NOTIFY_INFORMATION*)&buffer[offset];
					offset += notify->NextEntryOffset;
					if (notify->FileNameLength > 0) {
						size_t fileNameLengthInWChars = notify->FileNameLength / sizeof(WCHAR); //get the length of the filename in characters
						WCHAR *pwNullTerminatedFileName = new WCHAR[fileNameLengthInWChars + 1]; //allocate an intermediate buffer to hold a null-terminated version of fileInfo.FileName; +1 for null terminator
						wcsncpy(pwNullTerminatedFileName, notify->FileName, fileNameLengthInWChars); //copy the filename into a the intermediate buffer
						pwNullTerminatedFileName[fileNameLengthInWChars] = L'\0'; //null terminate the new buffer
						size_t fileNameLengthInChars = wcstombs(NULL, pwNullTerminatedFileName, 0); //first call to wcstombs() determines how long the output buffer needs to be
						char *pFileName = new char[fileNameLengthInChars + 1]; //allocate the final output buffer; +1 to leave room for null terminator
						wcstombs(pFileName, pwNullTerminatedFileName, fileNameLengthInChars + 1); //finally do the conversion!
						
						structs.L->RunLuaFile(pFileName);
					}
					if (notify->NextEntryOffset == 0) {
						break;
					}
				}
			}
		}
		memset(&over, 0, sizeof(over));
		ResetEvent(over.hEvent);
		CancelIo(hDir);
	}
}