/*
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "kernel32.lib")
#include <TlHelp32.h>
#include <psapi.h>

extern DWORD __stdcall source_hack(void);

// not entirely mine
__forceinline const char *GetProcessName(DWORD pid)
{
	HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

	if (!process)
	{
		CloseHandle(process);
		return "";
	}
	char filename[MAX_PATH + 1] = { 0 };

	HMODULE module;
	DWORD junk;


	if (EnumProcessModules(process, &module, sizeof(module), &junk))
	{
		GetModuleBaseNameA(process, module, filename, sizeof(filename));
	}
	CloseHandle(process);

	return filename; // ignore the warning, it's inline
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define THIS_MOD ((char *)(&__ImageBase))

int main(int argc, char *argv[])
{
	const char *dllname = argv[0];
	char *part_name = argv[0] + strlen(argv[0]);
	{
		auto len = strlen(argv[0]);
		while (len--)
		{
			if (*--part_name == '/' || *part_name == '\\')
			{
				++part_name;
				break;
			}
		}
	}


	DWORD LoadAddr = (char *)(&source_hack) - THIS_MOD;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 entry;
	entry.dwSize = sizeof(entry);

	BOOL good = true;

	bool found = false;
	DWORD processid = 0;
	HANDLE thread = 0;
	HANDLE hprocess = 0;

	for (Thread32First(snap, &entry); good; good = Thread32Next(snap, &entry))
	{
		if (strcmp(GetProcessName(entry.th32OwnerProcessID), "thread_injection.exe") == 0 || strcmp(GetProcessName(entry.th32OwnerProcessID), "hl2.exe") == 0)
		{
			processid = entry.th32OwnerProcessID;
			// make sure people can't complain it's an error number from opening invalid processes and shit before
			SetLastError(0);
			printf("Thread id %i\n", entry.th32ThreadID);

			// open our process and thread so we can do things
			hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32OwnerProcessID);
			thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, entry.th32ThreadID);

			// suspend that shit so it doesn't move! if it moves kill it!
			SuspendThread(thread);

			CONTEXT context;
			CONTEXT new_ctx;

			context.ContextFlags = CONTEXT_FULL;
			GetThreadContext(thread, &context);

			memcpy(&new_ctx, &context, sizeof(CONTEXT));

			// set active code to execute
			new_ctx.Eip = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

			// fix stack for our function call
			new_ctx.Esp = new_ctx.Esp - 0x8;

			// write the return address
			WriteProcessMemory(hprocess, (LPVOID)(new_ctx.Esp),
				&context.Eip,
				sizeof(context.Eip), NULL);

			size_t dll_len = strlen(dllname);
			DWORD write = new_ctx.Esp - dll_len - 0x200;

			// write our dll name's pointer-to-be
			WriteProcessMemory(hprocess, (LPVOID)(new_ctx.Esp + 4), &write, sizeof(write), NULL);

			// write our dll name in a safe spot
			WriteProcessMemory(hprocess, (LPVOID)write, dllname, dll_len + 1, NULL);

			// set the registers in the thread
			SetThreadContext(thread, &new_ctx);


			// make the thread run again
			//ResumeThread(thread);


			// close our handles
			CloseHandle(snap);
			found = true;
			printf("Found thread and wrote loadlibrary call.. (%p)\n", new_ctx.Eip);
			break;
		}
	}
	CloseHandle(snap);

	// we found our process
	Sleep(1000);
	if (found)
	{
		found = false;
		snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processid);
#undef Module32First
#undef MODULEENTRY32
#undef Module32Next
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(entry);
		good = true;
		for (Module32First(snap, &entry); good; good = Module32Next(snap, &entry))
		{
			printf("Module %s\n", entry.szModule);
			if (strcmp(entry.szModule, part_name) == 0)
			{
				printf("module found... (loaded)\n");
				DWORD loadaddr = (DWORD)(entry.modBaseAddr) + LoadAddr;

				SuspendThread(thread);

				CONTEXT context;
				CONTEXT new_ctx;

				context.ContextFlags = CONTEXT_FULL;
				GetThreadContext(thread, &context);

				memcpy(&new_ctx, &context, sizeof(CONTEXT));

				new_ctx.Esp -= 0x8;

				// write the return address
				WriteProcessMemory(hprocess, (LPVOID)(new_ctx.Esp),
					&context.Eip,
					sizeof(context.Eip), NULL);

				new_ctx.Eip = loadaddr;

				SetThreadContext(thread, &new_ctx);


				// make the thread run again
				found = true;
			}
		}
		CloseHandle(thread);
		CloseHandle(hprocess);
	}

	if(!found) MessageBoxA(0, "I FUCKED UP PLS HELP!!", ":(", 0);
	Sleep(1000);
}

*/