/*
 Copyright (C) 1996-2001 Id Software, Inc.
 Copyright (C) 2002-2009 John Fitzgibbons and others
 Copyright (C) 2007-2008 Kristian Duske
 Copyright (C) 2010-2014 QuakeSpasm developers

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */

#ifndef __MATHLIB_H
#define __MATHLIB_H

// mathlib.h

#include <math.h>
#include "q_stdinc.h"
#include "mygl.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_DIV_180	(M_PI / 180.0) //johnfitz
#define DEG2RAD(a)		((a) * M_PI_DIV_180)

struct mplane_s;

extern vec3_t vec3_origin;

#define	nanmask		(255 << 23)	/* 7F800000 */
#if 0	/* macro is violating strict aliasing rules */
#define	IS_NAN(x)	(((*(int *) (char *) &x) & nanmask) == nanmask)
#else
static inline int IS_NAN(float x) {
  union {
    float f;
    int i;
  } num;
  num.f = x;
  return ((num.i & nanmask) == nanmask);
}
#endif

#define Q_rint(x) ((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5)) //johnfitz -- from joequake

#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define DoublePrecisionDotProduct(x,y) ((double)(x)[0]*(y)[0]+(double)(x)[1]*(y)[1]+(double)(x)[2]*(y)[2])
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}

//johnfitz -- courtesy of lordhavoc
// QuakeSpasm: To avoid strict aliasing violations, use a float/int union instead of type punning.
#define VectorNormalizeFast(_v)\
{\
	union { float f; int i; } _y, _number;\
	_number.f = DotProduct((_v), (_v));\
	if (_number.f != 0.0)\
	{\
		_y.i = 0x5f3759df - (_number.i >> 1);\
		_y.f = _y.f * (1.5f - (_number.f * 0.5f * _y.f * _y.f));\
		VectorScale((_v), _y.f, (_v));\
	}\
}

void TurnVector(vec3_t out, const vec3_t forward, const vec3_t side, float angle);  //johnfitz
void VectorAngles(const vec3_t forward, vec3_t angles);  //johnfitz

void VectorMA(vec3_t veca, float scale, vec3_t vecb, vec3_t vecc);

vec_t _DotProduct(vec3_t v1, vec3_t v2);
void _VectorSubtract(vec3_t veca, vec3_t vecb, vec3_t out);
void _VectorAdd(vec3_t veca, vec3_t vecb, vec3_t out);
void _VectorCopy(vec3_t in, vec3_t out);

int VectorCompare(vec3_t v1, vec3_t v2);
vec_t VectorLength(vec3_t v);
void CrossProduct(vec3_t v1, vec3_t v2, vec3_t cross);
float VectorNormalize(vec3_t v);		// returns vector length
void VectorInverse(vec3_t v);
void VectorScale(vec3_t in, vec_t scale, vec3_t out);
int Q_log2(int val);

void R_ConcatRotations(float in1[3][3], float in2[3][3], float out[3][3]);
void R_ConcatTransforms(float in1[3][4], float in2[3][4], float out[3][4]);

void FloorDivMod(double numer, double denom, int *quotient, int *rem);
fixed16_t Invert24To16(fixed16_t val);
int GreatestCommonDivisor(int i1, int i2);

void AngleVectors(vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
int BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, struct mplane_s *plane);
float anglemod(float a);

#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

//orthonormal basis
typedef struct basis_s {
  union {
    struct {
      mygl_vec3 l, r, u;
    };
    mygl_vec3 lru[3];
  };
} Basis;

// FXR
#define mygl_tov3(v) mygl_v3(v[0], v[1], v[2])

#define TOL	(1e-7f)
#define TOL_SQ	(1e-14f)

typedef struct plane_s {
  mygl_vec3 n;
  float dist;
} Plane;

Plane make_plane(mygl_vec3 p, mygl_vec3 n);

typedef struct ray_s {
  union {
    mygl_vec3 ps[2];
    struct {
      mygl_vec3 o, e;  //origin/end
    };
  };
  mygl_vec3 d;     //direction unit vector
  float len;    //v1 + len*d = v2
} Ray;

Ray make_ray(mygl_vec3 p, mygl_vec3 p2);
qboolean ray_isect_plane(const Ray *ray, Plane plane, mygl_vec3 *p, float *dist);

typedef struct transform_s {
  mygl_vec3 p, p_loc;
  mygl_vec3 angles;
  Basis basis;
} Transform;

typedef enum transform_space_e {
  LocalSpace,
  WorldSpace,
} TransformSpace;

Transform make_transform(mygl_vec3 p, mygl_vec3 angles);
mygl_vec3 transform_vec3(const Transform *transform, mygl_vec3 p, TransformSpace space, qboolean direction_only);
Plane transform_plane(const Transform *transform, Plane plane, TransformSpace space);
Ray transform_ray(const Transform *transform, const Ray *ray, TransformSpace space);

#endif	/* __MATHLIB_H */

