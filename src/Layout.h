

#pragma once
#include "common.h"
#include "Image.h"
#include "ResourceManager.h"
#include <vector>


struct Layout
{
	enum Mode { HORIZONTAL, VERTICAL };
	
	bool                      enlarged;
	ImagePtr                  select_image_ptr;
	Frame                     select_frame;
	
	     Layout     ();
	void add        (vector<ImagePtr>& images);
	void tick       (float dt);
	void getVisible (vector<ImagePtr>& images);

private:
	ResourceManager           res_man;
	vector<ImagePtr>          images;
	vector<vector<ImagePtr> > lanes;
	float                     t;
	Mode                      mode;
	int                       lane_count;
	bool                      select_set;
	int                       select_lane, select_offset;
	Rect                      select_image_original_rect;
	
	void arrange     ();
	void calculate   (bool vertical, int lanes);
	void selectImage (bool reset_current_rect = true);
};


