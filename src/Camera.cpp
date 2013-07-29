

#include "Camera.h"


Camera& Camera::getSingleton ()
{
	// Guaranteed to be destroyed.
	// Instantiated on first use.
	static Camera instance;
	return instance;
}


