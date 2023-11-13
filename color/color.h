#pragma once

#include <stdint.h>


struct Color
{
	uint8_t red = 255, green = 255, blue = 255;

	Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255) : red(red), green(green), blue(blue) {}
	Color(uint32_t packed_color) 
	{
		this->red = (packed_color & 0x00FF0000) >> 16;
		this->green = (packed_color & 0x0000FF00) >> 8;
		this->blue = (packed_color & 0x000000FF);
	}
	
	uint32_t pack();
};