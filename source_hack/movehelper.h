#ifndef MOVEHELPER_H
#define MOVEHELPER_H

//do we need this?
class CMoveHelper
{
	template <typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
public:

};

#endif // MOVEHELPER_H