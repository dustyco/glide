

#include "Image.h"
#include <cstdio>
#include <cmath>
#include <jpeglib.h>


void Image::loadJpeg (string path, int x_preferred) throw(string)
{
	using namespace std;
	
	FILE* file;
	if ((file = fopen(path.c_str(), "rb")) == NULL) 
		throw string("Image::readJpeg(): Failed to open file for reading");
	
	// Init decompresser
	jpeg_decompress_struct d;
	jpeg_error_mgr e;
	d.err = jpeg_std_error(&e);
	jpeg_create_decompress(&d);
	jpeg_stdio_src(&d, file);
	
	// Get preliminary metadata
	jpeg_read_header(&d, TRUE);
//	cout << "image size: " << d.image_width << "x" << d.image_height << " px" << endl;
	
	// Set decompression options
//	d.scale_num = getJpegDecodeScale(d.image_width, x_preferred);
	Vec2i dim_libjpeg(d.image_width, d.image_height);
	if (dim_libjpeg != dim_full)
	{
		cout << "Mismatching dims: readFileInfo(): " << dim_full << "    " << path << endl;
		cout << "                      loadJpeg(): " << dim_libjpeg << endl;
	}
	d.scale_num = getJpegDecodeScale(dim_full.x, x_preferred);
	d.scale_denom = 8;
	jpeg_scale = d.scale_num;
//	cout << "scale: " << d.scale_num << "/" << d.scale_denom << endl;
	
	// Get calculated size if it's being scaled
	jpeg_calc_output_dimensions(&d);
//	cout << "output size: " << d.output_width << "x" << d.output_height << " px"<< endl;
	dim_loaded = Vec2i(d.output_width, d.output_height);
	
	// Decompress
	d.output_components = 3;
	if (d.output_components != 3)
	{
		// TODO Throw an error or convert to 3
		cout << "DANGER! DANGER! DANGER! Jpeg output component count is not 3" << endl;
	}
	int row_stride = d.output_width * d.output_components;
	int image_stride = row_stride*d.output_height;
//	cout << "row stride: " << float(row_stride)*1e-3 << " KB" << endl;
//	cout << "image stride: " << float(image_stride)*1e-6 << " MB" << endl;
	JSAMPLE* output = new(nothrow) JSAMPLE[image_stride];
	if (output == NULL) cout << "Allocation error" << endl; // TODO
	jpeg_start_decompress(&d);
	for (JSAMPLE* row=output; row!=output+image_stride; row+=row_stride)
	{
		// TODO Periodically check to see if the image is still visible and cancel if not
		jpeg_read_scanlines(&d, &row, 1);
	}
	jpeg_finish_decompress(&d);
	
	// Clean up
	jpeg_destroy_decompress(&d);
	fclose(file);
	
	// Set Image members
	pixels = (uint8_t*)output;
	dim_loaded = Vec2i(d.output_width, d.output_height);
	
	// Convert from RGB to RGBA
	uint8_t* pixels_rgba = new(nothrow) uint8_t[dim_loaded.x*dim_loaded.y*4];
	if (pixels_rgba == NULL) cout << "Allocation error" << endl; // TODO
	rgbToRgba(pixels, pixels_rgba, dim_loaded);
	delete[] pixels;
	pixels = pixels_rgba;
}


