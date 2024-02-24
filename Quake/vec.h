#ifndef MYVEC_H_
#define MYVEC_H_

#include "q_stdinc.h"
#include "math.h"

/*
 //VECTOR 2
 static inline void v2zero( vec2 v2 )
 {
 v2[0] = v2[1] = 0.0f;
 }

 static inline void v2set( vec2 v2, float x, float y )
 {
 v2[0] = x;
 v2[1] = y;
 }

 static inline int v2eqt( const vec2 v2u, const vec2 v2v, float tol )
 {
 return
 fabsf( v2u[0] - v2v[0] ) < tol &&
 fabsf( v2u[1] - v2v[1] ) < tol;
 }

 static inline void v2copy( vec2 v2dst, const vec2 v2src )
 {
 v2dst[0] = v2src[0];
 v2dst[1] = v2src[1];
 }

 static inline void v2add( vec2 v2to, const vec2 v2val )
 {
 v2to[0] += v2val[0];
 v2to[1] += v2val[1];
 }

 static inline void v2sub( vec2 v2from, const vec2 v2val )
 {
 v2from[0] -= v2val[0];
 v2from[1] -= v2val[1];
 }

 static inline void v2muls( vec2 v2, float s )
 {
 v2[0] *= s;
 v2[1] *= s;
 }

 static inline void v2mul( vec2 v2, const vec2 v2val )
 {
 v2[0] *= v2val[0];
 v2[1] *= v2val[1];
 }

 static inline void v2madd( vec2 v2to, float s, const vec2 v2val )
 {
 v2to[0] += s * v2val[0];
 v2to[1] += s * v2val[1];
 }

 static inline void v2make( vec2 v2, const vec2 v2from, const vec2 v2to )
 {
 v2[0] = v2to[0] - v2from[0];
 v2[1] = v2to[1] - v2from[1];
 }

 static inline float v2mag( const vec2 v2 )
 {
 return( sqrtf( v2[0]*v2[0] + v2[1]*v2[1] ) );
 }

 static inline float v2norm( vec2 v2 )
 {
 float m = v2mag(v2);
 v2muls( v2, 1.0f/m );
 return m;
 }

 static inline void v2lerp( vec2 v2, const vec2 v2u, const vec2 v2v, float mu )
 {
 float nu = 1.0 - mu;
 v2[0] = nu * v2u[0] + mu * v2v[0];
 v2[1] = nu * v2u[1] + mu * v2v[1];
 }

 static inline float v2dot( const vec2 v2u, const vec2 v2v )
 {
 return( v2u[0]*v2v[0] + v2u[1]*v2v[1] );
 }

 static inline void v2perp( vec2 v2, const vec2 v2u )
 {
 v2[0] = -v2u[1];
 v2[1] =  v2u[0];
 }

 //VECTOR 3
 static inline int v3eqt( const vec3 v3u, const vec3 v3v, float tol )
 {
 return
 fabsf( v3u[0] - v3v[0] ) < tol &&
 fabsf( v3u[1] - v3v[1] ) < tol &&
 fabsf( v3u[2] - v3v[2] ) < tol;
 }

 static inline void v3zero( vec3 v3 )
 {
 v3[0] = v3[1] = v3[2] = (float)0;
 }

 static inline void v3set( vec3 v3, float x, float y, float z )
 {
 v3[0] = x;
 v3[1] = y;
 v3[2] = z;
 }

 static inline void v3copy( vec3 v3dst, const vec3 v3src )
 {
 v3dst[0] = v3src[0];
 v3dst[1] = v3src[1];
 v3dst[2] = v3src[2];
 }

 static inline void v3add( vec3 v3to, const vec3 v3val )
 {
 v3to[0] += v3val[0];
 v3to[1] += v3val[1];
 v3to[2] += v3val[2];
 }

 static inline void v3sub( vec3 v3from, const vec3 v3val )
 {
 v3from[0] -= v3val[0];
 v3from[1] -= v3val[1];
 v3from[2] -= v3val[2];
 }

 static inline void v3mul( vec3 v3to, const vec3 v3val )
 {
 v3to[0] *= v3val[0];
 v3to[1] *= v3val[1];
 v3to[2] *= v3val[2];
 }

 static inline void v3div( vec3 v3to, const vec3 v3val )
 {
 v3to[0] /= v3val[0];
 v3to[1] /= v3val[1];
 v3to[2] /= v3val[2];
 }

 static inline void v3muls( vec3 v3, float s )
 {
 v3[0] *= s;
 v3[1] *= s;
 v3[2] *= s;
 }

 static inline void v3madd( vec3 v3, float s, const vec3 v3u )
 {
 v3[0] += s * v3u[0];
 v3[1] += s * v3u[1];
 v3[2] += s * v3u[2];
 }

 static inline void v3cadd( vec3 v3, const vec3 v3val1, const vec3 v3val2 )
 {
 v3[0] =  v3val1[0] + v3val2[0];
 v3[1] =  v3val1[1] + v3val2[1];
 v3[2] =  v3val1[2] + v3val2[2];
 }

 static inline void v3make( vec3 v3, const vec3 v3from, const vec3 v3to )
 {
 v3[0] = v3to[0] - v3from[0];
 v3[1] = v3to[1] - v3from[1];
 v3[2] = v3to[2] - v3from[2];
 }

 static inline float v3mag( const vec3 v3 )
 {
 return( sqrtf( v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2] ) );
 }

 static inline float v3norm( vec3 v3 )
 {
 float m = v3mag(v3);
 v3muls( v3, 1.0f/m );
 return m;
 }

 static inline void v3lerp( vec3 v3, const vec3 v3u, const vec3 v3v, float mu )
 {
 float nu = 1.0 - mu;
 v3[0] = nu * v3u[0] + mu * v3v[0];
 v3[1] = nu * v3u[1] + mu * v3v[1];
 v3[2] = nu * v3u[2] + mu * v3v[2];
 }

 static inline float v3dot( const vec3 v3u, const vec3 v3v )
 {
 return( v3u[0]*v3v[0] + v3u[1]*v3v[1] + v3u[2]*v3v[2] );
 }

 / *
 static inline unsigned int v3eqt( vec3 v3u, vec3 v3v )
 {
 vec3 v3t;
 v3make( v3t, v3u, v3v );
 return( v3dot(v3t,v3t) < THRESHOLD*THRESHOLD );
 }
 * /

 static inline void v3cross( const vec3 v3u, const vec3 v3v, vec3 v3n )
 {
 v3n[0] = (v3u[1]*v3v[2] - v3u[2]*v3v[1]);
 v3n[1] =-(v3u[0]*v3v[2] - v3u[2]*v3v[0]);
 v3n[2] = (v3u[0]*v3v[1] - v3u[1]*v3v[0]);
 }

 static inline void v3transf( const mat3x3 m3x3, vec3 v3 )
 {
 vec3 v3t;
 v3copy( v3t, v3 );
 v3[0] = v3dot( m3x3[0], v3t );
 v3[1] = v3dot( m3x3[1], v3t );
 v3[2] = v3dot( m3x3[2], v3t );
 }

 / *
 static inline void v3sat( vec3 v3 )
 {
 float s = v3[0];
 s = v3[1] > s ? v3[1] : s;
 s = v3[2] > s ? v3[2] : s;
 if( s > 0.0f )
 v3div( v3, s );
 }
 * /

 //MATRIX 2X2
 static inline float m2x2det( const mat2x2 m2x2 ){
 return m2x2[0][0] * m2x2[1][1] - m2x2[1][0] * m2x2[0][1];
 }

 static inline void m2x2inv( mat2x2 m2x2, const mat2x2 m2x2src ){
 float s = m2x2det( m2x2src );
 if( 0.0f == s ){
 m2x2[0][0] = m2x2[0][1] = m2x2[1][0] = m2x2[1][1] = 0.0f;
 return;
 }
 s = 1.0f/s;
 m2x2[0][0] =  s * m2x2src[1][1];
 m2x2[0][1] = -s * m2x2src[0][1];
 m2x2[1][0] = -s * m2x2src[1][0];
 m2x2[1][1] =  s * m2x2src[0][0];
 }

 //MATRIX 3X3
 static inline void m3x3copy( mat3x3 m3x3dst, const mat3x3 m3x3src )
 {
 memcpy( m3x3dst, m3x3src, sizeof(mat3x3) );
 }

 static inline void m3x3ident( mat3x3 m3x3 )
 {
 m3x3[0][0] = m3x3[1][1] = m3x3[2][2] = (float)1;
 m3x3[0][1] = m3x3[0][2] = (float)0;
 m3x3[1][0] = m3x3[1][2] = (float)0;
 m3x3[2][0] = m3x3[2][1] = (float)0;
 }

 static inline void m3x3skewsymcross( mat3x3 m3x3, const vec3 u )
 {
 m3x3[0][0] = m3x3[1][1] = m3x3[2][2] = (float)0;
 m3x3[0][1] = -u[2];  m3x3[1][0] =  u[2];
 m3x3[0][2] =  u[1];  m3x3[2][0] = -u[1];
 m3x3[1][2] = -u[0];  m3x3[2][1] =  u[0];
 }

 static inline void m3x3transp( mat3x3 m3x3 )
 {
 SWAP( m3x3[0][1], m3x3[1][0] );
 SWAP( m3x3[0][2], m3x3[2][0] );
 SWAP( m3x3[1][2], m3x3[2][1] );
 }

 static inline void m3x3transf( const mat3x3 m3x3a, const mat3x3 m3x3b, mat3x3 m3x3 )
 {
 / *
 | a b c |   | l m n |      |   al + bo + cr   am + bp + cs   an + bq + ct   |
 | d e f | * | o p q |  =   |   dl + eo + fr   dm + ep + fs   an + bq + ct   |
 | g h i |   | r s t |      |   gl + ho + ir   gm + hp + is   an + bq + ct   |
 * /

 mat3x3 m3x3t;

 m3x3copy( m3x3t, m3x3b );
 m3x3transp( m3x3t );

 m3x3[0][0] = v3dot( m3x3a[0], m3x3t[0] );
 m3x3[0][1] = v3dot( m3x3a[0], m3x3t[1] );
 m3x3[0][2] = v3dot( m3x3a[0], m3x3t[2] );

 m3x3[1][0] = v3dot( m3x3a[1], m3x3t[0] );
 m3x3[1][1] = v3dot( m3x3a[1], m3x3t[1] );
 m3x3[1][2] = v3dot( m3x3a[1], m3x3t[2] );

 m3x3[2][0] = v3dot( m3x3a[2], m3x3t[0] );
 m3x3[2][1] = v3dot( m3x3a[2], m3x3t[1] );
 m3x3[2][2] = v3dot( m3x3a[2], m3x3t[2] );
 }

 static inline float m3x3det( mat3x3 m3x3 )
 {
 //
 return
 m3x3[0][0] * ( m3x3[1][1] * m3x3[2][2] - m3x3[1][2] * m3x3[2][1] ) -
 m3x3[0][1] * ( m3x3[1][0] * m3x3[2][2] - m3x3[1][2] * m3x3[2][0] ) +
 m3x3[0][2] * ( m3x3[1][0] * m3x3[2][1] - m3x3[1][1] * m3x3[2][0] );
 }

 static inline void m3x3ypr( mat3x3 m3x3, float y, float p, float r, int in_degs ){
 if( in_degs ){
 y = y * M_PI / 180.0f;
 p = p * M_PI / 180.0f;
 r = r * M_PI / 180.0f;
 }

 float  cp = cosf( p );
 float  sp = sinf( p );

 float  cy = cosf( y );
 float  sy = sinf( y );

 float  cr = cosf( r );
 float  sr = sinf( r );

 mat3x3 m3x3r, m3x3p, m3x3y, m3x3pr;

 v3set( m3x3r[0],   cr, 0.0f,  -sr );
 v3set( m3x3r[1], 0.0f, 1.0f, 0.0f );
 v3set( m3x3r[2],   sr, 0.0f,   cr );

 v3set( m3x3p[0], 1.0f, 0.0f, 0.0f );
 v3set( m3x3p[1], 0.0f,   cp,  -sp );
 v3set( m3x3p[2], 0.0f,   sp,   cp );

 v3set( m3x3y[0],   cy,  -sy, 0.0f );
 v3set( m3x3y[1],   sy,   cy, 0.0f );
 v3set( m3x3y[2], 0.0f, 0.0f, 1.0f );

 m3x3transf( CONSTM3X3(m3x3p), CONSTM3X3(m3x3r),  m3x3pr );
 m3x3transf( CONSTM3X3(m3x3y), CONSTM3X3(m3x3pr), m3x3   );
 }

 //MATRIX 4X4
 static inline void m4x4copy( mat4x4 m4x4dst, const mat4x4 m4x4src )
 {
 memcpy( m4x4dst, m4x4src, sizeof(mat4x4) );
 }

 static inline void m4x4ident( mat4x4 m4x4 )
 {
 m4x4[0][0] = m4x4[1][1] = m4x4[2][2] = m4x4[3][3] = 1.0f;
 m4x4[0][1] = m4x4[0][2] = m4x4[0][3] = 0.0f;
 m4x4[1][0] = m4x4[1][2] = m4x4[1][3] = 0.0f;
 m4x4[2][0] = m4x4[2][1] = m4x4[2][3] = 0.0f;
 m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = 0.0f;
 }

 static inline void m4x4scale( mat4x4 m4x4, float x, float y, float z )
 {
 m4x4[0][0] = x;
 m4x4[1][1] = y;
 m4x4[2][2] = z;
 m4x4[3][3] = 1.0f;
 m4x4[0][1] = m4x4[0][2] = m4x4[0][3] = 0.0f;
 m4x4[1][0] = m4x4[1][2] = m4x4[1][3] = 0.0f;
 m4x4[2][0] = m4x4[2][1] = m4x4[2][3] = 0.0f;
 m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = 0.0f;
 }


 static inline void m4x4world(
 mat4x4 m4x4,
 const vec3 v3p, const vec3 v3i, const vec3 v3j, const vec3 v3k )
 {
 m4x4[0][0] = v3i[0];
 m4x4[1][0] = v3i[1];
 m4x4[2][0] = v3i[2];
 m4x4[3][0] = 0.0f;

 m4x4[0][1] = -v3j[0];
 m4x4[1][1] = -v3j[1];
 m4x4[2][1] = -v3j[2];
 m4x4[3][1] =  0.0f;

 m4x4[0][2] = v3k[0];
 m4x4[1][2] = v3k[1];
 m4x4[2][2] = v3k[2];
 m4x4[3][2] = 0.0f;

 m4x4[0][3] = v3p[0];
 m4x4[1][3] = v3p[1];
 m4x4[2][3] = v3p[2];
 m4x4[3][3] = 1.0f;
 }

 static inline void m4x4transp( mat4x4 m4x4 )
 {
 SWAP( m4x4[0][1], m4x4[1][0] );
 SWAP( m4x4[0][2], m4x4[2][0] );
 SWAP( m4x4[0][3], m4x4[3][0] );

 SWAP( m4x4[1][2], m4x4[2][1] );
 SWAP( m4x4[1][3], m4x4[3][1] );

 SWAP( m4x4[2][3], m4x4[3][2] );
 }

 static inline void m4x4transf( const mat4x4 m4x4a, const mat4x4 m4x4b, mat4x4 m4x4 )
 {
 int i, j;
 for( i = 0; i < 4; i++ )
 for( j = 0; j < 4; j++ )
 {
 m4x4[i][j] =
 m4x4a[i][0] * m4x4b[0][j] +
 m4x4a[i][1] * m4x4b[1][j] +
 m4x4a[i][2] * m4x4b[2][j] +
 m4x4a[i][3] * m4x4b[3][j];
 }

 }

 static inline void m4x4transv3( const mat4x4 m4x4c, const mat4x4 m4x4b, const mat4x4 m4x4a, mat4x4 m4x4 )
 {
 int      i, j;
 mat4x4 m4x4ba;

 for( i = 0; i < 4; i++ )
 for( j = 0; j < 4; j++ )
 {
 m4x4ba[i][j] =
 m4x4b[i][0] * m4x4a[0][j] +
 m4x4b[i][1] * m4x4a[1][j] +
 m4x4b[i][2] * m4x4a[2][j] +
 m4x4b[i][3] * m4x4a[3][j];
 }

 for( i = 0; i < 4; i++ )
 for( j = 0; j < 4; j++ )
 {
 m4x4[i][j] =
 m4x4c[i][0] * m4x4ba[0][j] +
 m4x4c[i][1] * m4x4ba[1][j] +
 m4x4c[i][2] * m4x4ba[2][j] +
 m4x4c[i][3] * m4x4ba[3][j];
 }
 }


 static inline void m4x4transl(
 mat4x4 m4x4, const vec3 v3p ){

 const vec3 v3axes[3] = {
 {1.0f,0.0f,0.0f},
 {0.0f,1.0f,0.0f},
 {0.0f,0.0f,1.0f}
 };
 m4x4world( m4x4, v3p, v3axes[0], v3axes[1], v3axes[2] );
 }

 static inline void m4x4view(
 mat4x4 m4x4,
 const vec3 v3p, const vec3 v3f, const vec3 v3r, const vec3 v3u )
 {
 vec3 v3t;
 v3t[0] = -v3dot( v3f, v3p );
 v3t[1] = -v3dot( v3r, v3p );
 v3t[2] = -v3dot( v3u, v3p );

 m4x4[0][0] = v3f[0];
 m4x4[0][1] = v3f[1];
 m4x4[0][2] = v3f[2];
 m4x4[0][3] = v3t[0];

 //in Quake, -y is right, so flip the sign (???)
 m4x4[1][0] = -v3r[0];
 m4x4[1][1] = -v3r[1];
 m4x4[1][2] = -v3r[2];
 m4x4[1][3] = -v3t[1];

 m4x4[2][0] = v3u[0];
 m4x4[2][1] = v3u[1];
 m4x4[2][2] = v3u[2];
 m4x4[2][3] = v3t[2];

 m4x4[3][0] = (float)0;
 m4x4[3][1] = (float)0;
 m4x4[3][2] = (float)0;
 m4x4[3][3] = (float)1;
 }

 static inline void m4x4persp( mat4x4 m4x4, float aspect, float rads, float n, float f )
 {
 // given point <x,y,z,w>, and 4 by 4 matrix mat
 //    | ...,   0,   0,   0 |   | x |   | x' |
 //    |   0, ...,   0,   0 | * | y | = | y' |
 //    |   0,   0,   a,   b |   | z |   | z' |
 //    |   0,   0,  -1,   0 |   | 1 |   | w' |
 //
 // x'  = x / tan( fov/2 )
 // y'  = y / tan( fov/2 ) * aspect
 // z'  = a * z + b
 // w' = -z
 //
 // NOTES:
 // z-buffer solution:
 //    i) ( a * -n + b ) = -1
 //   ii) ( a * -f + b ) = +1
 // solve for a & b (opengl z-buffer ranges from -1 to +1)
 //
 // screen coordinates are signed normalized, so the
 // perspective divide maps x & y values to the -1/1 range, meaning:
 //  -1 <= x'/w <= 1 &
 //  -1 <= y'/w <= 1
 // working in one dimension:
 // let x := sin( theta ), z:= cos( theta ), because look is adj to view angle, right is opp
 // x * D / z = 1, when theta is half of the fov,
 // solving for D, we get D = z/x, or cos( fov/2 ) / sin( fov/2 ), or cotan( fov/2 )

 //( A * -n + B ) = -1.0f
 //( A * -f + B ) = +1.0f

 //| -n, 1 | * |A| = |-1|
 //| -f, 1 |   |B|   |+1|

 mat2x2 M, Minv;
 M[0][0] = -n;
 M[0][1] = 1.0f;
 M[1][0] = -f;
 M[1][1] = 1.0f;
 m2x2inv( Minv, CONSTM2X2(M) );

 float a = Minv[0][0]*(-1.0f) + Minv[0][1] * (+1.0f);
 float b = Minv[1][0]*(-1.0f) + Minv[1][1] * (+1.0f);
 float s = 1.0f / tanf( rads * 0.5f );

 int i, j;
 for( i = 0; i < 4; i++ )
 for( j = 0; j < 4; j++ )
 m4x4[i][j] = 0.0f;

 //m4x4[0][0] = 1.0f / tanf( rads * 0.5f );
 //m4x4[1][1] = aspect * m4x4[0][0]; //1.0f / tanf( (float)h/(float)w * fov * 0.5f * PI/180.0f ); //

 m4x4[0][0] = s;
 m4x4[1][1] = s * aspect;
 m4x4[2][2] = a;
 m4x4[2][3] = b;
 m4x4[3][2] = -1.0f;
 }

 static inline void m4x4ortho( mat4x4 m4x4, int w, int h, float min, float max )
 {
 // given point <x,y,z,w>, and 4 by 4 matrix mat
 //    | ...,   0,   0,   0 |   | x |   | x' |
 //    |   0,   0, ...,   0 | * | y | = | y' |
 //    |   0,   a,   0,   b |   | z |   | z' |
 //    |   0,   0,   0,   1 |   | 1 |   | w' |
 //
 // x' = x / w
 // y' = z / h
 // z' = a * y + b
 // w' = 1


 memset( m4x4, 0, sizeof(mat4x4) );

 float a = 1.0f / max;
 float b = -min / max;

 m4x4[0][0] = 2.0f / (float)w;
 m4x4[1][1] = 2.0f / (float)h;
 m4x4[2][2] = a;
 m4x4[2][3] = b;
 m4x4[3][3] = 1.0f;
 }

 static inline void m4x4mul( mat4x4 m4x4, mat4x4 m4x4a, mat4x4 m4x4b )
 {
 int i, j, k;
 for( i = 0; i < 4; i++ )
 for( k = 0; k < 4; k++ )
 for( j = 0; j < 4; j++ )
 m4x4[i][k] += m4x4a[i][j] * m4x4b[j][k];
 }

 static inline float m4x4det( mat4x4 m4x4 )
 {
 int i, r, c, r0, c0;


 float det = 0.0f;
 for( i = 0; i < 4; i++ )
 {
 mat3x3 m3x3;
 float s = i & 0x01 ? -m4x4[0][i] : +m4x4[0][i];

 for( r = 1; r < 4; r++ )
 {
 r0 = r - 1;
 c0 = 0;
 for( c = 0; c < 4; c++ )
 if( c != i )
 {
 m3x3[r0][c0] = m4x4[r][c];
 c0++;
 }
 }
 det += s * m3x3det( m3x3 );
 }

 return det;
 }



 //VECTOR 4
 static inline void v4set( vec4 v4, float x, float y, float z, float w )
 {
 v4[0] = x;
 v4[1] = y;
 v4[2] = z;
 v4[3] = w;
 }

 static inline void v4copy( vec4 v4dst, const vec4 v4src )
 {
 v4dst[0] = v4src[0];
 v4dst[1] = v4src[1];
 v4dst[2] = v4src[2];
 v4dst[3] = v4src[3];
 }

 static inline void v4add( vec4 v4to, const vec4 v4val )
 {
 v4to[0] += v4val[0];
 v4to[1] += v4val[1];
 v4to[2] += v4val[2];
 v4to[3] += v4val[3];
 }

 static inline void v4muls( vec4 v4, float s )
 {
 v4[0] *= s;
 v4[1] *= s;
 v4[2] *= s;
 v4[3] *= s;
 }

 static inline float v4dot( const vec4 v4u, const vec4 v4v )
 {
 return( v4u[0]*v4v[0] + v4u[1]*v4v[1] + v4u[2]*v4v[2] + v4u[3]*v4v[3] );
 }

 static inline void v4transf( mat4x4 m4x4, vec4 v4 )
 {
 vec4 v4t;
 v4copy( v4t, v4 );
 v4[0] = v4dot( m4x4[0], v4t );
 v4[1] = v4dot( m4x4[1], v4t );
 v4[2] = v4dot( m4x4[2], v4t );
 v4[3] = v4dot( m4x4[3], v4t );
 }


 static inline void v3wtransf( const mat4x4 m4x4, vec3 v3, float w )
 {
 vec4 v4;
 v4set( v4, v3[0], v3[1], v3[2], w );
 v3[0] = v4dot( m4x4[0], v4 );
 v3[1] = v4dot( m4x4[1], v4 );
 v3[2] = v4dot( m4x4[2], v4 );
 }


 static inline void m4x4ypr( mat4x4 m4x4, const vec3 v3p, float y, float p, float r, int in_degs ){
 if( in_degs ){
 y = y * M_PI / 180.0f;
 p = p * M_PI / 180.0f;
 r = r * M_PI / 180.0f;
 }


 float  cp = cosf( p );
 float  sp = sinf( p );

 float  cy = cosf( y );
 float  sy = sinf( y );

 float  cr = cosf( r );
 float  sr = sinf( r );

 mat4x4 m4x4r, m4x4p, m4x4y, m4x4pr;

 v4set( m4x4r[0],   cr, 0.0f,  -sr, 0.0f );
 v4set( m4x4r[1], 0.0f, 1.0f, 0.0f, 0.0f );
 v4set( m4x4r[2],   sr, 0.0f,   cr, 0.0f );
 v4set( m4x4r[3], 0.0f, 0.0f, 0.0f, 1.0f );

 v4set( m4x4p[0], 1.0f, 0.0f, 0.0f, 0.0f );
 v4set( m4x4p[1], 0.0f,   cp,  -sp, 0.0f );
 v4set( m4x4p[2], 0.0f,   sp,   cp, 0.0f );
 v4set( m4x4p[3], 0.0f, 0.0f, 0.0f, 1.0f );

 v4set( m4x4y[0],   cy,  -sy, 0.0f, 0.0f );
 v4set( m4x4y[1],   sy,   cy, 0.0f, 0.0f );
 v4set( m4x4y[2], 0.0f, 0.0f, 1.0f, 0.0f );
 v4set( m4x4y[3], 0.0f, 0.0f, 0.0f, 1.0f );

 m4x4transf( CONSTM4X4(m4x4p), CONSTM4X4(m4x4r),  m4x4pr );
 m4x4transf( CONSTM4X4(m4x4y), CONSTM4X4(m4x4pr), m4x4   );

 m4x4[0][3] = v3p[0];
 m4x4[1][3] = v3p[1];
 m4x4[2][3] = v3p[2];
 }
 */

#endif
