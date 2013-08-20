

#pragma once
#include "common.h"


struct Frame
{
	Rect current;
	Rect target;
	
	void snap     ();
	void tween    (float dt);
	bool overlaps (Frame& frame);
};
