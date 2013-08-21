

#include "Layout.h"
#include "Camera.h"
#include "Controls.h"
#include <cmath>


Layout::Layout ()
{
	t = 0;
	mode = HORIZONTAL;
	lane_count = 5;
	select_set = false;
	enlarged = false;
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
	
	// Handle input
	if (!images.empty())
	{
		// Change lane count
		int temp_lane_count = lane_count;
		if (controls.buttons["lanes1"].is_pressed) lane_count = 1;
		if (controls.buttons["lanes2"].is_pressed) lane_count = 2;
		if (controls.buttons["lanes3"].is_pressed) lane_count = 3;
		if (controls.buttons["lanes4"].is_pressed) lane_count = 4;
		if (controls.buttons["lanes5"].is_pressed) lane_count = 5;
		if (controls.buttons["lanes6"].is_pressed) lane_count = 6;
		if (controls.buttons["lanes7"].is_pressed) lane_count = 7;
		if (controls.buttons["lanes8"].is_pressed) lane_count = 8;
		if (controls.buttons["lanes9"].is_pressed) lane_count = 9;
		if (temp_lane_count != lane_count) arrange();
		
		// Init selection if there isn't one
		selectImage();
		
		// Move selection
		if (!enlarged)
		{
			if (mode == HORIZONTAL)
			{
				for (int i=controls.buttons["up"].getRepeats(); i>0; --i)
				if (select_lane > 0)
				{
					// Move to lane above and image with nearest x position
				//	--select_lane;
				}
			
				for (int i=controls.buttons["down"].getRepeats(); i>0; --i)
				if (select_lane < lanes.size()-1)
				{
					// Move to lane below and image with nearest x position
				//	--select_lane;
				}
			
				for (int i=controls.buttons["left"].getRepeats(); i>0; --i)
				if (select_offset > 0) --select_offset;
			
				for (int i=controls.buttons["right"].getRepeats(); i>0; --i)
				if (select_offset < lanes[select_lane].size()-1) ++select_offset;
			
				// Set the ImagePtr for when it changes
				selectImage();
			}
			else if (mode == VERTICAL)
			{
			
			}
		}
		
		// Handle if the image is enlarged
		if (controls.buttons["select"].getRepeats()%2 == 1) enlarged = !enlarged;
		if (enlarged)
		{
			Image& image = *select_image_ptr;
			lock_guard<mutex> l(image);
			
			// TODO Handle case when mode is vertical
			image.target.p.x = select_image_original_rect.p.x;
			image.target.p.y = 0.5;
			
			// TODO Implement getAspect() member functions for Image and Camera
			// TODO Handle when image is wider than camera (image_aspect > camera_aspect)
			float image_aspect = float(image.dim_full.x)/image.dim_full.y;
			float camera_aspect = float(camera.dim.x)/camera.dim.y;
			image.target.rx = 0.5*image_aspect;
			image.target.ry = 0.5;
		}
		else
		{
			select_image_ptr->target = select_image_original_rect;
		}
		select_frame.target = select_image_ptr->target;
	}
	
	t += dt;
	
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
	
	// Tween selection rectangle
	select_frame.tween(dt);
	
	// Camera size and tween
	camera.target.rx = 0.5*camera.dim.x/camera.dim.y;
	camera.target.ry = 0.5;
	camera.tween(dt);
	
}

// Pack images into lanes with the requested lane_count and mode
// Assume images are sorted by name
void Layout::arrange ()
{
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
			
			// Reassign selection indices if this is it
			if (select_set && image_ptr == select_image_ptr)
			{
				select_lane = shortest;
				select_offset = lanes[shortest].size()-1;
				selectImage(false);
			}
			
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

// TODO Make this thread safe
void Layout::selectImage (bool reset_current_rect)
{
	if (!images.empty())
	{
		if (!select_set)
		{
			select_set = true;
			select_lane = 0;
			select_offset = 0;
		}
		else if (reset_current_rect)
		{
			// Reset the current selected Image's position if there was one in case it's enlarged
			select_image_ptr->target = select_image_original_rect;
		}
		select_image_ptr = lanes[select_lane][select_offset];
		select_image_original_rect = select_image_ptr->target;
			
		// Move camera
		Camera::getSingleton().target.p = Vec2f(select_image_original_rect.p.x, 0.5);
	}
}


