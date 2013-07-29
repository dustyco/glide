

#include "Layout.h"
#include "Camera.h"
#include <cmath>


void Layout::add (vector<Image*>& images)
{
	// TODO
	this->images = images;
	arrange();
}

void Layout::tick (float dt)
{
	// TODO
	t += dt;
	arrange();
}

void Layout::arrange ()
{
	// Update camera
	Camera& camera = Camera::getSingleton();
	camera.target.p = Vec2f::ORIGIN;
	camera.target.rx = 1.0*camera.dim.x/camera.dim.y;
	camera.target.ry = 1;
	camera.tween(1);
	
	// Move Images
	int i = 0;
	for (vector<Image*>::iterator it=images.begin(); it!=images.end(); ++it)
	{
		Image& image = *(*it);
		image.lock();
		image.visible = true;
		image.target.p = Vec2f::ORIGIN + Vec2f(cos(t)/5, sin(t)/5+float(i)/4-0.5);;
		image.target.rx = (sin(t/5)/2+0.5)*1.0*image.dim_full.x/image.dim_full.y;
		image.target.ry = (sin(t/5)/2+0.5)*1;
		image.tween(1);
		image.unlock();
		
		res_man.submit(*it);
		++i;
	}
}

void Layout::getVisible (vector<Image*>& images)
{
	// TODO Culling
	images = this->images;
}

/*
	// TODO Use this to update bool Image::visible
	// Test for visibility
	if (touching(Camera::getSingleton().getCurrent(), current)
	{
		// It's visible
	}
*/


