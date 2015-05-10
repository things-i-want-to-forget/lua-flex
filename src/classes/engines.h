#ifndef ENGINES_H
#define ENGINES_H

enum EngineVersion
{
	GARRYSMOD	= 0, // garry mode has own version
	HL2			= 1, // css, tf2, 
	CSGO		= 2  // csgo, alienswarm,
};

extern int GetAppID(void);
extern EngineVersion GetEngineVersion(int appid);

#endif