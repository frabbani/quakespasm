#include <mygl.h>

extern MyGL *mygl;

typedef MyGL_Vec2 Vec2;
typedef MyGL_Vec3 Vec3;
typedef MyGL_Vec4 Vec4;
typedef MyGL_Mat2 Mat2;
typedef MyGL_Mat3 Mat3;
typedef MyGL_Mat4 Mat4;

extern Vec3 v3zero();
extern Vec3 v3x();
extern Vec3 v3y();
extern Vec3 v3z();

extern Vec3 v3r();
extern Vec3 v3l();
extern Vec3 v3u();

extern Vec2 v2_(float x, float y);

extern Vec2 v2transf(Mat2 M, Vec2 v);

extern Vec3 v3_(float x, float y, float z);

extern Vec3 v3add(Vec3 lhs, Vec3 rhs);

extern Vec3 v3sub(Vec3 lhs, Vec3 rhs);

extern Vec3 v3scale(Vec3 v, float s);

extern float v3dot(Vec3 lhs, Vec3 rhs);

extern float v3mag(Vec3 v);

extern Vec3 v3norm(Vec3 v);

extern Vec3 v3cross(Vec3 lhs, Vec3 rhs);

extern Vec3 v3rot(Vec3 p, Vec3 axis, float radians);

#define v3make( a, b ) v3sub( b, a )

extern Vec4 v4_(float x, float y, float z, float w);

extern Vec4 v4scale(Vec4 v, float s);

extern Mat2 m2inv(Mat2 M);

extern Mat4 m4ident();
extern Mat4 m4mul(Mat4 lhs, Mat4 rhs);

extern Mat4 m4persp(float aspect, float fov_radians, float min, float max);
extern Mat4 m4ortho(uint32_t w, uint32_t h, float min, float max);

extern Mat4 m4view(Vec3 p, Vec3 r, Vec3 l, Vec3 u);
extern Mat4 m4world(Vec3 p, Vec3 r, Vec3 l, Vec3 u);

typedef MyGL_Str64 Str64;
#define str64 MyGL_str64
#define str64cat MyGL_str64Cat
#define str64fmt MyGL_str64Fmt
