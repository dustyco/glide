

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
	int dim = sqrt(images.size());
	for (int y=0; y!=dim; ++y)
	for (int x=0; x!=dim; ++x)
	{
		Image* image_ptr = images[y*dim + x];
		{
			Image& image = *image_ptr;
			unique_lock<mutex> l(image);
			image.visible = true;
			
//			image.target.p = Vec2f::ORIGIN + Vec2f(cos(t)/5, sin(t)/5+float(i)/4-0.5);
//			image.target.rx = (sin(-t/5)/2+0.5)*1.0*image.dim_full.x/image.dim_full.y;
//			image.target.ry = (sin(-t/5)/2+0.5)*1;
			
			image.target.p = Vec2f(-1.0 + 2.0*x/dim + 1.0/dim, -1.0 + 2.0*y/dim + 1.0/dim);
			image.target.rx = 1.0*image.dim_full.x/image.dim_full.y/dim;
			image.target.ry = 1.0/dim;
			
			image.tween(1);
		}
		
		res_man.submit(image_ptr);
		++i;
	}
	
	
/*	int i = 0;
	for (Image* image_ptr : images)
	{
		{
			// Lock while we edit it
			Image& image = *image_ptr;
			unique_lock<mutex> l(image);
			image.visible = true;
			image.target.p = Vec2f::ORIGIN + Vec2f(cos(t)/5, sin(t)/5+float(i)/4-0.5);;
			image.target.rx = (sin(t/5)/2+0.5)*1.0*image.dim_full.x/image.dim_full.y;
			image.target.ry = (sin(t/5)/2+0.5)*1;
			image.tween(1);
		}
		
		res_man.submit(image_ptr);
		++i;
	}
*/
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


