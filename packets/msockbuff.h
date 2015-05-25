#ifndef MSOCKBUFF_H
#define MSOCKBUFF_H

#include <cstdlib>

enum MPACKET
{
	MPACKET_LINE           = 0,
	MPACKET_OUTLINED_RECT  = 1,
	MPACKET_FILLED_RECT    = 2,
	MPACKET_FLUSH          = 3,
	MPACKET_CLEAR          = 4,
	MPACKET_COLOR          = 5
};

enum
{
	MSOCKBUFF_READNONE    = 0,
	MSOCKBUFF_READID      = 1,
	MSOCKBUFF_READLENGTH  = 2,
	MSOCKBUFF_READALL     = 3
};

class MSockAPI;

class MSockBuffer
{
public:
	// input
	MSockBuffer(void)
	{
		size = 0;
		state = MSOCKBUFF_READNONE;
	}

	MSockBuffer(MPACKET pid)
	{
		size = 0;
		id = pid;
	}

	~MSockBuffer(void)
	{
		Reset();
	}
	bool Initialize(MSockAPI *api);

	template<typename t>
	void write(t val, unsigned long len = sizeof(t))
	{
		(*(t *)(buf + size)) = val;
		size += len;
	}

	void Reset(void)
	{
		cbuf = 0;
		state = MSOCKBUFF_READNONE;
		size = 0;
	}

	template <typename t>
	bool read(t *var, unsigned long varlen = sizeof(t))
	{
		if (size < varlen)
		{
			return false;
		}
		memcpy(var, cbuf, varlen);
		cbuf += varlen;
		size -= varlen;
		return true;
	}

	unsigned long sizeleft(void)
	{
		return size;
	}

public:
	static char buf[4096];
	char *cbuf;

	unsigned long size;
	
	unsigned long id;

	unsigned char state;
};

#endif MSOCKBUFF_H