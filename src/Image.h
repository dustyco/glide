

#pragma once
#include "common.h"
#include "Frame.h"
#include "Edit.h"
#include <mutex>
#include <memory>
#include <vector>



// Safe way to handle references to an Image
struct Image;
typedef shared_ptr<Image> ImagePtr;
typedef vector<ImagePtr> ImageVec;

struct Image : Frame, Edit, NonCopyable, mutex
{
	struct CompareName { bool operator () (const ImagePtr lhs, const ImagePtr rhs) { return lhs->path < rhs->path; } };
	
	enum Format { JPEG, GIF, PNG, BMP };
	
	// Static functions
	static void createImages       (vector<ImagePtr>& images, string path) throw(string);
	static void readFileInfo       (string path, Format& format, int& x, int& y) throw(string);
	static int  getJpegDecodeScale (int current, int desired);
	static void rgbToRgba          (uint8_t* in, uint8_t* out, Vec2i dim);
	
	// Available on construction, never changes
	string path;
	Format format;
	Vec2i  dim_full;
	
	// Load data
	// If loading is true, do not touch pixels, dim_loaded, or jpeg_scale even while the mutex is locked
	bool     loaded, loading, failed;
	uint8_t* pixels;     // Protected by loading flag, not mutex
	Vec2i    dim_loaded; // Protected by loading flag, not mutex
	int      jpeg_scale; // Protected by loading flag, not mutex
	
	// Texture data (sf::Texture, hidden to save the unnecessary header)
	// NULL when not present
	void* texture;
	
	// World data
	bool  visible;
	
	// Member functions
	     Image          (string path, Format format, Vec2i dim_full);
	     ~Image         ();
	void loadEdit       ();
	void saveEdit       ();
	void loadJpeg       (string path, int x_preferred) throw(string);
	void unload         ();
	void resourceUpdate ();
};


