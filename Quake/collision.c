#include "collision.h"
#include "vec.h"

coll_tri_t coll_tri_make( const vec3 p0, const vec3 p1, const vec3 p2 ){
	const float skinny = 0.9998f;	// 1 degree skinny triangle cos(1)

	coll_tri_t tri = {0};
	vec3 u, v, w;

	tri.valid = false;
	v3make( u, p0, p1 );
	v3make( v, p0, p2 );
	v3make( w, p1, p2 );
	if( v3dot( u, u ) < TOL_SQ || v3dot( v, v ) < TOL_SQ || v3dot( w, w ) < TOL_SQ ){
		return tri;
	}

	v3norm( u );
	v3norm( v );
	v3norm( w );
	if( fabsf(v3dot( u, v )) > skinny ||
		fabsf(v3dot( v, w )) > skinny ||
		fabsf(v3dot( w, u )) > skinny )
		return tri;

	v3copy( tri.o, p0 );
	v3make( tri.u, p0, p1 );
	v3make( tri.v, p0, p2 );

	mat2x2 M;
	M[0][0] = v3dot( tri.u, tri.u );
	M[0][1] =
	M[1][0] = v3dot( tri.u, tri.v );
	M[1][1] = v3dot( tri.v, tri.v );

	m2x2inv( tri.A, M );
	tri.valid = true;

	v3cross( tri.u, tri.v, tri.plane.n );
	v3norm (tri.plane.n );
	tri.plane.dist = v3dot( tri.o, tri.plane.n );
	return tri;
}


qboolean coll_tri_ray_isect( const coll_tri_t *tri, ray_t ray, vec3_t p, float *len, float *mu, float *nu ){
	vec3 v;
	vec2 b;
	float  mu_, nu_;
	if( !tri->valid )
		return false;

	if( !ray_plane_isect( ray, tri->plane.n, tri->plane.dist, p ) )
			return false;

	if( *len ){
		v3make( v, ray.o, p );
		*len = v3dot( v, ray.d );
	}


	v3make( v, tri->o, p );
	b[0] = v3dot( v, tri->u );
	b[1] = v3dot( v, tri->v );

	mu_ = v2dot( tri->A[0], b );
	nu_ = v2dot( tri->A[1], b );

	if( *mu )
		*mu = mu_;
	if( *nu )
		*nu = nu_;

	if( mu_ < 0.0f || nu_ < 0.0f || ( mu_ + nu_ ) > 1.0f )
		return false;

	return true;
}

