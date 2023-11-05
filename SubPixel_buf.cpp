#include "SubPixel_buf.h"

float E1(const vec4 &V, const vec4 &VP)
{
  return (V[X] * VP[Y] - V[Y] * VP[X]);
}

SubPixelBuf
get_intersection(const vec4& Point, const float& A, const float& B, const float& C, const vec4& P1, const vec4& P2, const vec4& P3, const float& e)
{
    SubPixelBuf buff;
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
        buff.type = DIVIDED;

        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(Point);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);

        buff.calculateSquare();
    }
    else if (is_bottom && is_left) {
        buff.type = DIVIDED;

        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(Point);
        buff.second_part.push_back(leftPoint);

        buff.calculateSquare();
    }
    else if (is_bottom && is_right) {
        buff.type = DIVIDED;

        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(Point);
        buff.first_part.push_back(leftBottomCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();
    }
    else if (is_left && is_top) {
        buff.type = DIVIDED;

        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(Point);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);

        buff.calculateSquare();
    }
    else if (is_left && is_right) {
        buff.type = DIVIDED;

        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(Point);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();
    }
    else if (is_top && is_right) {
        buff.type = DIVIDED;

        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);

        buff.second_part.push_back(upPoint);
        buff.second_part.push_back(Point);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();
    }
    else if (!is_top && !is_bottom && !is_right && !is_left) {
        
    }

    return buff;
}

void
SubPixelBuf::calculateSquare()
{
    /*https://stackoverflow.com/questions/451426/how-do-i-calculate-the-area-of-a-2d-polygon*/

    float area = 0.f;

    for (int i = 0; i < this->first_part.size(); ++i) {
        int i1 = (i + 1) % this->first_part.size();

        area += (this->first_part[i][Y] + this->first_part[i1][Y]) * (this->first_part[i1][X] - this->first_part[i][X]);
    }

    this->square1 = area / 2.0;

    area = 0.f;

    for (int i = 0; i < this->second_part.size(); ++i) {
        int i1 = (i + 1) % this->second_part.size();

        area += (this->second_part[i][Y] + this->second_part[i1][Y]) * (this->second_part[i1][X] - this->second_part[i][X]);
    }

    this->square2 = area / 2.0;
}

Color 
SubPixelBuf::calcPixelColor()
{
    Color pixelColor;

    pixelColor.red = (color1.red * square1 + color2.red * square2) / (square1 + square2);
    pixelColor.green = (color1.green * square1 + color2.green * square2) / (square1 + square2);
    pixelColor.blue = (color1.blue * square1 + color2.blue * square2) / (square1 + square2);

    return pixelColor;
}