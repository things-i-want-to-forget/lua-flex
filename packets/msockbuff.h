#ifndef MSOCKBUFF_H
#define MSOCKBUFF_H

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
	MSockBuffer(void)
	{
		buf = 0;
		size = 0;
		state = MSOCKBUFF_READNONE;
	}
	~MSockBuffer(void)
	{
		Reset();
	}
	bool Initialize(MSockAPI *api);

	void Reset(void)
	{
		if (buf)
			delete[] buf;
		buf = 0;
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
		memcpy(var, buf, varlen);
		buf += varlen;
		size -= varlen;
		return true;
	}

	unsigned long sizeleft(void)
	{
		return size;
	}

	virtual unsigned long process(void) = 0;

public:
	char *buf;

	unsigned long size;
	
	unsigned long id;

	unsigned char state;
};

#endif MSOCKBUFF_H