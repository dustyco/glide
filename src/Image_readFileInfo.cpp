

#include "Image.h"
#include <cstdio>
#include <cstring>
#include <jpeglib.h>

#ifdef __unix__
#define ENDIAN_MACROS
#endif
#ifdef ENDIAN_MACROS
	#include <arpa/inet.h>
#endif


// Tries to use file header metadata to efficiently get image size
// Supports JPEG, GIF, PNG, BMP
void Image::readFileInfo (string path, Format& format, int& x, int& y) throw(string)
{
	// Start by reading the minimum amount of the file that
	// will contain size fields for all supported formats
	//
	// JPEG: Has a partition system
	// GIF: 10 B
	// PNG: 24 B
	// BMP: 26 B
	const long MIN_SIZE = 26;
	
	using namespace std;
	
	FILE* f = fopen(path.c_str(), "rb");
	if (f==0) throw string("Image::readFileDimensions(): Could not open file for reading");
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len<MIN_SIZE)
	{
		fclose(f);
		throw string("Image::readFileDimensions(): File is too small to be an image (probably empty)");
	}
	
	unsigned char buf[MIN_SIZE];
	fread(buf, 1, MIN_SIZE, f);

	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (
		   buf[0]==0xFF
		&& buf[1]==0xD8
		&& buf[2]==0xFF
		&& buf[3]==0xE0
		&& buf[6]=='J'
		&& buf[7]=='F'
		&& buf[8]=='I'
		&& buf[9]=='F'
	)
	{
/*		long pos=2;
		while (buf[2]==0xFF)
		{
			if (
				   buf[3]==0xC0
				|| buf[3]==0xC1
				|| buf[3]==0xC2
				|| buf[3]==0xC3
				|| buf[3]==0xC9
				|| buf[3]==0xCA
				|| buf[3]==0xCB
			) break;
			
			// Big endian
			#ifdef ENDIAN_MACROS
				pos += 2 + be16toh(*(uint16_t*)(&buf[4]));
			#else
				pos += 2 + (buf[4]<<8) + buf[5];
			#endif
			
			if (pos+12>len) break;
			fseek(f, pos, SEEK_SET);
			fread(buf+2,1,12,f);
		}
*/
	// Use libjpeg because our code doesn't work with EXIF type files
	jpeg_decompress_struct d;
	jpeg_error_mgr e;
	d.err = jpeg_std_error(&e);
	jpeg_create_decompress(&d);
	fseek(f, 0, SEEK_SET);
	jpeg_stdio_src(&d, f);
	
	// Read dimensions
	jpeg_read_header(&d, TRUE);
	x = d.image_width;
	y = d.image_height;
	
	jpeg_destroy_decompress(&d);
	fclose(f);
	
	format = JPEG;
	goto check;
	}

	fclose(f);
	
/*	// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
	if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
	{
		// Big endian
		#ifdef ENDIAN_MACROS
			y = be16toh(*(uint16_t*)(&buf[7]));
			x = be16toh(*(uint16_t*)(&buf[9]));
		#else
			y = (buf[7]<<8) + buf[8];
			x = (buf[9]<<8) + buf[10];
		#endif
		format = JPEG;
		goto check;
	}
*/
	// GIF: first three bytes say "GIF", next three give version number. Then dimensions
	if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
	{
		// Little endian
		#ifdef ENDIAN_MACROS
			x = le16toh(*(uint16_t*)(&buf[6]));
			y = le16toh(*(uint16_t*)(&buf[8]));
		#else
			x = buf[6] + (buf[7]<<8);
			y = buf[8] + (buf[9]<<8);
		#endif
		format = GIF;
		goto check;
	}
	
	// PNG: the first frame is by definition an IHDR frame, which gives dimensions
	if (
		   buf[0]==0x89
		&& buf[1]=='P'
		&& buf[2]=='N'
		&& buf[3]=='G'
		&& buf[4]=='\r'
		&& buf[5]=='\n'
		&& buf[6]==0x1A
		&& buf[7]=='\n'
		&& buf[12]=='I'
		&& buf[13]=='H'
		&& buf[14]=='D'
		&& buf[15]=='R'
	)
	{
		// Big endian
		#ifdef ENDIAN_MACROS
			x = be32toh(*(uint32_t*)(&buf[16]));
			y = be32toh(*(uint32_t*)(&buf[20]));
		#else
			x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
			y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);
		#endif
		format = PNG;
		goto check;
	}
	
	// BMP
	// http://www.fileformat.info/format/bmp/corion.htm
	if (buf[0]=='B' && buf[1]=='M')
	{
		// Little endian
		#ifdef ENDIAN_MACROS
			x = le32toh(*(uint32_t*)(&buf[18]));
			y = le32toh(*(uint32_t*)(&buf[22]));
		#else
			x = (buf[18]<<0) + (buf[19]<<8) + (buf[20]<<16) + (buf[21]<<24);
			y = (buf[22]<<0) + (buf[23]<<8) + (buf[24]<<16) + (buf[25]<<24);
		#endif
		format = BMP;
		goto check;
	}
	
	throw string("Image::readFileDimensions(): File doesn't look like a supported format");
	
	check:
	if (x<1 || y<1) throw string("Image::readFileDimensions(): Got impossible dimensions");
}


