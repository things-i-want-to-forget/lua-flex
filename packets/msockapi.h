#ifndef MSOCKAPI_H
#define MSOCKAPI_H

#include <Windows.h>

class MSockAPI
{
public:
	MSockAPI() { _connected = false; };

	void Connect(const char *pipename)
	{


#ifdef AIM_FLEX_OVERLAY
		//server
		SECURITY_ATTRIBUTES attr;
		SECURITY_DESCRIPTOR desc;
		InitializeSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);

		SetSecurityDescriptorDacl(&desc, TRUE, NULL, FALSE);

		attr.lpSecurityDescriptor = &desc;
		attr.nLength = sizeof(attr);
		attr.bInheritHandle = FALSE;

		pipe = CreateNamedPipeA(pipename, PIPE_ACCESS_INBOUND | WRITE_DAC,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT, PIPE_UNLIMITED_INSTANCES, 512, 512,
			NMPWAIT_USE_DEFAULT_WAIT, &attr);
		_connected = pipe != INVALID_HANDLE_VALUE;
#else
		//client
		pipe = CreateFileA(pipename, GENERIC_WRITE, 0,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		_connected = pipe != INVALID_HANDLE_VALUE;

#endif
	}

	bool connected(void)
	{
		return _connected;
	}

	template <typename t>
	bool read(t *val, unsigned long len = sizeof(t))
	{
		DWORD read;
		DWORD shit;
		PeekNamedPipe(pipe, val, len, &read, &shit, &shit);
		if (read == len)
		{
			ReadFile(pipe, val, len, &read, NULL);
			return true;
		}
		return false;
	}

	template <typename t>
	void write(t &val, unsigned long len = sizeof(t))
	{
		DWORD written;
		WriteFile(pipe, &val, len, &written, NULL);
	}

public:

	bool _connected;
	HANDLE pipe;

};

#endif 