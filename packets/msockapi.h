#ifndef MSOCKAPI_H
#define MSOCKAPI_H

#include <Windows.h>

class MSockAPI
{
public:
	MSockAPI() { _connected = false; };

	int Connect(const char *pipe)
	{
	}

	bool connected(void)
	{
		return _connected;
	}

	template <typename t>
	bool read(t *val, unsigned long len = sizeof(t))
	{
		return false;
	}

public:

	bool _connected;


};

#endif 