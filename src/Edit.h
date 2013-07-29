

#pragma once
#include "common.h"


struct Edit
{
	float version;
	float crop_left;
	float crop_right;
	float crop_bottom;
	float crop_top;
	
	     Edit      ();
	void clear     ();
	void loadEdit  (string path) throw(string);
	void saveEdit  (string path) throw(string);
	void clip      ();
	void cropScale (float x, float y);
	void cropMove  (float x, float y);
};
