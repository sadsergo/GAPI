#pragma once

struct Vec4 {
    float x;
    float y;
    float z;
    float w;

    Vec4(float x = 0, float y = 0, float z = 0, float w = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

class Vec2 {
    float x;
    float y;
public:
    Vec2(float x = 0, float y = 0)
    {
        this->x = x;
        this->y = y;
    }

    Vec2(const Vec2& obj)
    {
        this->x = obj.x;
        this->y = obj.y;
    }

    Vec2& operator = (const Vec2& obj) {
        this->x = obj.x;
        this->y = obj.y;

        return *this;
    }

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    friend Vec2 operator - (const Vec2& a, const Vec2& B);

    friend float E(const Vec2& A, const Vec2& B, const Vec2& P);
    friend float E(const Vec2& V, const Vec2& VP);
    friend long long F(const Vec2& A, const Vec2& B, const Vec2& P);
    friend void RenderPartiallyCoveredBlock(int j, int x, int blockSize, Vec2 A, Vec2 B, Vec2 C);
};

struct LightObj {
    float x;
    float y;
    float z;
    float w;

    Vec4 Color;
};