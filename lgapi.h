#pragma once 

#include <vector>
#include <cstddef>
#include <iostream>
#include <cmath>

enum RENDER_MODE { MODE_VERT_COLOR = 0,
                   MODE_TEXURE_3D  = 1, };

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
dot(const Vec<P, T> &obj1, const Vec<P, T> &obj2)
{
  T dot_product = 0;

  for (int i = 0; i < P; ++i) {
    dot_product += obj1[i] * obj2[i];
  }

  return dot_product;
}

template<int P, typename T>
Vec<P, T> normalize(const Vec<P, T> &vec)
{
  float norm_len = 0;
  Vec<P, T> res(vec);

  norm_len = vec.getLen();

  for (int i = 0; i < P && norm_len != 0; ++i) {
    res[i] /= norm_len;
  }

  return res;
}

template<typename T>
Mat<4, T>
lookAt(const Vec<4, T> &P, const Vec<4, T> &target, const Vec<4, T> &up)
{
  Vec<4, T> D = normalize(target - P);
  Vec<4, T> R = normalize(cross(up, D));
  Vec<4, T> U = cross(D, R);
  
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

struct LightObj {
    float x;
    float y;
    float z;
    float w;
    
    Vec4 Color;
};

enum GEOM_TYPE { GEOM_TRIANGLES = 1, GEOM_QUADS = 2 };

struct Geom
{
  const float*        vpos4f  = nullptr; ///< quads of vertex coordinates for vertex positions x,y,z,w; w is unused
  const float*        vcol4f  = nullptr; ///< quads of vertex color attributes r,g,b,a; a is unused
  const float*        vtex2f  = nullptr; ///< pairs of x and y coordinates
  const unsigned int* indices = nullptr; ///< index buffer of size 3*primsNum for triangle meshes and 4*primsNum for qual meshes

  unsigned int vertNum  = 0;
  unsigned int primsNum = 0;
  GEOM_TYPE    geomType = GEOM_TRIANGLES;
};

struct Image2D
{
  Image2D(){}
  Image2D(unsigned int w, unsigned int h, unsigned int* a_data) : data(a_data), width(w), height(h) {}

  unsigned int* data;   ///< access pixel(x,y) as data[y*width+x]
  unsigned int  width;
  unsigned int  height; 
};

struct TextureContainer {
  std::vector<Image2D> textures;

  size_t size()
  {
    return textures.size();
  }

  size_t addTexture(const Image2D &tex)
  {
    textures.push_back(tex);

    return textures.size() - 1;
  }
};

struct ShaderContainer {
  Vec4 (*vertexShader)(const float *, const Geom& , const float *);
  uint32_t (*textureShader)(const int &, const TextureContainer &, const float [3][2], const float &, const float &, const float &, const float &);
  Vec4 (*colorShader)(const float [3][4], const float &, const float &, const float &, const float &);
  uint32_t (*lightShader)(const float col[3][4], const float &w0, const float &w1, const float &w2, const float &w);
  Vec4 (*ambientLightShader)(const Vec4 &ObjColor, const Vec4 &LightSourceColor);
  Vec4 (*diffusalLightShader)(const Vec4 &ObjColor, const Vec4 &LightColor, const Vec4 &LightDir, const Vec4 &Norm);
};

struct PipelineStateObject
{
  float worldViewMatrix[16]; ///< assume row-major layout, i.e. M[0], M[1], M[2], M[3] is the first row of the matrix
  float projMatrix[16];      ///< assume row-major layout, i.e. M[0], M[1], M[2], M[3] is the first row of the matrix
  RENDER_MODE  mode  = MODE_VERT_COLOR;
  unsigned int imgId = 0;

  ShaderContainer *shader_container;
};

struct IRender
{
  IRender(){}
  virtual ~IRender(){}
  
  virtual unsigned int AddImage(const Image2D &a_img) = 0;

  virtual void BeginRenderPass(Image2D &fb, LightObj *L = 0) = 0;
  virtual void Draw(PipelineStateObject a_state, Geom a_geom) = 0;
  virtual void EndRenderPass(Image2D &fb) = 0;
};