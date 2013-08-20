

#pragma once
#include "common.h"
#include "Image.h"


struct ResourceManager : NonCopyable
{
	     ResourceManager  ();
	     ~ResourceManager ();
	void submit           (ImagePtr image);
	
private:
	struct Internal;
	
	Internal* i;
};
