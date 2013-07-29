

#pragma once
#include "common.h"
#include "Frame.h"
#include "Mutex.h"


// TODO Needs to be set by something
struct Camera : Frame, Mutex, NonCopyable
{
	Vec2i dim;
	
	static Camera& getSingleton ();
};
