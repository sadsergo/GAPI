#include "SubPixel_buf.h"

float E1(const vec4 &V, const vec4 &VP)
{
  return (V[X] * VP[Y] - V[Y] * VP[X]);
}

SubPixelBuf
get_intersection(const vec4& P, const float& A, const float& B, const float& C, const vec4& P1, const vec4& P2, const vec4& P3, const float& e)
{
    SubPixelBuf buff;
    float intersect_x = 0, intersect_y = 0;
    bool is_top = false, is_bottom = false, is_left = false, is_right = false;
    vec4 bottomPoint, upPoint, leftPoint, rightPoint;
    
    vec4 Point;
    Point[X] = P[X];
    Point[Y] = P[Y];

    //  Left Border

    intersect_y = (-A * Point[X] - C) / B;
    /*if (Point[Y] == 764)
        std::cout << Point[Y] << " " << Point[X] << " " << intersect_y << " " << Point[Y] + 1 << std::endl;*/
    
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

    //  Bottom Border

    intersect_x = (-B * Point[Y] - C) / A;

    if (intersect_x >= Point[X] && intersect_x <= Point[X] + 1) {
        float p[4] = {intersect_x, (float)Point[Y], 0, 1};
        vec4 newP(p);

        bottomPoint = newP;
        is_bottom = true;
    }

    //  Up Border

    intersect_x = (-B * (Point[Y] + 1) - C) / A;

    if (intersect_x >= Point[X] && intersect_x <= Point[X] + 1) {
        float p[4] = {intersect_x, (float)Point[Y] + 1, 0, 1};
        vec4 newP(p);

        upPoint = newP;
        is_top = true;
    }
    
    auto leftUpCorner = Point;
    leftUpCorner[Y] += 1;

    auto rightBottomCorner = Point;
    rightBottomCorner[X] += 1;
    //rightBottomCorner[Y] += 1;

    auto leftBottomCorner = Point;
    //leftBottomCorner[Y] += 1;

    auto rightUpCorner = Point;
    rightUpCorner[X] += 1;
    rightUpCorner[Y] += 1;

    if (is_bottom && is_top) {
        buff.type = DIVIDED;

        buff.first_part.push_back(leftBottomCorner);
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(leftUpCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, leftUpCorner - P1), w1 = E1(P3 - P2, leftUpCorner - P2), w2 = E1(P1 - P3, leftUpCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, rightBottomCorner - P1), w1 = E1(P3 - P2, rightBottomCorner - P2), w2 = E1(P1 - P3, rightBottomCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);
                
                buff.triangle_part = SECOND_PART;
                buff.depth2 = w;
            }
        }
    }
    else if (is_bottom && is_left) {
        buff.type = DIVIDED;

        buff.first_part.push_back(leftBottomCorner);
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(leftPoint);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(leftUpCorner);
        buff.second_part.push_back(leftPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, leftBottomCorner - P1), w1 = E1(P3 - P2, leftBottomCorner - P2), w2 = E1(P1 - P3, leftBottomCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, rightUpCorner - P1), w1 = E1(P3 - P2, rightUpCorner - P2), w2 = E1(P1 - P3, rightUpCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                buff.triangle_part = SECOND_PART;
                buff.depth2 = w;
            }
        }
    }
    else if (is_bottom && is_right) {
        buff.type = DIVIDED;
    
        buff.first_part.push_back(leftBottomCorner);
        buff.first_part.push_back(bottomPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(leftUpCorner);

        buff.second_part.push_back(bottomPoint);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, leftUpCorner - P1), w1 = E1(P3 - P2, leftUpCorner - P2), w2 = E1(P1 - P3, leftUpCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, rightBottomCorner - P1), w1 = E1(P3 - P2, rightBottomCorner - P2), w2 = E1(P1 - P3, rightBottomCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                buff.triangle_part = SECOND_PART;
                buff.depth2 = w;
            }
        }
    }
    else if (is_left && is_top) {
        buff.type = DIVIDED;
      
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(leftUpCorner);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightUpCorner);
        buff.second_part.push_back(upPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, leftUpCorner - P1), w1 = E1(P3 - P2, leftUpCorner - P2), w2 = E1(P1 - P3, leftUpCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, rightBottomCorner - P1), w1 = E1(P3 - P2, rightBottomCorner - P2), w2 = E1(P1 - P3, rightBottomCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                buff.triangle_part = SECOND_PART;
                buff.depth2 = w;
            }
        }
    }
    else if (is_left && is_right) {
        buff.type = DIVIDED;
        
        buff.first_part.push_back(leftPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);
        buff.first_part.push_back(leftUpCorner);

        buff.second_part.push_back(leftPoint);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, leftUpCorner - P1), w1 = E1(P3 - P2, leftUpCorner - P2), w2 = E1(P1 - P3, leftUpCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, rightUpCorner - P1), w1 = E1(P3 - P2, rightUpCorner - P2), w2 = E1(P1 - P3, rightUpCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                buff.triangle_part = FIRST_PART;
                buff.depth1 = w;
            }
            else {
                w0 = E1(P2 - P1, leftBottomCorner - P1), w1 = E1(P3 - P2, leftBottomCorner - P2), w2 = E1(P1 - P3, leftBottomCorner - P3);

                if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                    w0 = abs(w0) / e;
                    w1 = abs(w1) / e;
                    w2 = abs(w2) / e;

                    float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                    buff.triangle_part = SECOND_PART;
                    buff.depth2 = w;
                }
                else {
                    w0 = E1(P2 - P1, rightBottomCorner - P1), w1 = E1(P3 - P2, rightBottomCorner - P2), w2 = E1(P1 - P3, rightBottomCorner - P3);

                    if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                        w0 = abs(w0) / e;
                        w1 = abs(w1) / e;
                        w2 = abs(w2) / e;

                        float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                        buff.triangle_part = SECOND_PART;
                        buff.depth2 = w;
                    }
                }
            }
        }
    }
    else if (is_top && is_right) {
        buff.type = DIVIDED;
        
        buff.first_part.push_back(upPoint);
        buff.first_part.push_back(rightPoint);
        buff.first_part.push_back(rightUpCorner);

        buff.second_part.push_back(upPoint);
        buff.second_part.push_back(leftUpCorner);
        buff.second_part.push_back(leftBottomCorner);
        buff.second_part.push_back(rightBottomCorner);
        buff.second_part.push_back(rightPoint);

        buff.calculateSquare();

        float w0 = E1(P2 - P1, rightUpCorner - P1), w1 = E1(P3 - P2, rightUpCorner - P2), w2 = E1(P1 - P3, rightUpCorner - P3);

        if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
            w0 = abs(w0) / e;
            w1 = abs(w1) / e;
            w2 = abs(w2) / e;

            float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

            buff.triangle_part = FIRST_PART;
            buff.depth1 = w;
        }
        else {
            w0 = E1(P2 - P1, leftBottomCorner - P1), w1 = E1(P3 - P2, leftBottomCorner - P2), w2 = E1(P1 - P3, leftBottomCorner - P3);

            if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                w0 = abs(w0) / e;
                w1 = abs(w1) / e;
                w2 = abs(w2) / e;

                float w = (P3[2] * w0 + P1[2] * w1 + P2[2] * w2);

                buff.triangle_part = SECOND_PART;
                buff.depth2 = w;
            }
        }
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
        int j = (i + 1) % this->first_part.size();

        area += (first_part[i][X] * first_part[j][Y] - first_part[j][X] * first_part[i][Y]);
    }
    
    area /= 2.0;

    this->square1 = (area >= 0) ? area : -1 * area;

    /*area = 0.f;

    for (int i = 0; i < this->second_part.size(); ++i) {
        int j = (i + 1) % this->second_part.size();

        area += (second_part[i][X] * second_part[j][Y] - second_part[j][X] * second_part[i][Y]);
    }

    area /= 2.0;

    this->square2 = (area >= 0) ? area : -1 * area;*/

    this->square2 = 1 - this->square1;
}

Color 
SubPixelBuf::calcPixelColor()
{
    Color pixelColor;
    int red = ((float)((int)color1.red) * square1 + (float)((int)color2.red) * square2);
    int green = ((float)((int)color1.green) * square1 + (float)((int)color2.green) * square2);
    int blue = ((float)((int)color1.blue) * square1 + (float)((int)color2.blue) * square2);

    /*red = (red > 255) ? 255 : red;
    green = (green > 255) ? 255 : green;
    blue = (blue > 255) ? 255 : blue;*/

    pixelColor.red = (unsigned int)(red);
    pixelColor.green = (unsigned int)(green);
    pixelColor.blue = (unsigned int)(blue);

    return pixelColor;
}