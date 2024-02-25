#include "mygl.h"
#include <vecdefs.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

MyGL *mygl = NULL;

MYGLSTRNFUNCS(64);

Vec2 v2_(float x, float y) {
  return MyGL_vec2(x, y);
}

Vec2 v2transf(Mat2 M, Vec2 v) {
  return MyGL_vec2Transf(M, v);
}

static Vec3 vec3XAxis = { { { 1.0f, 0.0f, 0.0f } } };
static Vec3 vec3YAxis = { { { 0.0f, 1.0f, 0.0f } } };
static Vec3 vec3ZAxis = { { { 0.0f, 0.0f, 1.0f } } };

static Vec3 vec3RAxis = { { { 0.0f, -1.0f, 0.0f } } };
static Vec3 vec3FAxis = { { { 1.0f, 0.0f, 0.0f } } };
static Vec3 vec3UAxis = { { { 0.0f, 0.0f, 1.0f } } };

Vec3 v3_(float x, float y, float z) {
  return MyGL_vec3(x, y, z);
}

Vec3 v3zero() {
  return MyGL_vec3Zero;
}

Vec3 v3x() {
  return vec3XAxis;
}
Vec3 v3y() {
  return vec3YAxis;
}
Vec3 v3z() {
  return vec3ZAxis;
}

Vec3 vec3R() {
  return vec3RAxis;
}

Vec3 vec3L() {
  return vec3FAxis;
}

Vec3 vec3U() {
  return vec3UAxis;
}

Vec3 v3add(Vec3 lhs, Vec3 rhs) {
  return MyGL_vec3Add(lhs, rhs);
}

Vec3 v3sub(Vec3 lhs, Vec3 rhs) {
  return MyGL_vec3Sub(lhs, rhs);
}

Vec3 v3scale(Vec3 v, float s) {
  return MyGL_vec3Scale(v, s);
}

float v3dot(Vec3 lhs, Vec3 rhs) {
  return MyGL_vec3Dot(lhs, rhs);
}

float v3mag(Vec3 v) {
  return MyGL_vec3Mag(v);
}

Vec3 v3norm(Vec3 v) {
  return MyGL_vec3Norm(v);
}

Vec3 v3cross(Vec3 lhs, Vec3 rhs) {
  return MyGL_vec3Cross(lhs, rhs);
}

Vec3 v3rot(Vec3 p, Vec3 axis, float radians) {
  return MyGL_vec3Rotate(p, axis, radians);
}

Vec4 v4_(float x, float y, float z, float w) {
  return MyGL_vec4(x, y, z, w);
}

Vec4 v4scale(Vec4 v, float s) {
  return MyGL_vec4Scale(v, s);
}

Mat2 m2inv(Mat2 M) {
  return MyGL_mat2Inv(M);
}

Mat4 m4ident() {
  return MyGL_mat4Identity;
}

Mat4 m4mul(Mat4 lhs, Mat4 rhs) {
  return MyGL_mat4Multiply(lhs, rhs);
}

Mat4 m4persp(float aspect, float fov_radians, float min, float max) {
  return MyGL_mat4Perspective(aspect, fov_radians, min, max);
}
Mat4 m4ortho(uint32_t w, uint32_t h, float min, float max) {
  return MyGL_mat4Ortho(w, h, min, max);
}

Mat4 m4world(Vec3 p, Vec3 r, Vec3 l, Vec3 u) {
  return MyGL_mat4World(p, r, l, u);
}

Mat4 m4view(Vec3 p, Vec3 r, Vec3 l, Vec3 u) {
  return MyGL_mat4View(p, r, l, u);
}

