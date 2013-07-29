

#include "Edit.h"
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>


Edit::Edit ()
{
	clear();
}

void Edit::clear ()
{
	crop_left = 0;
	crop_right = 0;
	crop_bottom = 0;
	crop_top = 0;
}

void Edit::loadEdit (string path) throw(string)
{
	using namespace boost::property_tree;
	
	clear();
	try
	{
		ptree pt;
		info_parser::read_info(path, pt);
		ptree pt_e = pt.get_child("Edit");
		
		// Check this first before adding any player internal
		crop_left   = pt_e.get<float>("crop_left", 0.0f);
		crop_right  = pt_e.get<float>("crop_right", 0.0f);
		crop_bottom = pt_e.get<float>("crop_bottom", 0.0f);
		crop_top    = pt_e.get<float>("crop_top", 0.0f);
		
		// Add filters
		BOOST_FOREACH(ptree::value_type& v, pt_e) {
			if (v.first.compare("Curve") == 0)
			{
				// TODO Filters
//				ptree& pt_info = v.second;
//				PlayerInfo info(pt_info.get<PlayerID>("id", 0), pt_info.get<string>("name", ""));
//				if (info.id!=0 && !info.name.empty() && !has(info.id) && !has(info.name)) add(info);
			}
		}
	}
	catch (ptree_error& e)
	{
		clear();
		throw string("Edit::loadEdit(): ") + e.what();
	}
}

void Edit::saveEdit (string path) throw(string)
{
	/////////////////////////////////////////////////////////////////
	// Only write if there's something unique TO write like:
	// -Non-zero crop values
	// -Filters
	/////////////////////////////////////////////////////////////////
	
	using namespace boost::property_tree;
	try
	{
		ptree pt_e;
		if (crop_left   != 0) pt_e.put("crop_left",   crop_left);
		if (crop_right  != 0) pt_e.put("crop_right",  crop_right);
		if (crop_bottom != 0) pt_e.put("crop_bottom", crop_bottom);
		if (crop_top    != 0) pt_e.put("crop_top",    crop_top);
		
		// TODO Filters
		
		ptree pt;
		pt.put_child("Edit", pt_e);
		if (!pt_e.empty()) info_parser::write_info(path, pt);
	}
	catch (ptree_error& e)
	{
		clear();
		throw string("Edit::saveEdit(): ") + e.what();
	}
}

void Edit::clip ()
{
	crop_left   = min(max(crop_left,   0.0f), 1.0f-crop_right);
	crop_right  = min(max(crop_right,  0.0f), 1.0f-crop_left);
	crop_bottom = min(max(crop_bottom, 0.0f), 1.0f-crop_top);
	crop_top    = min(max(crop_top,    0.0f), 1.0f-crop_bottom);
}

void Edit::cropScale (float x, float y)
{
	float sx = 1.0f-crop_left-crop_right;
	float sy = 1.0f-crop_bottom-crop_top;
	float dx = (sx*x-sx)*0.5f;
	float dy = (sy*y-sy)*0.5f;
	crop_left -= dx;
	crop_right -= dx;
	crop_bottom -= dy;
	crop_top -= dy;
	clip();
}

void Edit::cropMove (float x, float y)
{
	float sx = 1.0f-crop_left-crop_right;
	float sy = 1.0f-crop_bottom-crop_top;
	float dx = (sx*x-sx)*0.5f;
	float dy = (sy*y-sy)*0.5f;
	crop_left += dx;
	crop_right -= dx;
	crop_bottom += dy;
	crop_top -= dy;
	clip();
}


