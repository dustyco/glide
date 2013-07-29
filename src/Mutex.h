

#pragma once
#include "common.h"


struct Mutex : NonCopyable
{
	struct Internal;
	
	Internal* i;
	
	     Mutex   ();
	     ~Mutex  ();
	bool tryLock ();
	void lock    ();
	void unlock  ();
};
