#include "SubPixel_buf.h"

float E1(const vec4 &V, const vec4 &VP)
{
  return (V[X] * VP[Y] - V[Y] * VP[X]);
}

SubPixelBuf
get_intersection(const vec4& Point, const float& A, const float& B, const float& C, const vec4& P1, const vec4& P2, const vec4& P3, const float& e)
{
    SubPixelBuf buff;
    std::vector<vec4> points;
    float intersect_x = 0, intersect_y = 0;
    bool is_top = false, is_bottom = false, is_left = false, is_right = false;
    vec4 bottomPoint, upPoint, leftPoint, rightPoint;
    
    //  Left Border

    intersect_y = (-A * Point[X] - C) / B;
    
    if (intersect_y >= Point[Y] && intersect_y <= Point[Y] + 1) {
        float p[4] = {(float)Point[X], intersect_y, 0, 1};
        vec4 newP(p);

        leftPoint = newP;
        is_left = true;
    }

    //  Right Border

    intersect_y = (-A * (Point[X] + 1) - C) / B;

    if (intersect_y >= Point[Y] && intersect_y <= Point[Y] + 1) {
        float p[4] = {(float)Point[X] + 1, intersect_y, 0, 1};
        vec4 newP(p);

        rightPoint = newP;
        is_right = true;
    }

    //  Top Border

    intersect_x = (-B * Point[Y] - C) / A;

    if (intersect_x >= Point[X] && intersect_x <= Point[X] + 1) {
        float p[4] = {intersect_x, (float)Point[Y], 0, 1};
        vec4 newP(p);

        upPoint = newP;
        is_top = true;
    }

    //  Bottom Border

    intersect_x = (-B * (Point[Y] + 1) - C) / A;

    if (intersect_x >= Point[X] && intersect_x <= Point[X] + 1) {
        float p[4] = {intersect_x, (float)Point[Y] + 1, 0, 1};
        vec4 newP(p);

        bottomPoint = newP;
        is_bottom = true;
    }

    auto leftBottomCorner = Point;
    leftBottomCorner[Y] += 1;

    auto rightBottomCorner = Point;
    rightBottomCorner[X] += 1;
    rightBottomCorner[Y] += 1;

    auto rightUpCorner = Point;
    rightUpCorner[X] += 1;

    if (is_bottom && is_top) {
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(Point);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);
    }
    else if (is_bottom && is_left) {
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(Point);
        buff.second_part.push_back(leftPoint);
    }
    else if (is_bottom && is_right) {
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(Point);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);
    }
    else if (is_left && is_top) {
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(Point);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);
    }
    else if (is_left && is_right) {
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(Point);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);
    }
    else if (is_top && is_right) {
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);

        buff.second_part.push_back(upPoint);
        buff.second_part.push_back(Point);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);
    }
    else if (!is_top && !is_bottom && !is_right && !is_left) {
        
    }

    return buff;
}