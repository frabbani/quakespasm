#pragma once


typedef struct coll_tri_s{
	qboolean valid;
	vec3     o, u, v;
	mat2x2   A;
	struct{
		vec3   n;
		float  dist;
	}plane;
	struct{
		vec3 r, f, u;	//right, forward and up axes
	}basis;

}coll_tri_t;


void coll_tri_make( coll_tri_t *tri, const vec3 p0, const vec3 p1, const vec3 p2 );
qboolean coll_tri_ray_hit( const hittri_t *tri, const ray_t *ray, vec3_t v, float *len, float *mu_val, float *nu_val );

typedef struct coll_header_s{
	int32 num_poses, num_tris_per_pose;
	coll_tri_t coll_tris[];
} hitheader_t;

