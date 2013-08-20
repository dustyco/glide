

#include "Layout.h"
#include "Camera.h"
#include "Controls.h"
#include <cmath>


Layout::Layout ()
{
	t = 0;
	mode = HORIZONTAL;
	lane_count = 5;
}

void Layout::add (vector<ImagePtr>& images)
{
	// TODO
	this->images = images;
	
	// Must keep images sorted by name
//	sort(images.begin(), images.end(), Image::compareName);
	
	arrange();
}

void Layout::tick (float dt)
{
	Controls& controls = Controls::getSingleton();
	Camera& camera = Camera::getSingleton();
	
	int temp_lane_count = lane_count;
	if (controls.buttons["lanes1"].pressed) lane_count = 1;
	if (controls.buttons["lanes2"].pressed) lane_count = 2;
	if (controls.buttons["lanes3"].pressed) lane_count = 3;
	if (controls.buttons["lanes4"].pressed) lane_count = 4;
	if (controls.buttons["lanes5"].pressed) lane_count = 5;
	if (controls.buttons["lanes6"].pressed) lane_count = 6;
	if (controls.buttons["lanes7"].pressed) lane_count = 7;
	if (controls.buttons["lanes8"].pressed) lane_count = 8;
	if (controls.buttons["lanes9"].pressed) lane_count = 9;
	if (temp_lane_count != lane_count) arrange();
	
	t += dt;
//	arrange();
	
	// Move images
	for (ImagePtr& image_ptr : images)
	{
		bool should_submit = false;
		{
			lock_guard<mutex> l(*image_ptr);
			image_ptr->tween(dt);
			bool visible_last = image_ptr->visible;
			bool visible_now = image_ptr->visible = touching(image_ptr->target, camera.target);
			if (visible_now || (visible_last != visible_now)) should_submit = true;
		}
		if (should_submit) res_man.submit(image_ptr);
	}
	
	// Update camera
	camera.target.rx = 0.5*camera.dim.x/camera.dim.y;
	camera.target.ry = 0.5;
	
	camera.target.p = Vec2f(cos(t/3)+1+camera.target.rx, 0.5);
//	camera.target.p = Vec2f::ORIGIN;
	
	camera.tween(dt);
	
}

void Layout::arrange ()
{
	// Pack images into lanes with the requested lane_count and mode
	// Assume images are sorted by name
	lanes.clear();
	lanes.resize(lane_count);
	vector<float> ends(lane_count, 0);
	for (ImagePtr& image_ptr : images)
	{
		// See which lane is the shortest
		int shortest = min_element(ends.begin(), ends.end()) - ends.begin();
		
		// Add the image to that lane
		lanes[shortest].push_back(image_ptr);
		lock_guard<mutex> l(*image_ptr);
		float aspect = float(image_ptr->dim_full.x) / image_ptr->dim_full.y;
		if (mode == HORIZONTAL)
		{
			// Set the Image's new target position and size
			float width = aspect/lane_count;
			image_ptr->target.p.y = (0.5 + shortest)/lane_count;
			image_ptr->target.p.x = ends[shortest] + width*0.5;
			image_ptr->target.ry = 0.5/lane_count;
			image_ptr->target.rx = width*0.5;
		
			// Move the lane's end
			ends[shortest] += width;
		}
		else if (mode == VERTICAL)
		{
			
		}
	}
	
	// Move Images
/*	int i = 0;
	int dim = sqrt(images.size());
	for (int y=0; y!=dim; ++y)
	for (int x=0; x!=dim; ++x)
	{
		bool do_resource = false;
		ImagePtr image_ptr = images[y*dim + x];
		{
			Image& image = *image_ptr;
			unique_lock<mutex> l(image);
			
//			image.target.p = Vec2f::ORIGIN + Vec2f(cos(t)/5, sin(t)/5+float(i)/4-0.5);
//			image.target.rx = (sin(-t/5)/2+0.5)*1.0*image.dim_full.x/image.dim_full.y;
//			image.target.ry = (sin(-t/5)/2+0.5)*1;
			
			image.target.p = Vec2f(-1.0 + 2.0*x/dim + 1.0/dim, -1.0 + 2.0*y/dim + 1.0/dim);
			image.target.rx = 1.0*image.dim_full.x/image.dim_full.y/dim;
			image.target.ry = 1.0/dim;
			
			bool visible_now = touching(camera.target, image.target);
			if (visible_now || visible_now!=image.visible) do_resource = true;
			image.visible = visible_now;
			
			image.tween(1);
		}
		
		if (do_resource) res_man.submit(image_ptr);
		++i;
	}
*/	
	
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

void Layout::getVisible (vector<ImagePtr>& images)
{
	// TODO Culling
	images = this->images;
}


