#ifndef LAU_H
#define LAU_H

struct lua_State;

class Lau
{
public:

	Lau(void);
	~Lau(void) { };

	lua_State *GetState(void)
	{
		return L;
	}
	int MetatableIndex(void)
	{
		return metatables;
	}

	int LoadBuffer(const char *contents, int content_length, const char *name = "=load");
	long ReadLuaFile(const char *relpath, char **output);

	// returns the amount of returns from running the file
	// or -1 on error
	int RunLuaFile(const char *relpath, bool safe = false);

	void Init(void);

private:
	lua_State *L;
	char *luadir;
	int metatables;
};

#endif