#include <cmath> 
#include <cstdint> 
#include <iostream> 
 
struct float3 
{ 
public: 
    float x{ 0 }, y{ 0 } , z{ 0 }; 
    float3 operator - (const float3& v) const 
    { return float3{ x - v.x, y - v.y, z - v.z }; } 
}; 
 
void normalize(float3& v) 
{ 
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
    std::cout << v.x << " " << v.y << " " << v.z << "\n"; 
    v.x /= len, v.y /= len, v.z /= len; 
    std::cout << v.x << " " << v.y << " " << v.z << "\n"; 
} 
 
float3 cross(const float3& a, const float3& b) 
{ 
    return { 
        a.y * b.z - a.z * b.y, 
        a.z * b.x - a.x * b.z, 
        a.x * b.y - a.y * b.x 
    }; 
} 
 
struct mat4 
{ 
public: 
    float m[4][4] = {{1, 0, 0, 0},  {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; 
    float* operator [] (uint8_t i) { return m[i]; } 
    const float* operator [] (uint8_t i) const { return m[i]; } 
    friend std::ostream& operator << (std::ostream& os, const mat4& m) 
    { 
        return os << std::endl << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << std::endl
                  << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << std::endl
                  << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << std::endl
                  << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3]; 
    } 
 
}; 
 
void lookat(const float3& from, const float3& to, const float3& up, mat4& m) 
{ 
    float3 forward = from - to; 
    normalize(forward); 
    float3 right = cross(up, forward); 
    normalize(right); 
    float3 newup = cross(forward, right); 
 
    m[0][0] = right.x,   m[0][1] = right.y,   m[0][2] = right.z; 
    m[1][0] = newup.x,   m[1][1] = newup.y,   m[1][2] = newup.z; 
    m[2][0] = forward.x, m[2][1] = forward.y, m[2][2] = forward.z; 
    m[3][0] = from.x,    m[3][1] = from.y,    m[3][2] = from.z; 
} 
 
 
int main() 
{ 
    mat4 m; 
 
    float3 up{ 0, 1, 0 }; 
    float3 from{ 1, 1, 1 }; 
    float3 to{ 0, 0, 0 }; 
 
    lookat(from, to, up, m); 
 
    std::cout << m << std::endl; 
 
    return 0; 
} 