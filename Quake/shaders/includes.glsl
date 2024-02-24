#ifdef TRANSFORM

struct MyGL
{
  mat4 matProj;
  mat4 matView;
  mat4 matWorld;
  mat4 matProjViewWorld;
};

uniform MyGL mygl;

#define P    mygl.matProj
#define V    mygl.matView
#define W    mygl.matWorld
#define PVW  mygl.matProjViewWorld

#endif

#ifdef __vert__
#define vary out
#endif


#ifdef __frag__
#define vary in
#endif
