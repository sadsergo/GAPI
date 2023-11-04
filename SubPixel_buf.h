#pragma once 

#include <vector>
#include <structs/Vec.h>
#include <cstdint>

struct SubPixelBuf 
{
    std::vector<vec4> first_part, second_part;
    uint8_t color1, color2;
    int64_t depth1, depth2;
};

SubPixelBuf get_intersection(const vec4& Point, const float& A, const float& B, const float& C, const vec4& P1, const vec4& P2, const vec4& P3, const float& e);
float E(const vec4 &V, const vec4 &VP);