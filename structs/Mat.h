#pragma once
#include <cstddef>
#include "Vec.h"
#include <iostream>

template<size_t P, typename T>
class Mat
{
private:
    T* matrix = nullptr;
    size_t size = 0;

public:
    Mat(T* matrix)
    {
        this->size = P;
        this->matrix = new T[size * size];
        
        for (size_t i = 0; i < size * size; ++i) {
        this->matrix[i] = matrix[i];
        } 
    }

    Mat() 
    {
        this->size = P;
        this->matrix = new T[P];
    }

    Mat(const Mat& obj) 
    {
        this->size = obj.size;
        this->matrix = new T[obj.size * obj.size];
        
        for (size_t i = 0; i < obj.size * obj.size; ++i) {
        this->matrix[i] = obj.matrix[i];
        } 
    }

    ~Mat()
    {
        delete [] matrix;
        size = 0;
    }

    Mat<P, T>
    operator* (const Mat<P, T> &obj) const
    {
        Mat<P, T> res(this->matrix);

        for (int i = 0; i < res.size; i++) {
        for (int j = 0; j < res.size; j++) {
            res.matrix[i * 4 + j] = 0.f;
            for (int k = 0; k < 4; k++) {
            res.matrix[i * 4 + j] += this->matrix[i * 4 + k] * obj.matrix[k * 4 + j];
            }
        }
        }

        return res;
    }

    Mat<P, T>&
    operator= (const Mat<P, T>& other)
    {
        this->size = other.len;
        delete [] this->matrix;

        this->matrix = new T[other.size * other.size];

        for (int i = 0; i < this->size * this->size; ++i) {
        this->matrix[i] = other.matrix;
        }

        return *this;
    }

    Vec<P, T>
    operator* (const Vec<P, T> &v)
    {
        Vec<P, T> res;

        for (int i = 0; i < P; ++i) {
        res[i] = 0;
        for (int j = 0; j < P; ++j) {
            res[i] += matrix[4 * i + j] * v[j];
        }
        }

        return res;
    }

    T*
    getMatrix() 
    {
        return matrix;
    }

    void
    show() const 
    {
        for (int i = 0 ; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i * size + j] << " ";
        }

        std::cout << std::endl;
        }
    }
};

Mat<4, float> lookAt(const Vec<4, float> &P, const Vec<4, float> &target, const Vec<4, float> &up);

// template<typename T>
// Mat<4, T>
// lookAt(const Vec<4, T> &P, const Vec<4, T> &target, const Vec<4, T> &up)
// {
//   Vec<4, T> D = normalize(target - P);
//   Vec<4, T> R = normalize(cross(up, D));
//   Vec<4, T> U = cross(D, R);
  
//   float m1[16] = {
//     R[0], R[1], R[2], 0.0f,
//     U[0], U[1], U[2], 0.0f,
//     D[0], D[1], D[2], 0.0f,
//     0.0f, 0.0f, 0.0f, 1.0f
//   };

//   float m2[16] = {
//     1.f, 0.f, 0.f, -P[0],
//     0.f, 1.f, 0.f, -P[1],
//     0.f, 0.f, 1.f, -P[2],
//     0.f, 0.f, 0.f, 1.f,
//   };

//   Mat<4, float> mat1(m1), mat2(m2);

//   return mat1 * mat2;

//   // float m[16] = {
//   //   R[0], R[1], R[2], -P[0],
//   //   U[0], U[1], U[2], -P[1],
//   //   D[0], D[1], D[2], -P[2],
//   //   0.f,  0.f,  0.f,   1.f ,
//   // };

//   // Mat<4, float> M(m);

//   // return M;
// }