#if 0
Name "Keyframe Animation"

Passes "Main"

#endif


Main {

/**
Cull Off
Blend SrcAlpha OneMinusSrcAlpha Add
Depth LEqual
**/

#define TRANSFORM
#include "includes.glsl"

vary vec4 var_t;

#ifdef __vert__

layout(location = 0) in vec4 vtx_t;
layout(location = 1) in vec4 vtx_p0;
layout(location = 2) in vec3 vtx_n0;
layout(location = 3) in vec4 vtx_p1;
layout(location = 4) in vec3 vtx_n1;

uniform float lerpValue = 0.0;

void main(){
  vec4 p = mix( vtx_p0, vtx_p1, vec4(lerpValue) );
  vec3 n = mix( vtx_n0, vtx_n1, vec3(lerpValue) );
  gl_Position = PVW * vec4(p.xyz, 1.0);
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