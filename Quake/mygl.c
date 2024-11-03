#include "mygl.h"
#include <vecdefs.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

MyGL *mygl = NULL;

MYGLSTRNFUNCS(64);

mygl_vec2 mygl_v2(float x, float y) {
  return MyGL_vec2(x, y);
}

mygl_vec2 mygl_v2transf(mygl_mat2 M, mygl_vec2 v) {
  return MyGL_vec2Transf(M, v);
}

static mygl_vec3 axis_r = { { { 0.0f, -1.0f, 0.0f } } };
static mygl_vec3 axis_f = { { { 1.0f, 0.0f, 0.0f } } };
static mygl_vec3 axis_u = { { { 0.0f, 0.0f, 1.0f } } };

mygl_vec3 mygl_v3(float x, float y, float z) {
  return MyGL_vec3(x, y, z);
}

mygl_vec3 mygl_v3zero() {
  return MyGL_vec3Zero;
}

mygl_vec3 mygl_v3x() {
  return MyGL_vec3X;
}
mygl_vec3 mygl_v3y() {
  return MyGL_vec3Y;
}
mygl_vec3 mygl_v3z() {
  return MyGL_vec3Z;
}

mygl_vec3 v3r() {
  return axis_r;
}

mygl_vec3 v3l() {
  return axis_f;
}

mygl_vec3 v3u() {
  return axis_u;
}

mygl_vec3 mygl_v3add(mygl_vec3 lhs, mygl_vec3 rhs) {
  return MyGL_vec3Add(lhs, rhs);
}

mygl_vec3 mygl_v3sub(mygl_vec3 lhs, mygl_vec3 rhs) {
  return MyGL_vec3Sub(lhs, rhs);
}

mygl_vec3 mygl_v3scale(mygl_vec3 v, float s) {
  return MyGL_vec3Scale(v, s);
}

float mygl_v3dot(mygl_vec3 lhs, mygl_vec3 rhs) {
  return MyGL_vec3Dot(lhs, rhs);
}

float mygl_v3mag(mygl_vec3 v) {
  return MyGL_vec3Mag(v);
}

mygl_vec3 mygl_v3norm(mygl_vec3 v) {
  return MyGL_vec3Norm(v);
}

mygl_vec3 mygl_v3cross(mygl_vec3 lhs, mygl_vec3 rhs) {
  return MyGL_vec3Cross(lhs, rhs);
}

mygl_vec3 mygl_v3rot(mygl_vec3 p, mygl_vec3 axis, float radians) {
  return MyGL_vec3Rotate(p, axis, radians);
}

mygl_vec4 mygl_v4(float x, float y, float z, float w) {
  return MyGL_vec4(x, y, z, w);
}

mygl_vec4 mygl_v4scale(mygl_vec4 v, float s) {
  return MyGL_vec4Scale(v, s);
}

mygl_mat2 mygl_m2inv(mygl_mat2 M) {
  return MyGL_mat2Inv(M);
}

mygl_mat4 mygl_m4ident() {
  return MyGL_mat4Identity;
}

mygl_mat4 mygl_m4mul(mygl_mat4 lhs, mygl_mat4 rhs) {
  return MyGL_mat4Multiply(lhs, rhs);
}

mygl_mat4 mygl_m4persp(float aspect, float fov_radians, float min, float max) {
  return MyGL_mat4Perspective(aspect, fov_radians, min, max);
}
mygl_mat4 mygl_m4ortho(uint32_t w, uint32_t h, float min, float max) {
  return MyGL_mat4Ortho(w, h, min, max);
}

mygl_mat4 mygl_m4world(mygl_vec3 p, mygl_vec3 r, mygl_vec3 l, mygl_vec3 u) {
  return MyGL_mat4World(p, r, l, u);
}

mygl_mat4 mygl_m4view(mygl_vec3 p, mygl_vec3 r, mygl_vec3 l, mygl_vec3 u) {
  return MyGL_mat4View(p, r, l, u);
}

