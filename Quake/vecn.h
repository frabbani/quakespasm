#pragma once

typedef struct {
  union {
    struct {
      float x, y;
    };
    float xy[2];
  };
} vec2;

typedef struct {
  union {
    struct {
      float x, y, z;
    };
    float xyz[3];
  };
} vec3;

typedef struct {
  union {
    struct {
      float x, y, z, w;
    };
    float xyzw[4];
  };
} vec4;

typedef struct mat {
  union {
    struct {
      float e00, e01;
      float e10, e11;
    };
    float es[2 * 2];
    float es22[2][2];
  };
} mat2;

typedef struct {
  union {
    struct {
      float e00, e01, e02;
      float e10, e11, e12;
      float e20, e21, e22;
    };
    float es[3 * 3];
    float es33[3][3];
  };
} mat3;

typedef struct {
  union {
    struct {
      float e00, e01, e02, e03;
      float e10, e11, e12, e13;
      float e20, e21, e22, e23;
      float e30, e31, e32, e33;
    };
    float es[4 * 4];
    float es44[4][4];
  };
} mat4;

extern vec3 v3zero();
extern vec3 v3x();
extern vec3 v3y();
extern vec3 v3z();

extern vec2 v2set(float x, float y);
extern vec2 v2sub(vec2 lhs, vec2 rhs);
extern vec2 v2add(vec2 lhs, vec2 rhs);
extern vec2 v2scale(vec2 v, float s);
extern float v2dot(vec2 lhs, vec2 rhs);
extern float v2mag(vec2 v);
extern vec2 v2norm(vec2 v);
extern vec2 v2transf(mat2 M, vec2 v);

extern vec3 v3set(float x, float y, float z);
extern vec3 v3add(vec3 lhs, vec3 rhs);
extern vec3 v3sub(vec3 lhs, vec3 rhs);
extern vec3 v3scale(vec3 v, float s);
extern float v3dot(vec3 lhs, vec3 rhs);
extern float v3mag(vec3 v);
extern vec3 v3norm(vec3 v);
extern vec3 v3cross(vec3 lhs, vec3 rhs);
extern vec3 v3rot(vec3 p, vec3 axis, float radians);

#define v3point( a, b ) v3sub( b, a )

extern vec4 v4set(float x, float y, float z, float w);
extern vec4 v4scale(vec4 v, float s);

extern mat2 m2inv(mat2 M);

extern mat4 m4ident();
extern mat4 m4mul(mat4 lhs, mat4 rhs);

extern mat4 m4persp(float aspect, float fov_radians, float min, float max);
extern mat4 m4ortho(uint32_t w, uint32_t h, float min, float max);

extern mat4 m4view(vec3 p, vec3 r, vec3 l, vec3 u);
extern mat4 m4world(vec3 p, vec3 r, vec3 l, vec3 u);
