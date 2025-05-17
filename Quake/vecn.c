#include <vecdefs.h>
#include "vecn.h"

typedef union {
  MyGL_Vec2 v_;
  vec2 v;
} vec2_u;

typedef union {
  MyGL_Vec3 v_;
  vec3 v;
} vec3_u;

typedef union {
  MyGL_Vec4 v_;
  vec4 v;
} vec4_u;

typedef union {
  MyGL_Mat2 m_;
  mat2 m;
} mat2_u;

typedef union {
  MyGL_Mat3 m_;
  mat3 m;
} mat3_u;

typedef union {
  MyGL_Mat4 m_;
  mat4 m;
} mat4_u;
//
//#define TO_MGLV2(v) ((vec2_u){ .v = (v) }.v_)
//#define FROM_MGLV2(v_) ((vec2_u){ .v_ = (v_) }.v)
//
//#define TO_MGLV3(v) ((vec3_u){ .v = (v) }.v_)
//#define FROM_MGLV3(v_) ((vec3_u){ .v_ = (v_) }.v)
//
//#define TO_MGLV4(v) ((vec4_u){ .v = (v) }.v_)
//#define FROM_MGLV4(v_) ((vec4_u){ .v_ = (v_) }.v)
//
//#define TO_MGLM2(m) ((mat2_u){ .m = (m) }.m_)
//#define FROM_MGLM2(m_) ((mat2_u){ .m_ = (m_) }.m)
//
//#define TO_MGLM3(m) ((mat3_u){ .m = (m) }.m_)
//#define FROM_MGLM3(m_) ((mat3_u){ .m_ = (m_) }.m)
//
//#define TO_MGLM4(m) ((mat4_u){ .m = (m) }.m_)
//#define FROM_MGLM4(m_) ((mat4_u){ .m_ = (m_) }.m)

#define TO_MGLV2(v) (*((MyGL_Vec2*)(void*)&(v)))
#define FROM_MGLV2(v_) (*((vec2*)(void*)&(v_)))

#define TO_MGLV3(v) (*((MyGL_Vec3*)(void*)&(v)))
#define FROM_MGLV3(v_) (*((vec3*)(void*)&(v_)))

#define TO_MGLV4(v) (*((MyGL_Vec4*)(void*)&(v)))
#define FROM_MGLV4(v_) (*((vec4*)(void*)&(v_)))

#define TO_MGLM2(m) (*((MyGL_Mat2*)(void*)&(m)))
#define FROM_MGLM2(m_) (*((mat2*)(void*)&(m_)))

#define TO_MGLM3(m) (*((MyGL_Mat3*)(void*)&(m)))
#define FROM_MGLM3(m_) (*((mat3*)(void*)&(m_)))

#define TO_MGLM4(m) (*((MyGL_Mat4*)(void*)&(m)))
#define FROM_MGLM4(m_) (*((mat4*)(void*)&(m_)))

vec2 v2sub(vec2 lhs, vec2 rhs) {
  vec2_u r;
  r.v_ = MyGL_vec2Sub(TO_MGLV2(lhs), TO_MGLV2(rhs));
  return r.v;
}
vec2 v2add(vec2 lhs, vec2 rhs) {
  vec2_u r;
  r.v_ = MyGL_vec2Add(TO_MGLV2(lhs), TO_MGLV2(rhs));
  return r.v;
}

vec2 v2scale(vec2 v, float s) {
  vec2_u r;
  r.v_ = MyGL_vec2Scale(TO_MGLV2(v), s);
  return r.v;
}

float v2dot(vec2 lhs, vec2 rhs) {
  return MyGL_vec2Dot(TO_MGLV2(lhs), TO_MGLV2(rhs));
}

float v2mag(vec2 v) {
  return MyGL_vec2Mag(TO_MGLV2(v));
}

vec2 v2norm(vec2 v) {
  vec2_u r;
  r.v_ = MyGL_vec2Norm(TO_MGLV2(v));
  return r.v;
}

vec2 v2set(float x, float y) {
  vec2_u r;
  r.v_ = MyGL_vec2(x, y);
  return r.v;
}

vec2 v2transf(mat2 M, vec2 v) {
  vec2_u r;
  r.v_ = MyGL_vec2Transf(TO_MGLM2(M), TO_MGLV2(v));
  return r.v;
}

vec3 v3set(float x, float y, float z) {
  vec3_u r;
  r.v_ = MyGL_vec3(x, y, z);
  return r.v;
}

vec3 v3zero() {
  vec3_u r;
  r.v_ = MyGL_vec3Zero;
  return r.v;
}

vec3 v3x() {
  vec3_u r;
  r.v_ = MyGL_vec3X;
  return r.v;
}

vec3 v3y() {
  vec3_u r;
  r.v_ = MyGL_vec3Y;
  return r.v;
}

vec3 v3z() {
  vec3_u r;
  r.v_ = MyGL_vec3Z;
  return r.v;
}

vec3 v3add(vec3 lhs, vec3 rhs) {
  vec3_u r;
  r.v_ = MyGL_vec3Add(TO_MGLV3(lhs), TO_MGLV3(rhs));
  return r.v;
}

vec3 v3sub(vec3 lhs, vec3 rhs) {
  vec3_u r;
  r.v_ = MyGL_vec3Sub(TO_MGLV3(lhs), TO_MGLV3(rhs));
  return r.v;
}

vec3 v3scale(vec3 v, float s) {
  vec3_u r;
  r.v_ = MyGL_vec3Scale(TO_MGLV3(v), s);
  return r.v;
}

float v3dot(vec3 lhs, vec3 rhs) {
  return MyGL_vec3Dot(TO_MGLV3(lhs), TO_MGLV3(rhs));
}

float v3mag(vec3 v) {
  return MyGL_vec3Mag(TO_MGLV3(v));
}

vec3 v3norm(vec3 v) {
  vec3_u r;
  r.v_ = MyGL_vec3Norm(TO_MGLV3(v));
  return r.v;
}

vec3 v3cross(vec3 lhs, vec3 rhs) {
  vec3_u r;
  r.v_ = MyGL_vec3Cross(TO_MGLV3(lhs), TO_MGLV3(rhs));
  return r.v;
}

vec3 v3rot(vec3 p, vec3 axis, float radians) {
  vec3_u r;
  r.v_ = MyGL_vec3Rotate(TO_MGLV3(p), TO_MGLV3(axis), radians);
  return r.v;
}

vec4 v4set(float x, float y, float z, float w) {
  vec4_u r;
  r.v_ = MyGL_vec4(x, y, z, w);
  return r.v;
}

vec4 v4scale(vec4 v, float s) {
  vec4_u r;
  r.v_ = MyGL_vec4Scale(TO_MGLV4(v), s);
  return r.v;
}

mat2 m2inv(mat2 M) {
  mat2_u r;
  r.m_ = MyGL_mat2Inv(TO_MGLM2(M));
  return r.m;
}

mat4 m4ident() {
  mat4_u r;
  r.m_ = MyGL_mat4Identity;
  return r.m;
}

mat4 m4mul(mat4 lhs, mat4 rhs) {
  mat4_u r;
  r.m_ = MyGL_mat4Multiply(TO_MGLM4(lhs), TO_MGLM4(rhs));
  return r.m;
}

mat4 m4persp(float aspect, float fov_radians, float min, float max) {
  mat4_u r;
  r.m_ = MyGL_mat4Perspective(aspect, fov_radians, min, max);
  return r.m;
}

mat4 m4ortho(uint32_t w, uint32_t h, float min, float max) {
  mat4_u r;
  r.m_ = MyGL_mat4Ortho(w, h, min, max);
  return r.m;
}

mat4 m4world(vec3 p, vec3 r_, vec3 l, vec3 u) {
  mat4_u r;
  r.m_ = MyGL_mat4World(TO_MGLV3(p), TO_MGLV3(r_), TO_MGLV3(l), TO_MGLV3(u));
  return r.m;
}

mat4 m4view(vec3 p, vec3 r_, vec3 l, vec3 u) {
  mat4_u r;
  r.m_ = MyGL_mat4View(TO_MGLV3(p), TO_MGLV3(r_), TO_MGLV3(l), TO_MGLV3(u));
  return r.m;
}
