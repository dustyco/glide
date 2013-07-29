

#pragma once
#include "common.h"
#include "Image.h"


struct ResourceManager : NonCopyable
{
	     ResourceManager  ();
	     ~ResourceManager ();
	void submit           (Image* image);
	
private:
	struct Internal;
	
	Internal* i;
};
