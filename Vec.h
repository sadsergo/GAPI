#pragma once 

#include <iostream>
#include <cmath>

template<int P, typename T>
class Vec 
{
private:
  size_t len = 0;
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
  }

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
    for (int i = 0 ; i < len; i++) {
      std::cout << coords[i] << " ";
    }

    std::cout << std::endl;
  }
};

template<typename T> Vec<4, T> cross(const Vec<4, T> &a, const Vec<4, T> &b) {
  Vec<4, T> res;

  res[0] = a[1] * b[2] - a[2] * b[1];
  res[1] = a[2] * b[0] - a[0] * b[2];
  res[2] = a[0] * b[1] - a[1] * b[0];
  res[3] = 0;

  return res;
}

template<int P, typename T> T dot(const Vec<P, T>&obj1, const Vec<P, T>&obj2) {
  T dot_product = 0;

  for (int i = 0; i < P; ++i) {
    dot_product += obj1[i] * obj2[i];
  }

  return dot_product;
}

template<int P, typename T> Vec<P, T> normalize(const Vec<P, T>& vec) {
  float norm_len = 0;
  Vec<P, T> res(vec);

  norm_len = vec.getLen();

  for (int i = 0; i < P && norm_len != 0; ++i) {
    res[i] /= norm_len;
  }

  return res;
}