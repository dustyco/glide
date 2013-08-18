

#include "Image.h"
#include "Camera.h"
#include <dirent.h>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <SFML/Graphics/Texture.hpp>


Image::Image (string path, Format format, Vec2i dim_full)
{
	this->path = path;
	this->format = format;
	this->dim_full = dim_full;
	
	loadEdit();
	
	loaded = false;
	loading = false;
	failed = false;
	pixels = 0;
	dim_loaded = Vec2i::ORIGIN;
	jpeg_scale = 8;
	
	texture = NULL;
	
	visible = false;
}

Image::~Image ()
{
	unload();
	
	delete (sf::Texture*)texture;
}

// Calls Edit::loadEdit(string path)
void Image::loadEdit ()
{
	typedef boost::filesystem::path BoostPath;
	BoostPath bp(path);
	BoostPath parent_path = bp.parent_path();
	BoostPath filename = bp.filename();
	BoostPath edit_path = parent_path += BoostPath("/.") += filename += BoostPath(".edit");
	
	try { ((Edit*)this)->loadEdit(edit_path.native()); }
	catch (string s) {}
}

// Calls Edit::saveEdit(string path)
void Image::saveEdit ()
{
	typedef boost::filesystem::path BoostPath;
	BoostPath bp(path);
	BoostPath parent_path = bp.parent_path();
	BoostPath filename = bp.filename();
	BoostPath edit_path = parent_path += BoostPath("/.") += filename += BoostPath(".edit");
	
	try { ((Edit*)this)->saveEdit(edit_path.native()); }
	catch (string s) {}
}

void Image::createImages (vector<Image*>& images, string path) throw(string)
{
	destroyImages(images);
	
	DIR*    dir = opendir(path.c_str());
	dirent* dir_entry;
	if (dir == NULL) throw string("Image::createImages: Failed to open path");
	
	for (dir_entry=readdir(dir); dir_entry!=NULL; dir_entry=readdir(dir))
	{
		switch (dir_entry->d_type)
		{
			case DT_UNKNOWN: break;
			case DT_LNK: break;
			case DT_REG:
				string file_name = dir_entry->d_name;
				string file_path = path + "/" + file_name;
				
				// Skip blank or hidden
				if (file_name.empty() || (file_name[0] == '.')) break;
				
				// Try to get header info
				int w, h;
				Format format;
				try { readFileInfo(file_path, format, w, h); }
				catch (string s) { break; }
				
				// TODO We only support JPEG right now
				if (format!=JPEG) break;
				
				// Add it
				Image* image = new Image(file_path, format, Vec2i(w, h));
				images.push_back(image);
				break;
		}
	}
    if (dir) closedir(dir);
    
    // Sort by name
	sort(images.begin(), images.end(), CompareName());
}

void Image::destroyImages (vector<Image*>& images)
{
	for (std::vector<Image*>::iterator it=images.begin(); it!=images.end(); ++it)
		delete *it;
}

// Considers the difference between cooresponding
// dimensions of what an image is and what you want
// it to be and returns N
//
// scale = N/8
// current*scale >= desired
//
// Where N is an integer 1 to 8 (inclusive) and as low as possible
//
// The purpose of this is to give the JPEG decoder a scaling factor
// so it can skip unnecessary high frequency DCT data and produce as
// small an image as it can which is still at least the size we need
//
int Image::getJpegDecodeScale (int current, int desired)
{
	// Shouldn't be scaled down
//	if (current<=desired) return 8;
	
	// Faster but doesn't provide exact results
//	int N = desired*8/current+1;
//	return (N<1)?(1):(N);
	
	// Use this if we want the result to be greater than desired
	// so that it will always be resampled at least a little
	for (int N=1; N!=9; ++N)
	{
		if (int(float(current*N)*0.125) > desired) return N;
	}
	return 8;
}

void Image::rgbToRgba (uint8_t* in, uint8_t* out, Vec2i dim)
{
	uint8_t* out_stop = out + (dim.x*dim.y*4);
	while (out != out_stop)
	{
		*out = *in; ++out; ++in;
		*out = *in; ++out; ++in;
		*out = *in; ++out; ++in;
		*out = 255; ++out;
	}
}

void Image::unload ()
{
	/////////////////////////////////////////////////////////////////
	// Assumes the Image is already locked
	/////////////////////////////////////////////////////////////////
	
	if (!loaded) return;
	if (pixels != 0) delete[] pixels;
	dim_loaded = Vec2i::ORIGIN;
	loaded = false;
}

void Image::resourceUpdate ()
{
	/////////////////////////////////////////////////////////////////
	// -Called from ResourceManager::worker()
	// -Load, reload, or unload as cessary given the current
	//  visibility and dimensions
	/////////////////////////////////////////////////////////////////
		
	// To be unlocked when this thread starts loading
	// or immediately if we don't have to load
	lock();
	if (loading)
	{
		unlock();
		return;
	}
	
	if (visible && !failed)
	{
		// Get the visible resolution
		Camera& camera = Camera::getSingleton();
		camera.lock();
		Vec2i& cam_dim = camera.dim;
		Rect& cam_rect = camera.target;
		camera.unlock();
		
		// Pixels / size
		// NOTE: rect_aa uses radii, not width and height, so there are
		//       factors of 2 and 1/2 that I canceled out (same answer)
		float cam_res = float(cam_dim.x+cam_dim.y) / float(cam_rect.rx+cam_rect.ry);
		float visible_dim_x = cam_res * target.rx / (1.0f-crop_left-crop_right);
//		float visible_dim_y = cam_res * target.ry / (1.0f-crop_bottom-crop_top);
		
		// See if it's not loaded or there is a better scale available
		if (!loaded || ((format == JPEG) && (getJpegDecodeScale(dim_full.x, visible_dim_x) != jpeg_scale)))
		{
			unload();
			loading = true;
			Format tmp_format = format;
			unlock();
			try
			{
				// Unlocked section protected by the loading flag until loading completes or errors
				// Each loadXXX function can safely edit pixels and output_dim without locking
				switch (tmp_format)
				{
				case JPEG: loadJpeg(path, visible_dim_x); break;
				default:   throw string("Image::resourceUpdate(): No loader for this image format");
				}
				
				// Succeeded loading
				lock();
				loading = false;
				loaded = true;
				failed = false;
				unlock();
				
			}
			catch (string s)
			{
				// Failed loading
				lock();
				unload();
				loaded = false;
				loading = false;
				failed = true;
				unlock();
				cout << s << endl;
			}
		}
		else unlock();
	}
	else
	{
		// Not visible or previously failed
		unload();
		unlock();
	}
}

