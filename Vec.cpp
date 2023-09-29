
#include "Mat.h"
#include "Vec.h"

// template<typename T>
// Vec<4, T> 
// cross(const Vec<4, T> &a, const Vec<4, T> &b)
// {
//   Vec<4, T> res;

//   res[0] = a[1] * b[2] - a[2] * b[1];
//   res[1] = a[2] * b[0] - a[0] * b[2];
//   res[2] = a[0] * b[1] - a[1] * b[0];
//   res[3] = 0;

//   return res;
// }

// template<int P, typename T>
// T
// dot(const Vec<P, T> &obj1, const Vec<P, T> &obj2)
// {
//   T dot_product = 0;

//   for (int i = 0; i < P; ++i) {
//     dot_product += obj1[i] * obj2[i];
//   }

//   return dot_product;
// }

// template<int P, typename T>
// Vec<P, T> normalize(const Vec<P, T> &vec)
// {
//   float norm_len = 0;
//   Vec<P, T> res(vec);

//   norm_len = vec.getLen();

//   for (int i = 0; i < P && norm_len != 0; ++i) {
//     res[i] /= norm_len;
//   }

//   return res;
// }