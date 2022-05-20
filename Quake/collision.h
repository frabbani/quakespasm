#pragma once

#include "q_stdinc.h"
#include "mathlib.h"

typedef struct coll_tri_s{
	qboolean valid;
	vec3     ps[3];
	vec3     o, u, v;
	mat2x2   A;
	fplane_t plane;

}coll_tri_t;


coll_tri_t coll_tri_make( const vec3 p0, const vec3 p1, const vec3 p2 );

void coll_tris_dump_obj( const coll_tri_t *tris, int32 num_tris, const char *name );

//p = tri.o + mu * tri.u + nu * tri.v
qboolean coll_tri_ray_isect( const coll_tri_t *tri, ray_t ray, vec3_t p, float *len, float *mu, float *nu );


