#pragma once

#include <stdint.h>


struct Color
{
	uint8_t red, green, blue;

	uint32_t pack();
};