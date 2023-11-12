#pragma once

#include <stdint.h>


struct Color
{
	uint8_t red = 255, green = 255, blue = 255;

	uint32_t pack();
};