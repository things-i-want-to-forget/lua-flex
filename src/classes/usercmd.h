#ifndef USERCMD_H
#define USERCMD_H

#include "angle.h"
#include "engines.h"
#include "../offset.h"

extern EngineVersion version;

class CUserCmd
{
	virtual ~CUserCmd(void) { };

public:
	int command_number; // 4
	int tickcount; // 8
	QAngle angles; // 12
	int &buttons(void)
	{
		return *(int *)((version >= CSGO ? 12 : 0) + _offset(buttons_) + (char *)this);
	}
	short &mousex(void)
	{
		return *(short *)((version >= CSGO ? 12 : 0) + _offset(mousex_) + (char *)this);
	}
	short &mousey(void)
	{
		return *(short *)((version >= CSGO ? 12 : 0) + _offset(mousey_) + (char *)this);
	}
	float &sidemove(void)
	{
		return *(float*)((version >= CSGO ? 12 : 0) + _offset(sidemove_) + (char *)this);
	}
	float &forwardmove(void)
	{
		return *(float*)((version >= CSGO ? 12 : 0) + _offset(forwardmove_) + (char *)this);
	}
	Vector &movement(void)
	{
		return *(Vector *)((version >= CSGO ? 12 : 0) + _offset(forwardmove_) + (char *)this);
	}
private:
	// IF ENGINE >= CSGO THEN ADD 12 FOR AIMDIRECTION
	float forwardmove_; // 24
	float sidemove_; // 28
	float upmove_; // 32
	int buttons_; // 36
	unsigned char impulse_; // 40
	int weaponselect_; // 44
	int weaponsubtype_; // 48
	int random_seed_; // 52
	short mousex_; // 56
	short mousey_; // 58
	bool predicted_; // 60
};


#endif // USERCMD_H