#pragma once

#include "q_stdinc.h"
#include "mathlib.h"

typedef struct coll_tri_s{
	qboolean valid;
	vec3     o, u, v;
	mat2x2   A;
	struct{
		vec3   n;
		float  dist;
	}plane;

}coll_tri_t;


coll_tri_t coll_tri_make( const vec3 p0, const vec3 p1, const vec3 p2 );

//p = t->o + mu * t->u + nu * t->v
//
qboolean coll_tri_ray_hit( const coll_tri_t *tri, ray_t ray, vec3_t p, float *len, float *mu, float *nu );

typedef struct coll_header_s{
	int32 num_poses, num_tris_per_pose;
	coll_tri_t coll_tris[];
} hitheader_t;

