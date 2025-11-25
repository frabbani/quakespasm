#pragma once

typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];

typedef float mat2_t[2][2];
typedef float mat3_t[3][3];
typedef float mat4_t[4][4];

#define KM3(m) ( (const vec3_t*)(const void*)m )
#define KM4(m) ( (const vec4_t*)(const void*)m )

void v2zero(vec2_t v2);
void v2set(vec2_t v2, float x, float y);
int v2eq(const vec2_t v2u, const vec2_t v2v);
void v2copy(vec2_t v2dst, const vec2_t v2src);
void v2add(vec2_t v2to, const vec2_t v2v);
void v2sub(vec2_t v2from, const vec2_t v2v);
void v2muls(vec2_t v2, float s);
void v2madd(vec2_t v2to, float s, const vec2_t v2v);
void v2make(vec2_t v2, const vec2_t v2from, const vec2_t v2to);
float v2mag(const vec2_t v2);
float v2norm(vec2_t v2);
void v2lerp(vec2_t v2, const vec2_t v2u, const vec2_t v2v, float mu);
float v2dot(const vec2_t v2u, const vec2_t v2v);
void v2perp(vec2_t v2, const vec2_t v2u);

int v3eq(const vec3_t v3u, const vec3_t v3v);
void v3zero(vec3_t v3);
void v3set(vec3_t v3, float x, float y, float z);
void v3copy(vec3_t v3dst, const vec3_t v3src);
void v3add(vec3_t v3to, const vec3_t v3val);
void v3sub(vec3_t v3from, const vec3_t v3val);
void v3mul(vec3_t v3to, const vec3_t v3val);
void v3div(vec3_t v3to, const vec3_t v3val);
void v3muls(vec3_t v3, float s);
void v3madd(vec3_t v3, float s, const vec3_t v3u);
void v3cadd(vec3_t v3, const vec3_t v3val1, const vec3_t v3val2);
void v3make(vec3_t v3, const vec3_t v3from, const vec3_t v3to);
float v3mag(const vec3_t v3);
float v3norm(vec3_t v3);
void v3lerp(vec3_t v3, const vec3_t v3u, const vec3_t v3v, float mu);
float v3dot(const vec3_t v3u, const vec3_t v3v);
void v3cross(const vec3_t v3u, const vec3_t v3v, vec3_t v3n);
void v3transf(const mat3_t v3x3, vec3_t v3);

void m3copy(mat3_t m3dst, const mat3_t m3src);
void m3ident(mat3_t m3);
void m3skewsymcross(mat3_t m3, const vec3_t u);
void m3transp(mat3_t m3);
void m3transf(const mat3_t m3a, const mat3_t m3b, mat3_t m3);
float m3det(mat3_t m3);
void m3ypr(mat3_t m3, float y, float p, float r, int in_degs);

void m4copy(mat4_t m4dst, const mat4_t m4src);
void m4ident(mat4_t m4);
void m4transp(mat4_t m4);
void m4transf(const mat4_t m4a, const mat4_t m4b, mat4_t m4);
void m4transv3(const mat4_t m4c, const mat4_t m4b, const mat4_t m4a, mat4_t m4);
void m4world(mat4_t m4, const vec3_t v3p, const vec3_t v3i, const vec3_t v3j, const vec3_t v3k);
void m4view(mat4_t m4, const vec3_t v3p, const vec3_t v3i, const vec3_t v3j, const vec3_t v3k);
void m4persp(mat4_t m4, float aspect, float rads, float min, float max);
