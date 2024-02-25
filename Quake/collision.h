#pragma once

#include "mathlib.h"

typedef struct CollTri_s {
  qboolean valid;
  Vec3 ps[3];
  Vec3 o, u, v;
  Mat2 A;
  Plane plane;
} CollTri;

CollTri makeCollTri(Vec3 p1, Vec3 p2, Vec3 p3);

void dumpCollTrisToObj(const CollTri *tris, int num_tris, const char *name);

void dumpCollTrisAndRayToObj(const CollTri *tris, int num_tris, Ray ray, const char *name);
//p = tri.o + mu * tri.u + nu * tri.v
qboolean collTriRayIsect(const CollTri *tri, const Ray *ray, Vec3 *p, float *len, float *mu, float *nu);

