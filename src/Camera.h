

#pragma once
#include "common.h"
#include "Frame.h"
#include <mutex>


// TODO Needs to be set by something
struct Camera : Frame, NonCopyable, mutex
{
	Vec2i dim;
	
	static Camera& getSingleton ();
};
