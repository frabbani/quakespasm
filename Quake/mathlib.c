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
// mathlib.c -- math primitives
#include "quakedef.h"

vec3_t vec3_origin = { 0, 0, 0 };

/*-----------------------------------------------------------------*/

void ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal) {
  float d;
  vec3_t n;
  float inv_denom;

  inv_denom = 1.0F / DotProduct(normal, normal);

  d = DotProduct( normal, p ) * inv_denom;

  n[0] = normal[0] * inv_denom;
  n[1] = normal[1] * inv_denom;
  n[2] = normal[2] * inv_denom;

  dst[0] = p[0] - d * n[0];
  dst[1] = p[1] - d * n[1];
  dst[2] = p[2] - d * n[2];
}

/*
 ** assumes "src" is normalized
 */
void PerpendicularVector(vec3_t dst, const vec3_t src) {
  int pos;
  int i;
  float minelem = 1.0F;
  vec3_t tempvec;

  /*
   ** find the smallest magnitude axially aligned vector
   */
  for (pos = 0, i = 0; i < 3; i++) {
    if (fabs(src[i]) < minelem) {
      pos = i;
      minelem = fabs(src[i]);
    }
  }
  tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
  tempvec[pos] = 1.0F;

  /*
   ** project the point onto the plane defined by src
   */
  ProjectPointOnPlane(dst, tempvec, src);

  /*
   ** normalize the result
   */
  VectorNormalize(dst);
}

//johnfitz -- removed RotatePointAroundVector() becuase it's no longer used and my compiler fucked it up anyway

/*-----------------------------------------------------------------*/

float anglemod(float a) {
#if 0
	if (a >= 0)
		a -= 360*(int)(a/360);
	else
		a += 360*( 1 + (int)(-a/360) );
#endif
  a = (360.0 / 65536) * ((int) (a * (65536 / 360.0)) & 65535);
  return a;
}

/*
 ==================
 BoxOnPlaneSide

 Returns 1, 2, or 1 + 2
 ==================
 */
int BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, mplane_t *p) {
  float dist1, dist2;
  int sides;

#if 0	// this is done by the BOX_ON_PLANE_SIDE macro before calling this
		// function
// fast axial cases
	if (p->type < 3)
	{
		if (p->dist <= emins[p->type])
			return 1;
		if (p->dist >= emaxs[p->type])
			return 2;
		return 3;
	}
#endif

// general case
  switch (p->signbits) {
    case 0:
      dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
      dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
      break;
    case 1:
      dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
      dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
      break;
    case 2:
      dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
      dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
      break;
    case 3:
      dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
      dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
      break;
    case 4:
      dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
      dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
      break;
    case 5:
      dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
      dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
      break;
    case 6:
      dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
      dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
      break;
    case 7:
      dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
      dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
      break;
    default:
      dist1 = dist2 = 0;		// shut up compiler
      Sys_Error("BoxOnPlaneSide:  Bad signbits");
      break;
  }

#if 0
	int		i;
	vec3_t	corners[2];

	for (i=0 ; i<3 ; i++)
	{
		if (plane->normal[i] < 0)
		{
			corners[0][i] = emins[i];
			corners[1][i] = emaxs[i];
		}
		else
		{
			corners[1][i] = emins[i];
			corners[0][i] = emaxs[i];
		}
	}
	dist = DotProduct (plane->normal, corners[0]) - plane->dist;
	dist2 = DotProduct (plane->normal, corners[1]) - plane->dist;
	sides = 0;
	if (dist1 >= 0)
		sides = 1;
	if (dist2 < 0)
		sides |= 2;
#endif

  sides = 0;
  if (dist1 >= p->dist)
    sides = 1;
  if (dist2 < p->dist)
    sides |= 2;

#ifdef PARANOID
	if (sides == 0)
		Sys_Error ("BoxOnPlaneSide: sides==0");
#endif

  return sides;
}

//johnfitz -- the opposite of AngleVectors.  this takes forward and generates pitch yaw roll
//TODO: take right and up vectors to properly set yaw and roll
void VectorAngles(const vec3_t forward, vec3_t angles) {
  vec3_t temp;

  temp[0] = forward[0];
  temp[1] = forward[1];
  temp[2] = 0;
  angles[PITCH] = -atan2(forward[2], VectorLength(temp)) / M_PI_DIV_180;
  angles[YAW] = atan2(forward[1], forward[0]) / M_PI_DIV_180;
  angles[ROLL] = 0;
}

void AngleVectors(vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
  float angle;
  float sr, sp, sy, cr, cp, cy;

  angle = angles[YAW] * (M_PI * 2 / 360);
  sy = sin(angle);
  cy = cos(angle);
  angle = angles[PITCH] * (M_PI * 2 / 360);
  sp = sin(angle);
  cp = cos(angle);
  angle = angles[ROLL] * (M_PI * 2 / 360);
  sr = sin(angle);
  cr = cos(angle);

  forward[0] = cp * cy;
  forward[1] = cp * sy;
  forward[2] = -sp;
  right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
  right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
  right[2] = -1 * sr * cp;
  up[0] = (cr * sp * cy + -sr * -sy);
  up[1] = (cr * sp * sy + -sr * cy);
  up[2] = cr * cp;
}

int VectorCompare(vec3_t v1, vec3_t v2) {
  int i;

  for (i = 0; i < 3; i++)
    if (v1[i] != v2[i])
      return 0;

  return 1;
}

void VectorMA(vec3_t veca, float scale, vec3_t vecb, vec3_t vecc) {
  vecc[0] = veca[0] + scale * vecb[0];
  vecc[1] = veca[1] + scale * vecb[1];
  vecc[2] = veca[2] + scale * vecb[2];
}

vec_t _DotProduct(vec3_t v1, vec3_t v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void _VectorSubtract(vec3_t veca, vec3_t vecb, vec3_t out) {
  out[0] = veca[0] - vecb[0];
  out[1] = veca[1] - vecb[1];
  out[2] = veca[2] - vecb[2];
}

void _VectorAdd(vec3_t veca, vec3_t vecb, vec3_t out) {
  out[0] = veca[0] + vecb[0];
  out[1] = veca[1] + vecb[1];
  out[2] = veca[2] + vecb[2];
}

void _VectorCopy(vec3_t in, vec3_t out) {
  out[0] = in[0];
  out[1] = in[1];
  out[2] = in[2];
}

void CrossProduct(vec3_t v1, vec3_t v2, vec3_t cross) {
  cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
  cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
  cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

vec_t VectorLength(vec3_t v) {
  return sqrt(DotProduct(v, v));
}

float VectorNormalize(vec3_t v) {
  float length, ilength;

  length = sqrt(DotProduct(v, v));

  if (length) {
    ilength = 1 / length;
    v[0] *= ilength;
    v[1] *= ilength;
    v[2] *= ilength;
  }

  return length;
}

void VectorInverse(vec3_t v) {
  v[0] = -v[0];
  v[1] = -v[1];
  v[2] = -v[2];
}

void VectorScale(vec3_t in, vec_t scale, vec3_t out) {
  out[0] = in[0] * scale;
  out[1] = in[1] * scale;
  out[2] = in[2] * scale;
}

int Q_log2(int val) {
  int answer = 0;
  while (val >>= 1)
    answer++;
  return answer;
}

/*
 ================
 R_ConcatRotations
 ================
 */
void R_ConcatRotations(float in1[3][3], float in2[3][3], float out[3][3]) {
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
}

/*
 ================
 R_ConcatTransforms
 ================
 */
void R_ConcatTransforms(float in1[3][4], float in2[3][4], float out[3][4]) {
  out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
  out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
  out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
  out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] + in1[0][2] * in2[2][3] + in1[0][3];
  out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
  out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
  out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
  out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] + in1[1][2] * in2[2][3] + in1[1][3];
  out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
  out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
  out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
  out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] + in1[2][2] * in2[2][3] + in1[2][3];
}

/*
 ===================
 FloorDivMod

 Returns mathematically correct (floor-based) quotient and remainder for
 numer and denom, both of which should contain no fractional part. The
 quotient must fit in 32 bits.
 ====================
 */

void FloorDivMod(double numer, double denom, int *quotient, int *rem) {
  int q, r;
  double x;

#ifndef PARANOID
  if (denom <= 0.0)
    Sys_Error("FloorDivMod: bad denominator %f\n", denom);

//	if ((floor(numer) != numer) || (floor(denom) != denom))
//		Sys_Error ("FloorDivMod: non-integer numer or denom %f %f\n",
//				numer, denom);
#endif

  if (numer >= 0.0) {

    x = floor(numer / denom);
    q = (int) x;
    r = (int) floor(numer - (x * denom));
  } else {
    //
    // perform operations with positive values, and fix mod to make floor-based
    //
    x = floor(-numer / denom);
    q = -(int) x;
    r = (int) floor(-numer - (x * denom));
    if (r != 0) {
      q--;
      r = (int) denom - r;
    }
  }

  *quotient = q;
  *rem = r;
}

/*
 ===================
 GreatestCommonDivisor
 ====================
 */
int GreatestCommonDivisor(int i1, int i2) {
  if (i1 > i2) {
    if (i2 == 0)
      return (i1);
    return GreatestCommonDivisor(i2, i1 % i2);
  } else {
    if (i1 == 0)
      return (i2);
    return GreatestCommonDivisor(i1, i2 % i1);
  }
}

/*
 ===================
 Invert24To16

 Inverts an 8.24 value to a 16.16 value
 ====================
 */

fixed16_t Invert24To16(fixed16_t val) {
  if (val < 256)
    return (0xFFFFFFFF);

  return (fixed16_t) (((double) 0x10000 * (double) 0x1000000 / (double) val) + 0.5);
}

Plane make_plane(mygl_vec3 p, mygl_vec3 n) {
  Plane plane;
  plane.n = mygl_v3norm(n);
  plane.dist = mygl_v3dot(p, plane.n);
  return plane;
}

Ray make_ray(mygl_vec3 p, mygl_vec3 p2) {
  Ray ray;
  ray.ps[0] = p;
  ray.ps[1] = p2;

  ray.d = mygl_v3point(ray.o, ray.e);
  if (mygl_v3dot(ray.d, ray.d) > TOL_SQ) {
    ray.len = mygl_v3mag(ray.d);
    ray.d = mygl_v3scale(ray.d, 1.0f / ray.len);
  } else {
    ray.d = mygl_v3zero();
    ray.len = 0.0f;
  }
  return ray;
}

qboolean ray_isect_plane(const Ray *ray, Plane plane, mygl_vec3 *p, float *dist) {
  const float grazing = 0.01745f;  //~1 degs grazing angle (sin(~1))
  if (ray->len < TOL)
    return false;

  float ndotd, odotn, alpha;

  ndotd = mygl_v3dot(plane.n, ray->d);
  if (fabsf(ndotd) < grazing)  //assume not touching at grazing angle
    return false;

  //        dot( ( v + alpha d ), n  ) = dist;
  // dot( v, n ) + alpha * dot( d, n ) = dist;
  //               alpha * dot( d, n ) = dist - dot( v, n )
  //                             alpha = ( dist - dot( v, n ) ) / dot( d, n )

  odotn = mygl_v3dot(plane.n, ray->o);
  alpha = (plane.dist - odotn) / ndotd;

  if (alpha < 0.0f || alpha > ray->len)
    return false;

  if (p)
    *p = mygl_v3add(ray->o, mygl_v3scale(ray->d, alpha));

  if (dist)
    *dist = alpha;
  return true;
}

Transform make_transform(mygl_vec3 p, mygl_vec3 angles) {
  Transform t = { { { { 0 } } } };
  t.p = p;
  t.angles = angles;

  AngleVectors(t.angles.f3, t.basis.l.f3, t.basis.r.f3, t.basis.u.f3);
  mygl_v3scale(t.basis.r, -1.0f);  //-y is right, +y is left

  t.p_loc.x = mygl_v3dot(t.p, t.basis.lru[0]);
  t.p_loc.y = mygl_v3dot(t.p, t.basis.lru[1]);
  t.p_loc.z = mygl_v3dot(t.p, t.basis.lru[2]);
  return t;
}

mygl_vec3 transform_vec3(const Transform *transform, mygl_vec3 p, TransformSpace space, qboolean direction_only) {
  mygl_vec3 r = mygl_v3zero();
  if (space == LocalSpace) {
    if (!direction_only)
      p = mygl_v3point(transform->p, p);
    r.x = mygl_v3dot(p, transform->basis.l);
    r.y = mygl_v3dot(p, transform->basis.r);
    r.z = mygl_v3dot(p, transform->basis.u);
  } else {
    mygl_vec3 lru[3];
    for (int i = 0; i < 3; i++) {
      lru[i] = mygl_v3scale(transform->basis.lru[i], p.f3[i]);
    }
    r = mygl_v3add(lru[0], mygl_v3add(lru[1], lru[2]));
    if (!direction_only)
      r = mygl_v3add(r, transform->p);
  }
  return r;
}

Plane transform_plane(const Transform *transform, Plane plane, TransformSpace space) {
  mygl_vec3 p = transform_vec3(transform, mygl_v3scale(plane.n, plane.dist), space, false);
  plane.n = transform_vec3(transform, plane.n, space, true);
  plane.dist = mygl_v3dot(p, plane.n);
  return plane;
}

Ray transform_ray(const Transform *transform, const Ray *ray, TransformSpace space) {
  mygl_vec3 o = transform_vec3(transform, ray->o, space, false);
  mygl_vec3 e = transform_vec3(transform, ray->e, space, false);
  return make_ray(o, e);
}

