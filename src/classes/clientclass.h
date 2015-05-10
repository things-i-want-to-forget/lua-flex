#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

class RecvProp;
class RecvTable;

class RecvProp
{
public:
	const char *name;
	int type;
	int flags;
	int stringsize;
	bool isarraymember;
	void *extradata;
	RecvProp *arrayprop;
	void *fns[3];
	RecvTable *datatable;
	int offset;
	void *rest[3];
};

class RecvTable
{
public:
	RecvProp *props;
	int nprops;
	void *something;
	char *name;
	bool something2, idontcare;
};

class ClientClass
{

public:
	void *whocares, *idont;
	char		*name;
	RecvTable	*table;
	ClientClass	*next;
	int			classid;
};

#endif