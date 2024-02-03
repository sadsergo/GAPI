#include "lgapi.h"
#include <memory>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

Vec2 operator - (const Vec2& a, const Vec2& B)
{
    Vec2 res(a);

    res.x -= B.x;
    res.y -= B.y;

    return res;
}

float E(const Vec2& A, const Vec2& B, const Vec2& P)
{
    return ((P.x - A.x) * (B.y - A.y) - (P.y - A.y) * (B.x - A.x));
}

float E(const Vec2& V, const Vec2& VP)
{
    return (V.x * VP.y - V.y * VP.x);
}

long long F(const Vec2& A, const Vec2& B, const Vec2& P)
{
    return ((A.y - B.y) * P.x + (B.x - A.x) * P.y + (A.x * B.y - A.y * B.x));
}

float find_min_3(float a, float b, float c)
{
    if (a <= b && a <= c) {
        return a;
    }
    else if (b <= a && b <= c) {
        return b;
    }

    return c;
}

float find_max_3(float a, float b, float c)
{
    if (a >= b && a >= c) {
        return a;
    }
    else if (b >= a && b >= c) {
        return b;
    }

    return c;
}

struct MyRender : public IRender
{
    Image2D fb;
    LightObj* L;
    float* depthBuf = nullptr;
    TextureContainer Textures;

    ~MyRender() override {
        if (depthBuf != nullptr) {
            delete[] depthBuf;
            depthBuf = nullptr;
        }
    }

    unsigned int AddImage(const Image2D& a_img) override;

    void BeginRenderPass(Image2D& fb, LightObj* L) override;
    void Draw(PipelineStateObject a_state, Geom a_geom, float angle) override;
    void EndRenderPass(Image2D& fb) override;
};

std::shared_ptr<IRender> MakeMyImpl()
{
    return std::make_shared<MyRender>();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyRender::BeginRenderPass(Image2D& fb, LightObj* L = 0)
{
    this->fb = fb;

    size_t s = fb.width * fb.height;
    this->depthBuf = new float[s];

    for (size_t i = 0; i < s; i++) {
        depthBuf[i] = 0.f;
    }

    if (L != nullptr) {
        this->L = L;
    }
}

unsigned int MyRender::AddImage(const Image2D& a_img)
{
    size_t ind = Textures.addTexture(a_img);

    return ind;
}

bool isInTriangle(Vec4 W)
{
    if ((W.x > 0 && W.y > 0 && W.z > 0) || (W.x < 0 && W.y < 0 && W.z < 0)) {
        return true;
    }

    return false;
}

Vec4 performEdgeFunc(Vec2 A, Vec2 B, Vec2 C, Vec2 Point)
{
    float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);
    Vec4 res = { w0, w1, w2, 0 };

    return res;
}

void MyRender::Draw(PipelineStateObject a_state, Geom a_geom, float angle = 0.0)
{
    float worldViewProj[16];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            worldViewProj[i * 4 + j] = 0.f;
            for (int k = 0; k < 4; k++) {
                worldViewProj[i * 4 + j] += a_state.projMatrix[i * 4 + k] * a_state.worldViewMatrix[k * 4 + j];
            }
        }
    }

    Vec4 vertex_vec;

    float rotatedWorldViewProj[16];
    float rotateMtrx[16] = {
      cos(angle), 0, sin(angle), 0,
      0,          1,          0, 0,
      -sin(angle),0, cos(angle), 0,
      0,          0,          0, 1,
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rotatedWorldViewProj[i * 4 + j] = 0.f;
            for (int k = 0; k < 4; k++) {
                rotatedWorldViewProj[i * 4 + j] += worldViewProj[i * 4 + k] * rotateMtrx[k * 4 + j];
            }
        }
    }

    for (unsigned int tr_num = 0; tr_num < a_geom.primsNum; tr_num++) {
        unsigned vert_indx[3] = {
          a_geom.indices[tr_num * 3 + 0],
          a_geom.indices[tr_num * 3 + 1],
          a_geom.indices[tr_num * 3 + 2]
        };

        float vNDS_i[4];
        float p[3][4] = { {} };
        float col[3][4] = { {} };
        float uv[3][2] = { {} };
        float p_n[3][4] = { {} };

        for (int ind_ver = 0; ind_ver < 3; ++ind_ver) {
            float vpos4f_i[4] = {
              a_geom.vpos4f[4 * vert_indx[ind_ver]],
              a_geom.vpos4f[4 * vert_indx[ind_ver] + 1],
              a_geom.vpos4f[4 * vert_indx[ind_ver] + 2],
              a_geom.vpos4f[4 * vert_indx[ind_ver] + 3],
            };

            if (a_state.shader_container->ambientLightShader != nullptr) {
                for (int i = 0; i < 4; ++i) {
                    p_n[ind_ver][i] = 0;
                    for (int j = 0; j < 4; ++j) {
                        p_n[ind_ver][i] += a_state.worldViewMatrix[4 * i + j] * vpos4f_i[j];
                    }
                }
            }

            vertex_vec = a_state.shader_container->vertexShader
            (rotatedWorldViewProj, a_geom, vpos4f_i);

            p[ind_ver][0] = vertex_vec.x;
            p[ind_ver][1] = vertex_vec.y;
            p[ind_ver][2] = vertex_vec.z;
            p[ind_ver][3] = vertex_vec.w;

            float w = vertex_vec.w;

            for (int i = 0; i < 2; i++) {
                p[ind_ver][i] *= 0.5f;
                p[ind_ver][i] += 0.5f;
            }

            p[ind_ver][0] *= (float)fb.width;
            p[ind_ver][1] *= (float)fb.height;

            for (int i = 0; i < 2; i++) {
                p[ind_ver][i] -= 0.5f;
            }

            p[ind_ver][2] = 1 / w;

            if (a_state.imgId != uint32_t(-1)) {
                uv[ind_ver][0] = a_geom.vtex2f[2 * vert_indx[ind_ver]] / w;
                uv[ind_ver][1] = a_geom.vtex2f[2 * vert_indx[ind_ver] + 1] / w;
            }
            else {
                col[ind_ver][0] = a_geom.vcol4f[4 * vert_indx[ind_ver]] / w;
                col[ind_ver][1] = a_geom.vcol4f[4 * vert_indx[ind_ver] + 1] / w;
                col[ind_ver][2] = a_geom.vcol4f[4 * vert_indx[ind_ver] + 2] / w;
                col[ind_ver][3] = a_geom.vcol4f[4 * vert_indx[ind_ver] + 3] / w;
            }
        }

        Vec2 A(p[0][0], p[0][1]), B(p[1][0], p[1][1]), C(p[2][0], p[2][1]);
        Vec4 LightDir;
        Vec4 Norm;

        if (a_state.shader_container->ambientLightShader != nullptr) {
            Vec4 a = Vec4(p_n[1][0] - p_n[0][0], p_n[1][1] - p_n[0][1], p_n[1][2] - p_n[0][2], 0);
            Vec4 b = Vec4(p_n[2][0] - p_n[1][0], p_n[2][1] - p_n[1][1], p_n[2][2] - p_n[1][2], 0);

            Norm = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
            float norm_len = sqrt(pow(Norm.x, 2) + pow(Norm.y, 2) + pow(Norm.z, 2));

            Norm.x /= norm_len;
            Norm.y /= norm_len;
            Norm.z /= norm_len;

            LightDir = { L->x - p_n[0][0], L->y - p_n[0][1], L->z - p_n[0][2], 0 };
            float lightDir_len = sqrt(pow(LightDir.x, 2) + pow(LightDir.y, 2) + pow(LightDir.z, 2));

            LightDir.x /= lightDir_len;
            LightDir.y /= lightDir_len;
            LightDir.z /= lightDir_len;

            // cout << LightDir.x << " " << LightDir.y << " " << LightDir.z << endl;
        }

        float xmin = find_min_3(p[0][0], p[1][0], p[2][0]), xmax = find_max_3(p[0][0], p[1][0], p[2][0]);
        float ymin = find_min_3(p[0][1], p[1][1], p[2][1]), ymax = find_max_3(p[0][1], p[1][1], p[2][1]);

        int x0 = std::max(0, (int32_t)(xmin)-1), y0 = std::max(0, (int32_t)(ymin - 1));
        int x1 = std::max(0, (int32_t)(xmax)+1), y1 = std::max(0, (int32_t)(ymax + 1));

        x1 = std::min(fb.width, (uint32_t)x1);
        y1 = std::min(fb.height, (uint32_t)y1);

        float e = abs(E(A, B, C));

        int q = 4;
        int blockSize = q;

        if ((x1 - x0) * (y1 - y0) > 120 * blockSize) {
            int halfY = y0 + ((y1 - y0) >> 1) & ~(q - 1);
            Vec2 D = (A.getY() == y0) ? A : (B.getY() == y0) ? B : C;

            int leftPoint = (int)D.getX() - (int)D.getX() % blockSize, rightPoint = leftPoint + 2 * blockSize;

            for (int j = y0; j <= halfY + blockSize; j += blockSize) {
                int midPoint = leftPoint + ((rightPoint - leftPoint) >> 1) & ~(q - 1);
                int x = midPoint;

                for (int k = 0; k < 2; ++k) {
                    for (; (q > 0 ? x < x1 : x > x0 - blockSize); x += q) {
                        float C1x = (x), C1y = (x + blockSize - 1), C2y = j, C2x = (j + blockSize - 1);
                        Vec2 M1(C1x, C2x), M2(C1x, C2x), M3(C1y, C2y), M4(C1y, C2x);
                        bool L1 = isInTriangle(performEdgeFunc(A, B, C, M1)), L2 = isInTriangle(performEdgeFunc(A, B, C, M2));
                        bool L3 = isInTriangle(performEdgeFunc(A, B, C, M3)), L4 = isInTriangle(performEdgeFunc(A, B, C, M4));

                        if (L1 && L2 && L3 && L4) {
                            for (int bl_y = j; bl_y <= j + blockSize && bl_y < fb.height; ++bl_y) {
                                for (int bl_x = x; bl_x <= x + blockSize && bl_x < fb.width; ++bl_x) {
                                    Vec2 Point = Vec2((float)bl_x, (float)bl_y);
                                    float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);

                                    w0 = abs(w0) / e;
                                    w1 = abs(w1) / e;
                                    w2 = abs(w2) / e;

                                    float w = (p[2][2] * w0 + p[0][2] * w1 + p[1][2] * w2);

                                    if (1 / w < 1 / depthBuf[fb.width * bl_y + bl_x]) {
                                        depthBuf[fb.width * bl_y + bl_x] = w;

                                        if (a_state.imgId != uint32_t(-1)) {
                                            fb.data[fb.width * bl_y + bl_x] = a_state.shader_container->textureShader(a_state.imgId, Textures, uv, w0, w1, w2, w);
                                        }
                                        else {
                                            Vec4 colorV = a_state.shader_container->colorShader(col, w0, w1, w2, w);

                                            if (a_state.shader_container->ambientLightShader != nullptr) {
                                                colorV = a_state.shader_container->ambientLightShader(colorV, L->Color);
                                                colorV = a_state.shader_container->diffusalLightShader(colorV, L->Color, LightDir, Norm);
                                            }

                                            uint32_t color = ((unsigned char)(colorV.x) << 16) + ((unsigned char)(colorV.y) << 8) + ((unsigned char)(colorV.z));
                                            fb.data[fb.width * bl_y + bl_x] = color;
                                        }
                                    }
                                }
                            }
                            continue;
                        }
                        else if (!L1 && !L2 && !L3 && !L4) {
                            continue;
                        }

                        for (int bl_y = j; bl_y <= j + blockSize && bl_y < fb.height; ++bl_y) {
                            for (int bl_x = x; bl_x <= x + blockSize && bl_x < fb.width; ++bl_x) {
                                Vec2 Point = Vec2((float)bl_x, (float)bl_y);
                                float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);

                                if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                                    w0 = abs(w0) / e;
                                    w1 = abs(w1) / e;
                                    w2 = abs(w2) / e;

                                    float w = (p[2][2] * w0 + p[0][2] * w1 + p[1][2] * w2);

                                    if (1 / w < 1 / depthBuf[fb.width * bl_y + bl_x]) {
                                        depthBuf[fb.width * bl_y + bl_x] = w;

                                        if (a_state.imgId != uint32_t(-1)) {
                                            fb.data[fb.width * bl_y + bl_x] = a_state.shader_container->textureShader(a_state.imgId, Textures, uv, w0, w1, w2, w);
                                        }
                                        else {
                                            Vec4 colorV = a_state.shader_container->colorShader(col, w0, w1, w2, w);

                                            if (a_state.shader_container->ambientLightShader != nullptr) {
                                                colorV = a_state.shader_container->ambientLightShader(colorV, L->Color);
                                                colorV = a_state.shader_container->diffusalLightShader(colorV, L->Color, LightDir, Norm);
                                            }

                                            uint32_t color = ((unsigned char)(colorV.x) << 16) + ((unsigned char)(colorV.y) << 8) + ((unsigned char)(colorV.z));
                                            fb.data[fb.width * bl_y + bl_x] = color;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    q = -q;

                    if (k == 0) {
                        rightPoint = x - blockSize;
                    }
                    else {
                        leftPoint = x + blockSize;
                    }

                    x = midPoint - blockSize;
                }
            }

            D = (A.getY() == y1) ? A : (B.getY() == y1) ? B : C;
            leftPoint = (int)D.getX() - (int)D.getX() % blockSize - blockSize, rightPoint = leftPoint + 2 * blockSize;

            for (int j = y1 & ~(blockSize - 1); j >= halfY; j -= blockSize) {
                int midPoint = leftPoint + ((rightPoint - leftPoint) >> 1) & ~(q - 1);
                int x = midPoint;

                for (int k = 0; k < 2; ++k) {
                    for (; (q > 0 ? x < x1 : x > x0 - blockSize); x += q) {
                        float C1x = (x), C1y = (x + blockSize - 1), C2y = j, C2x = (j + blockSize - 1);
                        Vec2 M1(C1x, C2x), M2(C1x, C2x), M3(C1y, C2y), M4(C1y, C2x);
                        bool L1 = isInTriangle(performEdgeFunc(A, B, C, M1)), L2 = isInTriangle(performEdgeFunc(A, B, C, M2));
                        bool L3 = isInTriangle(performEdgeFunc(A, B, C, M3)), L4 = isInTriangle(performEdgeFunc(A, B, C, M4));

                        if (!L1 && !L2 && !L3 && !L4) {
                            continue;
                        }
                        else if (L1 && L2 && L3 && L4) {
                            for (int bl_y = j; bl_y <= j + blockSize && bl_y < fb.height; ++bl_y) {
                                for (int bl_x = x; bl_x <= x + blockSize && bl_x < fb.width; ++bl_x) {
                                    Vec2 Point = Vec2((float)bl_x, (float)bl_y);
                                    float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);

                                    w0 = abs(w0) / e;
                                    w1 = abs(w1) / e;
                                    w2 = abs(w2) / e;

                                    float w = (p[2][2] * w0 + p[0][2] * w1 + p[1][2] * w2);

                                    if (1 / w < 1 / depthBuf[fb.width * bl_y + bl_x]) {
                                        depthBuf[fb.width * bl_y + bl_x] = w;

                                        if (a_state.imgId != uint32_t(-1)) {
                                            fb.data[fb.width * bl_y + bl_x] = a_state.shader_container->textureShader(a_state.imgId, Textures, uv, w0, w1, w2, w);
                                        }
                                        else {
                                            Vec4 colorV = a_state.shader_container->colorShader(col, w0, w1, w2, w);

                                            if (a_state.shader_container->ambientLightShader != nullptr) {
                                                colorV = a_state.shader_container->ambientLightShader(colorV, L->Color);
                                                colorV = a_state.shader_container->diffusalLightShader(colorV, L->Color, LightDir, Norm);
                                            }

                                            uint32_t color = ((unsigned char)(colorV.x) << 16) + ((unsigned char)(colorV.y) << 8) + ((unsigned char)(colorV.z));
                                            fb.data[fb.width * bl_y + bl_x] = color;
                                        }
                                    }
                                }
                            }

                            continue;
                        }

                        for (int bl_y = j; bl_y <= j + blockSize && bl_y < fb.height; ++bl_y) {
                            for (int bl_x = x; bl_x <= x + blockSize && bl_x < fb.width; ++bl_x) {
                                Vec2 Point = Vec2((float)bl_x, (float)bl_y);
                                float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);

                                if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                                    w0 = abs(w0) / e;
                                    w1 = abs(w1) / e;
                                    w2 = abs(w2) / e;

                                    float w = (p[2][2] * w0 + p[0][2] * w1 + p[1][2] * w2);

                                    if (1 / w < 1 / depthBuf[fb.width * bl_y + bl_x]) {
                                        depthBuf[fb.width * bl_y + bl_x] = w;

                                        if (a_state.imgId != uint32_t(-1)) {
                                            fb.data[fb.width * bl_y + bl_x] = a_state.shader_container->textureShader(a_state.imgId, Textures, uv, w0, w1, w2, w);
                                        }
                                        else {
                                            Vec4 colorV = a_state.shader_container->colorShader(col, w0, w1, w2, w);

                                            if (a_state.shader_container->ambientLightShader != nullptr) {
                                                colorV = a_state.shader_container->ambientLightShader(colorV, L->Color);
                                                colorV = a_state.shader_container->diffusalLightShader(colorV, L->Color, LightDir, Norm);
                                            }

                                            uint32_t color = ((unsigned char)(colorV.x) << 16) + ((unsigned char)(colorV.y) << 8) + ((unsigned char)(colorV.z));
                                            fb.data[fb.width * bl_y + bl_x] = color;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    q = -q;

                    if (k == 0) {
                        rightPoint = x - blockSize;
                    }
                    else {
                        leftPoint = x + blockSize;
                    }

                    x = midPoint - blockSize;
                }
            }
        }
        else {
            for (uint32_t x = x0; x <= x1 && x < fb.width; x++) {
                for (uint32_t y = y0; y <= y1 && y < fb.height; y++) {
                    Vec2 Point = Vec2((float)x, (float)y);
                    float w0 = E(B - A, Point - A), w1 = E(C - B, Point - B), w2 = E(A - C, Point - C);

                    if ((w0 > 0 && w1 > 0 && w2 > 0) || (w0 < 0 && w1 < 0 && w2 < 0)) {
                        w0 = abs(w0) / e;
                        w1 = abs(w1) / e;
                        w2 = abs(w2) / e;

                        float w = (p[2][2] * w0 + p[0][2] * w1 + p[1][2] * w2);

                        if (1 / w < 1 / depthBuf[fb.width * y + x]) {
                            depthBuf[fb.width * y + x] = w;

                            if (a_state.imgId != uint32_t(-1)) {
                                fb.data[fb.width * y + x] = a_state.shader_container->textureShader(a_state.imgId, Textures, uv, w0, w1, w2, w);
                            }
                            else {
                                Vec4 colorV = a_state.shader_container->colorShader(col, w0, w1, w2, w);

                                if (a_state.shader_container->ambientLightShader != nullptr) {
                                    colorV = a_state.shader_container->ambientLightShader(colorV, L->Color);
                                    colorV = a_state.shader_container->diffusalLightShader(colorV, L->Color, LightDir, Norm);
                                }

                                uint32_t color = ((unsigned char)(colorV.x) << 16) + ((unsigned char)(colorV.y) << 8) + ((unsigned char)(colorV.z));
                                fb.data[fb.width * y + x] = color;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MyRender::EndRenderPass(Image2D& fb)
{
    std::fill(&depthBuf[0], &depthBuf[fb.width * fb.height - 1], 0);
}


