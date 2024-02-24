#include <mygl.h>

extern MyGL *mygl;

typedef MyGL_Vec2 Vec2;
typedef MyGL_Vec3 Vec3;
typedef MyGL_Vec4 Vec4;
typedef MyGL_Mat2 Mat2;
typedef MyGL_Mat3 Mat3;
typedef MyGL_Mat4 Mat4;

extern Vec3 vec3Zero();
extern Vec3 vec3X();
extern Vec3 vec3Y();
extern Vec3 vec3Z();
extern Vec3 vec3R();
extern Vec3 vec3L();
extern Vec3 vec3U();
#define vec3F vec3L

extern Vec2 vec2_(float x, float y);

extern Vec2 vec2Transf(Mat2 M, Vec2 v);

extern Vec3 vec3_(float x, float y, float z);

extern Vec3 vec3Add(Vec3 lhs, Vec3 rhs);

extern Vec3 vec3Sub(Vec3 lhs, Vec3 rhs);

extern Vec3 vec3Scale(Vec3 v, float s);

extern float vec3Dot(Vec3 lhs, Vec3 rhs);

extern float vec3Mag(Vec3 v);

extern Vec3 vec3Norm(Vec3 v);

extern Vec3 vec3Cross(Vec3 lhs, Vec3 rhs);

extern Vec3 vec3Rot(Vec3 p, Vec3 axis, float radians);

#define vec3Make( a, b ) vec3Sub( b, a )

extern Vec4 vec4_(float x, float y, float z, float w);

extern Vec4 vec4Scale(Vec4 v, float s);

extern Mat2 mat2Inv(Mat4 M);

extern Mat4 mat4I();
extern Mat4 mat4Mul(Mat4 lhs, Mat4 rhs);

extern Mat4 mat4Persp(float aspect, float fovInRadians, float min, float max);
extern Mat4 mat4Ortho(uint32_t w, uint32_t h, float min, float max);

extern Mat4 mat4View(Vec3 p, Vec3 r, Vec3 l, Vec3 u);
extern Mat4 mat4World(Vec3 p, Vec3 r, Vec3 l, Vec3 u);

typedef MyGL_Str64 Str64;
#define str64 MyGL_str64
#define str64Cat MyGL_str64Cat
#define str64Fmt MyGL_str64Fmt
