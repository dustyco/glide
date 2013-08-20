

#pragma once
#include "common.h"
#include "Image.h"
#include "ResourceManager.h"
#include <vector>


struct Layout
{
	enum Mode { HORIZONTAL, VERTICAL };
	
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
	
	void arrange    ();
	void calculate  (bool vertical, int lanes);
};


