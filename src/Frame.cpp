

#include "Frame.h"


void Frame::snap ()
{
	current = target;
}

void Frame::tween (float dt)
{
	// TODO
	float k = 10.0;
	current.p += (target.p - current.p)*dt*k;
	current.rx += (target.rx - current.rx)*dt*k;
	current.ry += (target.ry - current.ry)*dt*k;
}

bool Frame::overlaps (Frame& frame)
{
	return touching(current, frame.current);
}
