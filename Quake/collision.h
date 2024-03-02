#pragma once

#include "mathlib.h"

typedef struct colltri_s {
  qboolean valid;
  Vec3 ps[3];
  Vec3 o, u, v;
  Mat2 A;
  Plane plane;
} Colltri;

Colltri make_colltri(Vec3 p1, Vec3 p2, Vec3 p3);

void dump_colltri_to_obj(const Colltri *tris, int num_tris, const char *name);

void dump_colltri_ray_to_obj(const Colltri *tris, int num_tris, Ray ray, const char *name);
//p = tri.o + mu * tri.u + nu * tri.v
qboolean colltri_ray_isect(const Colltri *tri, const Ray *ray, Vec3 *p, float *len, float *mu, float *nu);

