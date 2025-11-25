#include "vecn.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#ifndef EPSILON
#define EPSILON 1e-7f
#endif

#ifndef SWAP
#define SWAP(a,b) do { __typeof__(a) t = (a); (a) = (b); (b) = t; } while(0)
#endif

#define KM3(m) ( (const vec3_t*)(const void*)m )
#define KM4(m) ( (const vec4_t*)(const void*)m )

//FLOAT2
void v2zero(vec2_t v2) {
  v2[0] = v2[1] = 0.0f;
}

void v2set(vec2_t v2, float x, float y) {
  v2[0] = x;
  v2[1] = y;
}

int v2eq(const vec2_t v2u, const vec2_t v2v) {
  return fabsf(v2u[0] - v2v[0]) < EPSILON && fabsf(v2u[1] - v2v[1]) < EPSILON;
}

void v2copy(vec2_t v2dst, const vec2_t v2src) {
  v2dst[0] = v2src[0];
  v2dst[1] = v2src[1];
}

void v2add(vec2_t v2to, const vec2_t v2v) {
  v2to[0] += v2v[0];
  v2to[1] += v2v[1];
}

void v2sub(vec2_t v2from, const vec2_t v2v) {
  v2from[0] -= v2v[0];
  v2from[1] -= v2v[1];
}

void v2muls(vec2_t v2, float s) {
  v2[0] *= s;
  v2[1] *= s;
}

void v2madd(vec2_t v2to, float s, const vec2_t v2v) {
  v2to[0] += s * v2v[0];
  v2to[1] += s * v2v[1];
}

void v2make(vec2_t v2, const vec2_t v2from, const vec2_t v2to) {
  v2[0] = v2to[0] - v2from[0];
  v2[1] = v2to[1] - v2from[1];
}

float v2mag(const vec2_t v2) {
  return (sqrtf(v2[0] * v2[0] + v2[1] * v2[1]));
}

float v2norm(vec2_t v2) {
  float m = v2mag(v2);
  v2muls(v2, 1.0f / m);
  return m;
}

void v2lerp(vec2_t v2, const vec2_t v2u, const vec2_t v2v, float mu) {
  float nu = 1.0 - mu;
  v2[0] = nu * v2u[0] + mu * v2v[0];
  v2[1] = nu * v2u[1] + mu * v2v[1];
}

float v2dot(const vec2_t v2u, const vec2_t v2v) {
  return (v2u[0] * v2v[0] + v2u[1] * v2v[1]);
}

void v2perp(vec2_t v2, const vec2_t v2u) {
  v2[0] = -v2u[1];
  v2[1] = v2u[0];
}

int v3eq(const vec3_t v3u, const vec3_t v3v) {
  return fabsf(v3u[0] - v3v[0]) < EPSILON && fabsf(v3u[1] - v3v[1]) < EPSILON && fabsf(v3u[2] - v3v[2]) < EPSILON;
}

void v3zero(vec3_t v3) {
  v3[0] = v3[1] = v3[2] = (float) 0;
}

void v3set(vec3_t v3, float x, float y, float z) {
  v3[0] = x;
  v3[1] = y;
  v3[2] = z;
}

void v3copy(vec3_t v3dst, const vec3_t v3src) {
  v3dst[0] = v3src[0];
  v3dst[1] = v3src[1];
  v3dst[2] = v3src[2];
}

void v3add(vec3_t v3to, const vec3_t v3val) {
  v3to[0] += v3val[0];
  v3to[1] += v3val[1];
  v3to[2] += v3val[2];
}

void v3sub(vec3_t v3from, const vec3_t v3val) {
  v3from[0] -= v3val[0];
  v3from[1] -= v3val[1];
  v3from[2] -= v3val[2];
}

void v3mul(vec3_t v3to, const vec3_t v3val) {
  v3to[0] *= v3val[0];
  v3to[1] *= v3val[1];
  v3to[2] *= v3val[2];
}

void v3div(vec3_t v3to, const vec3_t v3val) {
  v3to[0] /= v3val[0];
  v3to[1] /= v3val[1];
  v3to[2] /= v3val[2];
}

void v3muls(vec3_t v3, float s) {
  v3[0] *= s;
  v3[1] *= s;
  v3[2] *= s;
}

void v3madd(vec3_t v3, float s, const vec3_t v3u) {
  v3[0] += s * v3u[0];
  v3[1] += s * v3u[1];
  v3[2] += s * v3u[2];
}

void v3cadd(vec3_t v3, const vec3_t v3val1, const vec3_t v3val2) {
  v3[0] = v3val1[0] + v3val2[0];
  v3[1] = v3val1[1] + v3val2[1];
  v3[2] = v3val1[2] + v3val2[2];
}

void v3make(vec3_t v3, const vec3_t v3from, const vec3_t v3to) {
  v3[0] = v3to[0] - v3from[0];
  v3[1] = v3to[1] - v3from[1];
  v3[2] = v3to[2] - v3from[2];
}

float v3mag(const vec3_t v3) {
  return (sqrtf(v3[0] * v3[0] + v3[1] * v3[1] + v3[2] * v3[2]));
}

float v3norm(vec3_t v3) {
  float m = v3mag(v3);
  v3muls(v3, 1.0f / m);
  return m;
}

void v3lerp(vec3_t v3, const vec3_t v3u, const vec3_t v3v, float mu) {
  float nu = 1.0 - mu;
  v3[0] = nu * v3u[0] + mu * v3v[0];
  v3[1] = nu * v3u[1] + mu * v3v[1];
  v3[2] = nu * v3u[2] + mu * v3v[2];
}

float v3dot(const vec3_t v3u, const vec3_t v3v) {
  return (v3u[0] * v3v[0] + v3u[1] * v3v[1] + v3u[2] * v3v[2]);
}


void v3cross(const vec3_t v3u, const vec3_t v3v, vec3_t v3n) {
  v3n[0] = (v3u[1] * v3v[2] - v3u[2] * v3v[1]);
  v3n[1] = -(v3u[0] * v3v[2] - v3u[2] * v3v[0]);
  v3n[2] = (v3u[0] * v3v[1] - v3u[1] * v3v[0]);
}

void v3transf(const mat3_t v3x3, vec3_t v3) {
  vec3_t v3t;
  v3copy(v3t, v3);
  v3[0] = v3dot(v3x3[0], v3t);
  v3[1] = v3dot(v3x3[1], v3t);
  v3[2] = v3dot(v3x3[2], v3t);
}

void m3copy(mat3_t m3dst, const mat3_t m3src) {
  memcpy(m3dst, m3src, sizeof(mat3_t));
}

void m3ident(mat3_t m3) {
  m3[0][0] = m3[1][1] = m3[2][2] = (float) 1;
  m3[0][1] = m3[0][2] = (float) 0;
  m3[1][0] = m3[1][2] = (float) 0;
  m3[2][0] = m3[2][1] = (float) 0;
}

void m3skewsymcross(mat3_t m3, const vec3_t u) {
  m3[0][0] = m3[1][1] = m3[2][2] = (float) 0;
  m3[0][1] = -u[2];
  m3[1][0] = u[2];
  m3[0][2] = u[1];
  m3[2][0] = -u[1];
  m3[1][2] = -u[0];
  m3[2][1] = u[0];
}

void m3transp(mat3_t m3) {
  SWAP(m3[0][1], m3[1][0]);
  SWAP(m3[0][2], m3[2][0]);
  SWAP(m3[1][2], m3[2][1]);
}

void m3transf(const mat3_t m3a, const mat3_t m3b, mat3_t m3) {
  mat3_t m3t;

  m3copy(m3t, m3b);
  m3transp(m3t);

  m3[0][0] = v3dot(m3a[0], m3t[0]);
  m3[0][1] = v3dot(m3a[0], m3t[1]);
  m3[0][2] = v3dot(m3a[0], m3t[2]);

  m3[1][0] = v3dot(m3a[1], m3t[0]);
  m3[1][1] = v3dot(m3a[1], m3t[1]);
  m3[1][2] = v3dot(m3a[1], m3t[2]);

  m3[2][0] = v3dot(m3a[2], m3t[0]);
  m3[2][1] = v3dot(m3a[2], m3t[1]);
  m3[2][2] = v3dot(m3a[2], m3t[2]);
}

float m3det(mat3_t m3) {
//
  return m3[0][0] * (m3[1][1] * m3[2][2] - m3[1][2] * m3[2][1]) - m3[0][1] * (m3[1][0] * m3[2][2] - m3[1][2] * m3[2][0]) + m3[0][2] * (m3[1][0] * m3[2][1] - m3[1][1] * m3[2][0]);
}

void m3ypr(mat3_t m3, float y, float p, float r, int in_degs) {
  if (in_degs) {
    y = y * M_PI / 180.0f;
    p = p * M_PI / 180.0f;
    r = r * M_PI / 180.0f;
  }

  float cp = cosf(p);
  float sp = sinf(p);

  float cy = cosf(y);
  float sy = sinf(y);

  float cr = cosf(r);
  float sr = sinf(r);

  mat3_t m3r, m3p, m3y, m3pr;

  v3set(m3r[0], cr, 0.0f, -sr);
  v3set(m3r[1], 0.0f, 1.0f, 0.0f);
  v3set(m3r[2], sr, 0.0f, cr);

  v3set(m3p[0], 1.0f, 0.0f, 0.0f);
  v3set(m3p[1], 0.0f, cp, -sp);
  v3set(m3p[2], 0.0f, sp, cp);

  v3set(m3y[0], cy, -sy, 0.0f);
  v3set(m3y[1], sy, cy, 0.0f);
  v3set(m3y[2], 0.0f, 0.0f, 1.0f);

  m3transf(KM3(m3p), KM3(m3r), m3pr);
  m3transf(KM3(m3y), KM3(m3pr), m3);
}

void m4copy(mat4_t m4dst, const mat4_t m4src) {
  memcpy(m4dst, m4src, sizeof(mat4_t));
}

void m4ident(mat4_t m4) {
  m4[0][0] = m4[1][1] = m4[2][2] = m4[3][3] = 1.0f;
  m4[0][1] = m4[0][2] = m4[0][3] = 0.0f;
  m4[1][0] = m4[1][2] = m4[1][3] = 0.0f;
  m4[2][0] = m4[2][1] = m4[2][3] = 0.0f;
  m4[3][0] = m4[3][1] = m4[3][2] = 0.0f;
}

void m4transp(mat4_t m4) {
  SWAP(m4[0][1], m4[1][0]);
  SWAP(m4[0][2], m4[2][0]);
  SWAP(m4[0][3], m4[3][0]);

  SWAP(m4[1][2], m4[2][1]);
  SWAP(m4[1][3], m4[3][1]);

  SWAP(m4[2][3], m4[3][2]);
}

void m4transf(const mat4_t m4a, const mat4_t m4b, mat4_t m4) {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
      m4[i][j] = m4a[i][0] * m4b[0][j] + m4a[i][1] * m4b[1][j] + m4a[i][2] * m4b[2][j] + m4a[i][3] * m4b[3][j];
    }
}

void m4transv3(const mat4_t m4c, const mat4_t m4b, const mat4_t m4a, mat4_t m4) {
  int i, j;
  mat4_t m4ba;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
      m4ba[i][j] = m4b[i][0] * m4a[0][j] + m4b[i][1] * m4a[1][j] + m4b[i][2] * m4a[2][j] + m4b[i][3] * m4a[3][j];
    }

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
      m4[i][j] = m4c[i][0] * m4ba[0][j] + m4c[i][1] * m4ba[1][j] + m4c[i][2] * m4ba[2][j] + m4c[i][3] * m4ba[3][j];
    }
}

void m4world(mat4_t m4, const vec3_t v3p, const vec3_t v3i, const vec3_t v3j, const vec3_t v3k) {
  m4[0][0] = v3i[0];
  m4[1][0] = v3i[1];
  m4[2][0] = v3i[2];
  m4[3][0] = 0.0f;

  m4[0][1] = v3j[0];
  m4[1][1] = v3j[1];
  m4[2][1] = v3j[2];
  m4[3][1] = 0.0f;

  m4[0][2] = v3k[0];
  m4[1][2] = v3k[1];
  m4[2][2] = v3k[2];
  m4[3][2] = 0.0f;

  m4[0][3] = v3p[0];
  m4[1][3] = v3p[1];
  m4[2][3] = v3p[2];
  m4[3][3] = 1.0f;
}

void m4view(mat4_t m4, const vec3_t v3p, const vec3_t v3i, const vec3_t v3j, const vec3_t v3k) {
  vec3_t v3t;
  v3t[0] = -v3dot(v3i, v3p);
  v3t[1] = -v3dot(v3j, v3p);
  v3t[2] = -v3dot(v3k, v3p);

  m4[0][0] = v3i[0];
  m4[0][1] = v3i[1];
  m4[0][2] = v3i[2];
  m4[0][3] = v3t[0];

  m4[1][0] = v3j[0];
  m4[1][1] = v3j[1];
  m4[1][2] = v3j[2];
  m4[1][3] = v3t[1];

  m4[2][0] = v3k[0];
  m4[2][1] = v3k[1];
  m4[2][2] = v3k[2];
  m4[2][3] = v3t[2];

  m4[3][0] = (float) 0;
  m4[3][1] = (float) 0;
  m4[3][2] = (float) 0;
  m4[3][3] = (float) 1;
}

void m4persp(mat4_t m4, float aspect, float rads, float min, float max) {
// given point <x,y,z,w>, and 4 by 4 matrix mat
//    | ...,   0,   0,   0 |   | x |   | x' |
//    |   0,   0, ...,   0 | * | y | = | y' |
//    |   0,   a,   0,   b |   | z |   | z' |
//    |   0,   1,   0,   0 |   | 1 |   | w' |
//
// x' = x / tan( fov/2 )
// y' = z / tan( fov/2 * aspect )
// z' = a * y + b
// w' = y
//
// NOTES:
// z-buffer := z'/w, or z'/y. for min/max distances, we want:
//    i) ( a * min + b ) / min = -1
//   ii) ( a * max + b ) / max = +1
// solve for a & b (opengl z-buffer ranges from -1 to +1)
//
// screen coordinates are signed normalized, so the
// perspective divide maps x & y values to the -1/1 range, meaning:
//  -1 <= x'/w <= 1 &
//  -1 <= y'/w <= 1
// working in one dimension:
// let x := sin( theta ), z:= cos( theta ), because look is adj to view angle, right is opp
// x * D / z = 1, when theta is half of the fov,
// solving for D, we get D = z/x, or cos( fov/2 ) / sin( fov/2 ), or cotan( fov/2 )

// solution:
//
// 1. a * min + b = min * n,
// 2. a * max + b = max * f,
// subtract 2. from 1 and solve for a
// substitute value of a in 2. & solve for b

  const float z_n = -1.0f;
  const float z_f = +1.0f;
  int i, j;

  float a = (z_f * max - z_n * min) / (max - min);
  float b = z_f * max - a * max;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      m4[i][j] = 0.0f;

  m4[0][0] = 1.0f / tanf(rads * 0.5f);
  m4[1][2] = aspect * m4[0][0];  //1.0f / tanf( (float)h/(float)w * fov * 0.5f * M_PI/180.0f ); //
  m4[2][1] = a;
  m4[2][3] = b;
  m4[3][1] = 1.0f;
}

void m4ortho(mat4_t m4, int w, int h, float min, float max) {
// given point <x,y,z,w>, and 4 by 4 matrix mat
//    | ...,   0,   0,   0 |   | x |   | x' |
//    |   0,   0, ...,   0 | * | y | = | y' |
//    |   0,   a,   0,   b |   | z |   | z' |
//    |   0,   1,   0,   0 |   | 1 |   | w' |
//
// x' = x / w
// y' = z / h
// z' = a * y + b
// w' = 1

  memset(m4, 0, sizeof(mat4_t));

  float a = 1.0f / max;
  float b = -min / max;

  m4[0][0] = 2.0f / (float) w;
  m4[1][2] = 2.0f / (float) h;
  m4[2][1] = a;
  m4[2][3] = b;
  m4[3][3] = 1.0f;
}

void m4mul(mat4_t m4, mat4_t m4a, mat4_t m4b) {
  int i, j, k;
  for (i = 0; i < 4; i++)
    for (k = 0; k < 4; k++)
      for (j = 0; j < 4; j++)
        m4[i][k] += m4a[i][j] * m4b[j][k];
}

float m4det(mat4_t m4) {
  int i, r, c, r0, c0;

  float det = 0.0f;
  for (i = 0; i < 4; i++) {
    mat3_t m3;
    float s = i & 0x01 ? -m4[0][i] : +m4[0][i];

    for (r = 1; r < 4; r++) {
      r0 = r - 1;
      c0 = 0;
      for (c = 0; c < 4; c++)
        if (c != i) {
          m3[r0][c0] = m4[r][c];
          c0++;
        }
    }
    det += s * m3det(m3);
  }

  return det;
}

//VECTOR 4
void v4set(vec4_t v4, float x, float y, float z, float w) {
  v4[0] = x;
  v4[1] = y;
  v4[2] = z;
  v4[3] = w;
}

void v4copy(vec4_t v4dst, const vec4_t v4src) {
  v4dst[0] = v4src[0];
  v4dst[1] = v4src[1];
  v4dst[2] = v4src[2];
  v4dst[3] = v4src[3];
}

void v4add(vec4_t v4to, const vec4_t v4val) {
  v4to[0] += v4val[0];
  v4to[1] += v4val[1];
  v4to[2] += v4val[2];
  v4to[3] += v4val[3];
}

void v4muls(vec4_t v4, float s) {
  v4[0] *= s;
  v4[1] *= s;
  v4[2] *= s;
  v4[3] *= s;
}

float v4dot(const vec4_t v4u, const vec4_t v4v) {
  return (v4u[0] * v4v[0] + v4u[1] * v4v[1] + v4u[2] * v4v[2] + v4u[3] * v4v[3]);
}

void v4transf(mat4_t m4, vec4_t v4) {
  vec4_t v4t;
  v4copy(v4t, v4);
  v4[0] = v4dot(m4[0], v4t);
  v4[1] = v4dot(m4[1], v4t);
  v4[2] = v4dot(m4[2], v4t);
  v4[3] = v4dot(m4[3], v4t);
}

void v3wtransf(const mat4_t m4, vec3_t v3, float w) {
  vec4_t v4;
  v4set(v4, v3[0], v3[1], v3[2], w);
  v3[0] = v4dot(m4[0], v4);
  v3[1] = v4dot(m4[1], v4);
  v3[2] = v4dot(m4[2], v4);
}

void m4ypr(mat4_t m4, const vec3_t v3p, float y, float p, float r, int in_degs) {
  if (in_degs) {
    y = y * M_PI / 180.0f;
    p = p * M_PI / 180.0f;
    r = r * M_PI / 180.0f;
  }

  float cp = cosf(p);
  float sp = sinf(p);

  float cy = cosf(y);
  float sy = sinf(y);

  float cr = cosf(r);
  float sr = sinf(r);

  mat4_t m4r, m4p, m4y, m4pr;

  v4set(m4r[0], cr, 0.0f, -sr, 0.0f);
  v4set(m4r[1], 0.0f, 1.0f, 0.0f, 0.0f);
  v4set(m4r[2], sr, 0.0f, cr, 0.0f);
  v4set(m4r[3], 0.0f, 0.0f, 0.0f, 1.0f);

  v4set(m4p[0], 1.0f, 0.0f, 0.0f, 0.0f);
  v4set(m4p[1], 0.0f, cp, -sp, 0.0f);
  v4set(m4p[2], 0.0f, sp, cp, 0.0f);
  v4set(m4p[3], 0.0f, 0.0f, 0.0f, 1.0f);

  v4set(m4y[0], cy, -sy, 0.0f, 0.0f);
  v4set(m4y[1], sy, cy, 0.0f, 0.0f);
  v4set(m4y[2], 0.0f, 0.0f, 1.0f, 0.0f);
  v4set(m4y[3], 0.0f, 0.0f, 0.0f, 1.0f);

  m4transf(KM4(m4p), KM4(m4r), m4pr);
  m4transf(KM4(m4y), KM4(m4pr), m4);

  m4[0][3] = v3p[0];
  m4[1][3] = v3p[1];
  m4[2][3] = v3p[2];
}

