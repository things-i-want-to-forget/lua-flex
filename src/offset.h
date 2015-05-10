#ifndef OFFSET_H
#define OFFSET_H

#ifdef offsetof // _WIN32|_WIN64
#undef offsetof
#endif

#define offsetof(clas, member) ((unsigned long long)&(((clas *)0)->member))
#define _offset(member) ((unsigned long long)&(((decltype(this))0)->member))

#endif