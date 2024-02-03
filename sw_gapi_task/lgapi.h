#pragma once 

#include <vector>
#include <cstddef>
#include <iostream>
#include <cmath>

#include "./structs/structs.h"

enum RENDER_MODE {
    MODE_VERT_COLOR = 0,
    MODE_TEXURE_3D = 1,
};



enum GEOM_TYPE { GEOM_TRIANGLES = 1, GEOM_QUADS = 2 };

struct Geom
{
    const float* vpos4f = nullptr; ///< quads of vertex coordinates for vertex positions x,y,z,w; w is unused
    const float* vcol4f = nullptr; ///< quads of vertex color attributes r,g,b,a; a is unused
    const float* vtex2f = nullptr; ///< pairs of x and y coordinates
    const unsigned int* indices = nullptr; ///< index buffer of size 3*primsNum for triangle meshes and 4*primsNum for qual meshes

    unsigned int vertNum = 0;
    unsigned int primsNum = 0;
    GEOM_TYPE    geomType = GEOM_TRIANGLES;
};

struct Image2D
{
    Image2D() {}
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

    size_t addTexture(const Image2D& tex)
    {
        textures.push_back(tex);

        return textures.size() - 1;
    }
};

struct ShaderContainer {
    Vec4(*vertexShader)(const float*, const Geom&, const float*);
    uint32_t(*textureShader)(const int&, const TextureContainer&, const float[3][2], const float&, const float&, const float&, const float&);
    Vec4(*colorShader)(const float[3][4], const float&, const float&, const float&, const float&);
    uint32_t(*lightShader)(const float col[3][4], const float& w0, const float& w1, const float& w2, const float& w);
    Vec4(*ambientLightShader)(const Vec4& ObjColor, const Vec4& LightSourceColor);
    Vec4(*diffusalLightShader)(const Vec4& ObjColor, const Vec4& LightColor, const Vec4& LightDir, const Vec4& Norm);
};

struct PipelineStateObject
{
    float worldViewMatrix[16]; ///< assume row-major layout, i.e. M[0], M[1], M[2], M[3] is the first row of the matrix
    float projMatrix[16];      ///< assume row-major layout, i.e. M[0], M[1], M[2], M[3] is the first row of the matrix
    RENDER_MODE  mode = MODE_VERT_COLOR;
    unsigned int imgId = 0;

    ShaderContainer* shader_container;
};

struct IRender
{
    IRender() {}
    virtual ~IRender() {}

    virtual unsigned int AddImage(const Image2D& a_img) = 0;

    virtual void BeginRenderPass(Image2D& fb, LightObj* L = 0) = 0;
    virtual void Draw(PipelineStateObject a_state, Geom a_geom, float angle = 0.0) = 0;
    virtual void EndRenderPass(Image2D& fb) = 0;
};