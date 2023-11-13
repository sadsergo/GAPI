#pragma once 

#include <vector>
#include <cstdint>

#include <structs/Vec.h>
#include "color/color.h"

enum 
{
    NOT_DIVIDED = 0,
    DIVIDED = 1,
    FULL_SQUARE = 2,
};

enum
{
    FIRST_PART = 1,
    SECOND_PART = 2,
    NO_PART = 0,
};

struct SubPixelBuf 
{
    std::vector<vec4> first_part, second_part;
    Color color1, color2;
    float depth1 = 0, depth2 = 0;
    int type = NOT_DIVIDED;
    float square1 = 0, square2 = 0;
    char triangle_part = NO_PART;

    void calculateSquare();
    Color calcPixelColor();
};

SubPixelBuf get_intersection(const vec4& Point, const float& A, const float& B, const float& C, const vec4& P1, const vec4& P2, const vec4& P3, const float& e);
float E1(const vec4 &V, const vec4 &VP);