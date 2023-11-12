#pragma once

#include "./color/color.h"


Vec4 
vertexShader(const float *rotatedWorldViewProj, const Geom &a_geom, const float vpos4f_i[4])  
{
  float vNDS_i[4];
  
  for (int i = 0; i < 4; i++) {
    vNDS_i[i] = 0;
    for (int j = 0; j < 4; j++) {
      vNDS_i[i] += rotatedWorldViewProj[i * 4 + j] * vpos4f_i[j];
    }
  } 

  float w = vNDS_i[3];
  Vec4 Res = {vNDS_i[0] / w, vNDS_i[1] / w, vNDS_i[2] / w, w};

  return Res;
}

uint32_t 
defaultVertexShader(const float *rotatedWorldViewProj, const Geom &a_geom, const float vpos4f_i[4])
{
  return 0;
}

void
defColorShader(const float col[3][4], const float& w0,
    const float& w1, const float& w2, const float& w, Color& color)
{
    color.red = 100;
    color.green = 100;
    color.blue = 0;
}

uint32_t 
defaultTextureShader(const int &imgId,
  const TextureContainer &Textures, const float uv[3][2], const float &w0, 
  const float &w1, const float &w2, const float &w)
{
  return 0;
}

Vec4
defaultAmbientLightShader(const Vec4 &ObjColor, const Vec4 &LightSourceColor)
{
  return Vec4(0.f, 0.f);
}

Vec4
defaultDiffusalLightShader(const Vec4 &ObjColor, const Vec4 &LightColor, const Vec4 &LightDir, const Vec4 &Norm)
{
  return Vec4(0.f, 0.f);
}

void
colorShader(const float col[3][4], const float &w0, 
  const float &w1, const float &w2, const float &w, Color& color)
{
  float red = (col[2][0] * w0 + col[0][0] * w1 + col[1][0] * w2) / w;
  float green = (col[2][1] * w0 + col[0][1] * w1 + col[1][1] * w2) / w;
  float blue = (col[2][2] * w0 + col[0][2] * w1 + col[1][2] * w2) / w;

  color.red = 255 * red;
  color.green = 255 * green;
  color.blue = 255 * blue;
}

void
textureShader(const int &imgId,
  const TextureContainer &Textures, const float uv[3][2], const float &w0, 
  const float &w1, const float &w2, const float &w, Color& color)
{
  Image2D texture = Textures.textures[imgId];
  size_t s = texture.width * texture.height;
  int uv1 = (w0 * uv[2][0] + w1 * uv[0][0] + w2 * uv[1][0]) / w * texture.width;
  int uv2 = (w0 * uv[2][1] + w1 * uv[0][1] + w2 * uv[1][1]) / w * texture.height;

  unsigned int texture_color = texture.data[(texture.width * uv2 + uv1) % (s)];
  unsigned int red = (texture_color & 0x00FF0000) >> 16;
  unsigned int green = (texture_color & 0x0000FF00) >> 8;
  unsigned int blue = texture_color & 0x00000FF;

  color.red = red;
  color.green = green;
  color.blue = blue;
}

Vec4 
ambientLightShader(const Vec4 &ObjColor, const Vec4 &LightSourceColor)
{
  float ambientStrength = 0.1f;
  Vec4 Color;

  Color.x = ObjColor.x * LightSourceColor.x * ambientStrength;
  Color.y = ObjColor.y * LightSourceColor.y * ambientStrength;
  Color.z = ObjColor.z * LightSourceColor.z * ambientStrength;
  
  return Color;
}

Vec4 diffusalLightShader(const Vec4 &ObjColor, const Vec4 &LightColor, const Vec4 &LightDir, const Vec4 &Norm)
{
  float diff = std::max(0.f, LightDir.x * Norm.x + LightDir.y * Norm.y + LightDir.z * Norm.z);
  Vec4 Res;

  Res.x = diff * LightColor.x * ObjColor.x;
  Res.y = diff * LightColor.y * ObjColor.y;
  Res.z = diff * LightColor.z * ObjColor.z;

  return Res;
}

Vec4 CubeStartColorShader(const float col[3][4], const float &w0, 
  const float &w1, const float &w2, const float &w)
{
  float red = 1;
  float green = 0;
  float blue = 0;

  Vec4 Res = {255 * red, 255 * green, 255 * blue, 0};
  
  return Res;
}

Vec4 colorCubeLightSourceShader(const float col[3][4], const float &w0, 
  const float &w1, const float &w2, const float &w)
{
  float red = 1;
  float green = 1;
  float blue = 1;

  Vec4 Res = {255 * red, 255 * green, 255 * blue, 0};
  
  return Res;
}

void colorBarShader(const float col[3][4], const float &w0,
  const float &w1, const float &w2, const float &w, Color& color)
{
  float red = w0;
  float green = w1;
  float blue = w2;

  color.red = 255 * red;
  color.green = 255 * green;
  color.blue = 255 * blue;
}

Vec4 colorSideShader(const float col[3][4], const float &w0, 
  const float &w1, const float &w2, const float &w)
{
  float eps = 0.03;
  Vec4 Res {0,0,0,0};

  if (w0 <= eps || w1 <= eps || w2 <= eps) {
    Res.x = 0;
    Res.y = 255;
    Res.z = 0;
  }

  return Res;
}