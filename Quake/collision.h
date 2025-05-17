#pragma once

#include "mathlib.h"

typedef struct colltri_s {
  qboolean valid;
  vec3 ps[3];
  vec3 o, u, v;
  mat2 A;
  Plane plane;
} colltri_t;

colltri_t make_colltri(vec3 p1, vec3 p2, vec3 p3);

void dump_colltri_to_obj(const colltri_t *tris, int num_tris, const char *name);

void dump_colltri_ray_to_obj(const colltri_t *tris, int num_tris, Ray ray, const char *name);
//p = tri.o + mu * tri.u + nu * tri.v
qboolean colltri_ray_isect(const colltri_t *tri, const Ray *ray, vec3 *p, float *len, float *mu, float *nu);

