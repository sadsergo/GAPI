#include "Mat.h"


Mat<4, float>
lookAt(const Vec<4, float> &P, const Vec<4, float> &target, const Vec<4, float> &up)
{
  Vec<4, float> D = normalize(target - P);
  Vec<4, float> R = normalize(cross(up, D));
  Vec<4, float> U = cross(D, R);
  
  float m1[16] = {
    R[0], R[1], R[2], 0.0f,
    U[0], U[1], U[2], 0.0f,
    D[0], D[1], D[2], 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  float m2[16] = {
    1.f, 0.f, 0.f, -P[0],
    0.f, 1.f, 0.f, -P[1],
    0.f, 0.f, 1.f, -P[2],
    0.f, 0.f, 0.f, 1.f,
  };

  Mat<4, float> mat1(m1), mat2(m2);

  return mat1 * mat2;

  // float m[16] = {
  //   R[0], R[1], R[2], -P[0],
  //   U[0], U[1], U[2], -P[1],
  //   D[0], D[1], D[2], -P[2],
  //   0.f,  0.f,  0.f,   1.f ,
  // };

  // Mat<4, float> M(m);

  // return M;
}