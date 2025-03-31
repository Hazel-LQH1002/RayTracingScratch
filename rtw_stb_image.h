#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#ifdef _MSC_VER
#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>


class rtw_image
{
public:

	rtw_image(const char* filename)
	{
		std::string sFilename = filename;
		if (load(sFilename)) return;
		std::cerr << "Loading invalid picture" << "\n";
	}

	~rtw_image()
	{
		delete bData;
		STBI_FREE(fData);
	}

	bool load(const std::string& filename)
	{
		int channels_in_file = bytes_per_pixel;
		fData = stbi_loadf(filename.c_str(), &img_width, &img_height, &channels_in_file, bytes_per_pixel);
		if (!fData)
			return false;
		bytes_per_scanline = bytes_per_pixel * img_width;
		convert_to_bytes();
		return true;
	}

	static int clamp(int x, int low, int high) {
		// Return the value clamped to the range [low, high).
		if (x < low) return low;
		if (x < high) return x;
		return high - 1;
	}

	static unsigned char float_to_byte(float value) {
		if (value <= 0.0)
			return 0;
		if (1.0 <= value)
			return 255;
		return static_cast<unsigned char>(256.0 * value);
	}

	void convert_to_bytes() {
		// Convert the linear floating point pixel data to bytes, storing the resulting byte
		// data in the `bdata` member.

		int total_bytes = img_width * img_height * bytes_per_pixel;
		bData = new unsigned char[total_bytes];

		// Iterate through all pixel components, converting from [0.0, 1.0] float values to
		// unsigned [0, 255] byte values.

		auto* bptr = bData;
		auto* fptr = fData;
		for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
			*bptr = float_to_byte(*fptr);
	}

	int width() const { return fData ? img_width : 0; }
	int height() const { return fData ? img_height : 0; }

	const unsigned char* pixel_data(int x, int y) const
	{
		static unsigned char redColor[] = { 255, 0, 0 };
		if (!fData) return redColor;
		x = clamp(x, 0, img_width);
		y = clamp(y, 0, img_height);
		return bData + y * bytes_per_scanline + x * bytes_per_pixel;
	}


private:
	int img_width = 0;
	int img_height = 0;
	float* fData = nullptr;
	unsigned char* bData = nullptr;
	int bytes_per_pixel = 3;
	int bytes_per_scanline = 0;


};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif
