#include <Windows.h>

bool GoodPtr(void *ptr)
{
	return !IsBadCodePtr((FARPROC)ptr);
}