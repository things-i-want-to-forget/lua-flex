#ifndef LAU_H
#define LAU_H

struct lua_State;

class Lau
{
public:

	Lau(void);
	~Lau(void) { };

	lua_State *GetState()
	{
		return L;
	}

	int LoadBuffer(const char *contents, int content_length, const char *name = "=load");
	long ReadLuaFile(const char *relpath, char **output);

	// returns the amount of returns from running the file
	// or -1 on error
	int RunLuaFile(const char *relpath);

	void Init(void);

private:
	lua_State *L;
	char *luadir;
};

#endif