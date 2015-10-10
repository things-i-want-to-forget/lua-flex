
bool SigScanCompare(const char *sig, void *_addr)
{

	const char *addr = (const char *)_addr;
	while (1)
	{
		if (*sig == 0)
			return true;
		if (*sig != '?' && *addr != *sig)
			break;
		addr++;
		sig++;
	}

	return false;

}

void *SigScan(const char *sig, void *_base)
{
	char *base = (char *)_base;
	while (1)
	{
		const char *cursig = sig;
		char *curbase = base;
		while (1)
		{
			if (*cursig == 0)
				return base;
			if (*cursig != '?' && *cursig != *curbase)
				break;
			cursig++;
			curbase++;
		}

		base++;
	}
	return 0;
}