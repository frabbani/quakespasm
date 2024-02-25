#include "collision.h"

CollTri makeCollTri(Vec3 p1, Vec3 p2, Vec3 p3) {
  const float skinny = 0.9998f;  // 1 degree skinny triangle cos(1)

  CollTri tri = { 0 };
  Vec3 u, v, w;

  tri.valid = false;

  tri.ps[0] = p1;
  tri.ps[1] = p2;
  tri.ps[2] = p3;
  u = vec3Make(p1, p2);
  v = vec3Make(p1, p3);
  w = vec3Make(p2, p3);
  if (vec3Dot(u, u) < TOL_SQ || vec3Dot(v, v) < TOL_SQ || vec3Dot(w, w) < TOL_SQ) {
    return tri;
  }
  u = vec3Norm(u);
  v = vec3Norm(v);
  w = vec3Norm(w);
  if (fabsf(vec3Dot(u, v)) > skinny || fabsf(vec3Dot(v, w)) > skinny || fabsf(vec3Dot(w, u)) > skinny)
    return tri;

  tri.o = p1;
  tri.u = vec3Make(p1, p2);
  tri.v = vec3Make(p1, p3);

  Mat2 M;
  M.e00 = vec3Dot(tri.u, tri.u);
  M.e01 = M.e10 = vec3Dot(tri.u, tri.v);
  M.e11 = vec3Dot(tri.v, tri.v);

  tri.A = mat2Inv(M);
  tri.valid = true;

  tri.plane.n = vec3Norm(vec3Cross(tri.u, tri.v));
  tri.plane.dist = vec3Dot(tri.o, tri.plane.n);
  return tri;
}

qboolean collTriRayIsect(const CollTri *tri, const Ray *ray, Vec3 *p, float *len, float *mu, float *nu) {
  if (!tri->valid || !p)
    return false;

  float dist;
  if (!rayIsectPlane(ray, tri->plane, p, &dist))
    return false;

  if (len) {
    *len = dist;  //between 0 to ray length
  }

  Vec2 b;
  Vec3 v = vec3Make(tri->o, *p);
  b.x = vec3Dot(v, tri->u);
  b.y = vec3Dot(v, tri->v);
  b = vec2Transf(tri->A, b);

  if (mu)
    *mu = b.x;
  if (nu)
    *nu = b.y;

  if (b.x < 0.0f || b.y < 0.0f || (b.x + b.y) > 1.0f)
    return false;

  return true;
}

void dumpCollTrisToObj(const CollTri *tris, int num_tris, const char *name) {

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

void dumpCollTrisAndRayToObj(const CollTri *tris, int num_tris, Ray ray, const char *name) {

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
