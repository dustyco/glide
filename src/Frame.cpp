

#include "Frame.h"


void Frame::tween (float dt)
{
	// TODO
	current = target;
}

bool Frame::overlaps (Frame& frame)
{
	return touching(current, frame.current);
}
