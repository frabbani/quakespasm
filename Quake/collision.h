#pragma once

#include "mathlib.h"

typedef struct colltri_s {
  qboolean valid;
  mygl_vec3 ps[3];
  mygl_vec3 o, u, v;
  mygl_mat2 A;
  Plane plane;
} Colltri;

Colltri make_colltri(mygl_vec3 p1, mygl_vec3 p2, mygl_vec3 p3);

void dump_colltri_to_obj(const Colltri *tris, int num_tris, const char *name);

void dump_colltri_ray_to_obj(const Colltri *tris, int num_tris, Ray ray, const char *name);
//p = tri.o + mu * tri.u + nu * tri.v
qboolean colltri_ray_isect(const Colltri *tri, const Ray *ray, mygl_vec3 *p, float *len, float *mu, float *nu);

