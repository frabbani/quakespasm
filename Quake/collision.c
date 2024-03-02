#include "collision.h"

Colltri make_colltri(Vec3 p1, Vec3 p2, Vec3 p3) {
  const float skinny = 0.9998f;  // 1 degree skinny triangle cos(1)

  Colltri tri = { 0 };
  Vec3 u, v, w;

  tri.valid = false;

  tri.ps[0] = p1;
  tri.ps[1] = p2;
  tri.ps[2] = p3;
  u = v3make(p1, p2);
  v = v3make(p1, p3);
  w = v3make(p2, p3);
  if (v3dot(u, u) < TOL_SQ || v3dot(v, v) < TOL_SQ || v3dot(w, w) < TOL_SQ) {
    return tri;
  }
  u = v3norm(u);
  v = v3norm(v);
  w = v3norm(w);
  if (fabsf(v3dot(u, v)) > skinny || fabsf(v3dot(v, w)) > skinny || fabsf(v3dot(w, u)) > skinny)
    return tri;

  tri.o = p1;
  tri.u = v3make(p1, p2);
  tri.v = v3make(p1, p3);

  Mat2 M;
  M.e00 = v3dot(tri.u, tri.u);
  M.e01 = M.e10 = v3dot(tri.u, tri.v);
  M.e11 = v3dot(tri.v, tri.v);

  tri.A = m2inv(M);
  tri.valid = true;

  tri.plane.n = v3norm(v3cross(tri.u, tri.v));
  tri.plane.dist = v3dot(tri.o, tri.plane.n);
  return tri;
}

qboolean colltri_ray_isect(const Colltri *tri, const Ray *ray, Vec3 *p, float *len, float *mu, float *nu) {
  if (!tri->valid || !ray || !p)
    return false;

  float dist;
  if (!ray_isect_plane(ray, tri->plane, p, &dist))
    return false;

  if (len) {
    *len = dist;  //between 0 to ray length
  }

  Vec2 b;
  Vec3 v = v3make(tri->o, *p);
  b.x = v3dot(v, tri->u);
  b.y = v3dot(v, tri->v);
  b = v2transf(tri->A, b);

  if (mu)
    *mu = b.x;
  if (nu)
    *nu = b.y;

  if (b.x < 0.0f || b.y < 0.0f || (b.x + b.y) > 1.0f)
    return false;

  return true;
}

void dump_colltri_to_obj(const Colltri *tris, int num_tris, const char *name) {

  char fn[64];
  sprintf(fn, "debug/%s.obj", name);
  printf("dumping file '%s'\n", fn);
  FILE *fp = fopen(fn, "w");
  for (int32 i = 0; i < num_tris; i++) {
    fprintf(fp, "v %f %f %f\n", tris[i].ps[0].x, tris[i].ps[0].y, tris[i].ps[0].z);
    fprintf(fp, "v %f %f %f\n", tris[i].ps[1].x, tris[i].ps[1].y, tris[i].ps[1].z);
    fprintf(fp, "v %f %f %f\n", tris[i].ps[2].x, tris[i].ps[2].y, tris[i].ps[2].z);
  }

  for (int32 i = 0; i < num_tris; i++) {
    int a = i * 3 + 0;
    int b = i * 3 + 1;
    int c = i * 3 + 2;
    fprintf(fp, " f %d %d %d\n", a + 1, b + 1, c + 1);
  }

  fclose(fp);

}

void dump_colltri_ray_to_obj(const Colltri *tris, int num_tris, Ray ray, const char *name) {

  char fn[64];
  sprintf(fn, "debug/%s.obj", name);
  printf("dumping file '%s'\n", fn);
  FILE *fp = fopen(fn, "w");

  Vec3 p = ray.o;
  if (fabsf(ray.o.x) > 0.80f)
    p.z += 3.0f;
  else
    p.x += 3.0f;

  fprintf(fp, "v %f %f %f\n", ray.o.x, ray.o.y, ray.o.z);
  fprintf(fp, "v %f %f %f\n", p.x, p.y, p.z);
  fprintf(fp, "v %f %f %f\n", ray.e.x, ray.e.y, ray.e.z);

  for (int32 i = 0; i < num_tris; i++) {
    fprintf(fp, "v %f %f %f\n", tris[i].ps[0].x, tris[i].ps[0].y, tris[i].ps[0].z);
    fprintf(fp, "v %f %f %f\n", tris[i].ps[1].x, tris[i].ps[1].y, tris[i].ps[1].z);
    fprintf(fp, "v %f %f %f\n", tris[i].ps[2].x, tris[i].ps[2].y, tris[i].ps[2].z);
  }

  fprintf(fp, "f %d %d %d\n", 1, 2, 3);
  for (int32 i = 0; i < num_tris; i++) {
    int a = i * 3 + 4;
    int b = i * 3 + 5;
    int c = i * 3 + 6;
    fprintf(fp, " f %d %d %d\n", a, b, c);
  }
  fclose(fp);

}
