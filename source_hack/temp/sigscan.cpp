#define WIN32_LEAN_AND_MEAN
#define VS_EXTRA_LEAN
#include <Windows.h>

void *SigScan(const char *sig, void *_base)
{
	char *base = (char *)_base;
	while (!IsBadCodePtr((FARPROC)base))
	{
		const char *cursig = sig;
		char *curbase = base;
		while (!IsBadCodePtr((FARPROC)curbase))
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