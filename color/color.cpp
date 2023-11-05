
#include "color.h"

uint32_t
Color::pack()
{
	return ((unsigned char)(red) << 16) + ((unsigned char)(green) << 8) + ((unsigned char)(blue));
}
