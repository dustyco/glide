

#pragma once
#include "common.h"
#include "Image.h"
#include "ResourceManager.h"
#include <vector>


struct Layout
{
	struct Lane
	{
		list<Image*> images;
	};
	
	ResourceManager res_man;
	vector<Image*>  images;
	vector<Lane>    lanes;
	bool            vertical;
	float           t;
	
	void add        (vector<Image*>& images);
	void tick       (float dt);
	void arrange    ();
	void getVisible (vector<Image*>& images);
	void calculate  (bool vertical, int lanes);
};


