#pragma once 

#include <iostream>
#include <cmath>

enum 
{
  X = 0,
  Y = 1,
  Z = 2,
  W = 3,
};

template<int P, typename T>
class Vec 
{
private:
  int len = 0;
  T *coords = nullptr;
public:
  Vec(T *coords) : len(P)
  {
    this->coords = new T[len];

    for (int i = 0; i < len; ++i) {
      this->coords[i] = coords[i]; 
    }
  }

  Vec() 
  {
    this->len = P;
    this->coords = new T[P];

    for (int i = 0; i < P; ++i) {
      this->coords[i] = 0;
    }
  }

  Vec(const float& x, const float& y, const float& z, const float& w)
  {
    this->len = 4;
    this->coords = new float[4];

    coords[X] = x;
    coords[Y] = y;
    coords[Z] = z;
    coords[3] = w;
  }

  // template<typename... Args>
  // Vec(Args... args) : Vec()
  // {
  //   T arr[P] = { args... };
  //   for (int i = 0; i < P; ++i) {
  //     coords[i] = arr[i];
  //   }
  // }

  Vec(const Vec<P, T> &obj)
  {
    this->len = obj.len;

    this->coords = new T[len];

    for (int i = 0; i < len; ++i) {
      this->coords[i] = obj.coords[i]; 
    }
  }

  ~Vec()
  {
    delete [] coords;

    len = 0;
  }

  T&
  operator[] (size_t ind) const
  {
    return coords[ind];
  }

  bool
  operator==(const Vec<P, T>& obj)
  {
    bool is_true = true;

    for (int i = 0; is_true && i < P; ++i) {
      if (this->coords[i] != obj[i]) {
        is_true = false;
      }
    }

    return is_true;
  }

  Vec<P, T>&
  operator= (const Vec<P, T>& other)
  {
    this->len = other.len;
    delete [] this->coords;

    this->coords = new T[other.len];

    for (int i = 0; i < this->len; ++i) {
      this->coords[i] = other.coords[i];
    }

    return *this;
  }

  // Vec<P, T>&
  // operator= (const T& coords) {
  //   for (int i = 0; i < P; ++i) {
  //     this->coords[i] = coords[i];
  //   }

  //   return *this;
  // }

  Vec<P, T>
  operator- (const Vec<P, T>& other) const
  {
    Vec<P, T> result(this->coords);

    for (int i = 0; i < this->len; ++i) {
      result[i] -= other[i];
    }

    return result;
  }

  Vec<P, T>
  operator+ (const Vec<P, T>& other) const 
  {
    Vec<P, T> result(this->coords);

    for (int i = 0; i < this->len; ++i) {
      result[i] += other[i];
    }

    return result;
  }

  Vec<P, T>
  operator* (const T& num) const 
  {
    Vec<P, T> result(this->coords);

    for (int i = 0; i < this->len; ++i) {
      result[i] *= num;
    }

    return result;
  }

  float
  getLen() const 
  {
    float len = 0;

    for (int i = 0; i < P; ++i) {
      len += this->coords[i] * this->coords[i];
    }

    return sqrt(len);
  }

  void
  show() const
  {
    for (int i = 0; i < len; i++) {
      std::cout << coords[i] << " ";
    }

    std::cout << std::endl;
  }
};

template<typename T> 
Vec<4, T> 
cross(const Vec<4, T> &a, const Vec<4, T> &b) 
{
  Vec<4, T> res;

  res[0] = a[1] * b[2] - a[2] * b[1];
  res[1] = a[2] * b[0] - a[0] * b[2];
  res[2] = a[0] * b[1] - a[1] * b[0];
  res[3] = 0;

  return res;
}

template<int P, typename T> 
T 
dot(const Vec<P, T>&obj1, const Vec<P, T>&obj2) 
{
  T dot_product = 0;

  for (int i = 0; i < P; ++i) {
    dot_product += obj1[i] * obj2[i];
  }

  return dot_product;
}

template<int P, typename T> 
Vec<P, T> 
normalize(const Vec<P, T>& vec) 
{
  float norm_len = 0;
  Vec<P, T> res(vec);

  norm_len = vec.getLen();

  for (int i = 0; i < P && norm_len != 0; ++i) {
    res[i] /= norm_len;
  }

  return res;
}

template<int P, typename T>
Vec<P, T>
vec_intersect(const Vec<P, T>& A, const Vec<P, T>& B, const Vec<P, T>& C, const Vec<P, T>& D, bool *is_intersect)
{
  float a = (A[X] - C[Y]) * (B[X] - A[X]) + (C[X] - A[X]) * (B[Y] - A[Y]);
  float b = (D[Y] - C[Y]) * (B[X] - A[X]) - (D[X] - C[X]) * (B[Y] - A[Y]);

  if (b < 0.001 && b > -0.001) {
    *is_intersect = false;
    return A;
  }

  *is_intersect = true;

  Vec<P, T> res = C + D * (a / b);

  return res;
}

template<int P, typename T>
bool
isInHalfPlane(const Vec<P, T>& point, const Vec<P, T>& left_point, const Vec<P, T>& v)
{
  if (dot(v, point - left_point) >= 0) {
    return true;
  }

  return false;
}

using vec4 = Vec<4, float>;

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

  Vec2(const Vec2 &obj)
  {
    this->x = obj.x;
    this->y = obj.y;
  }

  Vec2 & operator = (const Vec2 &obj) {
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

  friend Vec2 operator - (const Vec2 &a, const Vec2 &B);

  friend float E(const Vec2 &A, const Vec2 &B, const Vec2 &P);
  friend float E(const Vec2 &V, const Vec2 &VP);
  friend long long F(const Vec2 &A, const Vec2 &B, const Vec2 &P);
  friend void RenderPartiallyCoveredBlock(int j, int x, int blockSize, Vec2 A, Vec2 B, Vec2 C);
};