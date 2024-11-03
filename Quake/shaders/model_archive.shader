#if 0
Name "Vertex Position and Texture (Animated)"

Passes "Main"

#endif


Main {

/**
Cull Back
Blend SrcAlpha OneMinusSrcAlpha Add
Depth LEqual
**/

#define TRANSFORM
#define VTX_P_T
#include "includes.glsl"

vary vec4 var_t;

#ifdef __vert__

layout(binding = 1) uniform samplerBuffer frame;
layout(binding = 2) uniform samplerBuffer frame2;

uniform float lerpValue = 0.0;

void main(){
  vec3 p = texelFetch( frame, gl_VertexID ).xyz;
  vec3 p2 = texelFetch( frame2, gl_VertexID ).xyz;
  p = mix( p, p2, vec3(lerpValue) );
  gl_Position = PVW * vec4(p, 1.0); //vtx_p
  var_t = vtx_t;
}

#endif

#ifdef __frag__

layout(binding = 0) uniform sampler2D tex;


void main(){
  vec3 c = texture( tex, var_t.xy ).rgb;
  gl_FragData[0] = vec4( c, 1.0 );
}

#endif

}