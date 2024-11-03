#include <mygl.h>

extern MyGL *mygl;

typedef MyGL_Vec2 mygl_vec2;
typedef MyGL_Vec3 mygl_vec3;
typedef MyGL_Vec4 mygl_vec4;
typedef MyGL_Mat2 mygl_mat2;
typedef MyGL_Mat3 mygl_mat3;
typedef MyGL_Mat4 mygl_mat4;

extern mygl_vec3 mygl_v3zero();
extern mygl_vec3 mygl_v3x();
extern mygl_vec3 mygl_v3y();
extern mygl_vec3 mygl_v3z();

//extern mygl_vec3 v3r();
//extern mygl_vec3 v3l();
//extern mygl_vec3 v3u();

extern mygl_vec2 mygl_v2(float x, float y);

extern mygl_vec2 mygl_v2transf(mygl_mat2 M, mygl_vec2 v);

extern mygl_vec3 mygl_v3(float x, float y, float z);

extern mygl_vec3 mygl_v3add(mygl_vec3 lhs, mygl_vec3 rhs);

extern mygl_vec3 mygl_v3sub(mygl_vec3 lhs, mygl_vec3 rhs);

extern mygl_vec3 mygl_v3scale(mygl_vec3 v, float s);

extern float mygl_v3dot(mygl_vec3 lhs, mygl_vec3 rhs);

extern float mygl_v3mag(mygl_vec3 v);

extern mygl_vec3 mygl_v3norm(mygl_vec3 v);

extern mygl_vec3 mygl_v3cross(mygl_vec3 lhs, mygl_vec3 rhs);

extern mygl_vec3 mygl_v3rot(mygl_vec3 p, mygl_vec3 axis, float radians);

#define mygl_v3point( a, b ) mygl_v3sub( b, a )

extern mygl_vec4 mygl_v4(float x, float y, float z, float w);

extern mygl_vec4 mygl_v4scale(mygl_vec4 v, float s);

extern mygl_mat2 mygl_m2inv(mygl_mat2 M);

extern mygl_mat4 mygl_m4ident();
extern mygl_mat4 mygl_m4mul(mygl_mat4 lhs, mygl_mat4 rhs);

extern mygl_mat4 mygl_m4persp(float aspect, float fov_radians, float min, float max);
extern mygl_mat4 mygl_m4ortho(uint32_t w, uint32_t h, float min, float max);

extern mygl_mat4 mygl_m4view(mygl_vec3 p, mygl_vec3 r, mygl_vec3 l, mygl_vec3 u);
extern mygl_mat4 mygl_m4world(mygl_vec3 p, mygl_vec3 r, mygl_vec3 l, mygl_vec3 u);

typedef MyGL_Str64 mygl_str64;
extern mygl_str64 mygl_s64(const char *str);
extern mygl_str64 mygl_s64cat(mygl_str64 s, const char *str);
