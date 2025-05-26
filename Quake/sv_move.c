/*
 Copyright (C) 1996-2001 Id Software, Inc.
 Copyright (C) 2002-2009 John Fitzgibbons and others
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
// sv_move.c -- monster movement
#include "quakedef.h"

#include "reinforce.h"

#define	STEPSIZE	18
/*
 =============
 SV_CheckBottom

 Returns false if any part of the bottom of the entity is off an edge that
 is not a staircase.

 =============
 */
int c_yes, c_no;

qboolean SV_CheckBottom(edict_t *ent) {
  vec3_t mins, maxs, start, stop;
  trace_t trace;
  int x, y;
  float mid, bottom;

  VectorAdd(ent->v.origin, ent->v.mins, mins);
  VectorAdd(ent->v.origin, ent->v.maxs, maxs);

// if all of the points under the corners are solid world, don't bother
// with the tougher checks
// the corners must be within 16 of the midpoint
  start[2] = mins[2] - 1;
  for (x = 0; x <= 1; x++)
    for (y = 0; y <= 1; y++) {
      start[0] = x ? maxs[0] : mins[0];
      start[1] = y ? maxs[1] : mins[1];
      if (SV_PointContents(start) != CONTENTS_SOLID)
        goto realcheck;
    }

  c_yes++;
  return true;		// we got out easy

  realcheck: c_no++;
//
// check it for real...
//
  start[2] = mins[2];

// the midpoint must be within 16 of the bottom
  start[0] = stop[0] = (mins[0] + maxs[0]) * 0.5;
  start[1] = stop[1] = (mins[1] + maxs[1]) * 0.5;
  stop[2] = start[2] - 2 * STEPSIZE;
  trace = SV_Move(start, vec3_origin, vec3_origin, stop, true, ent);

  if (trace.fraction == 1.0)
    return false;
  mid = bottom = trace.endpos[2];

// the corners must be within 16 of the midpoint
  for (x = 0; x <= 1; x++)
    for (y = 0; y <= 1; y++) {
      start[0] = stop[0] = x ? maxs[0] : mins[0];
      start[1] = stop[1] = y ? maxs[1] : mins[1];

      trace = SV_Move(start, vec3_origin, vec3_origin, stop, true, ent);

      if (trace.fraction != 1.0 && trace.endpos[2] > bottom)
        bottom = trace.endpos[2];
      if (trace.fraction == 1.0 || mid - trace.endpos[2] > STEPSIZE)
        return false;
    }

  c_yes++;
  return true;
}

/*
 =============
 SV_movestep

 Called by monster program code.
 The move will be adjusted for slopes and stairs, but if the move isn't
 possible, no move is done, false is returned, and
 pr_global_struct->trace_normal is set to the normal of the blocking wall
 =============
 */
qboolean SV_movestep(edict_t *ent, vec3_t move, qboolean relink) {
  float dz;
  vec3_t oldorg, neworg, end;
  trace_t trace;
  int i;
  edict_t *enemy;

// try the move
  VectorCopy(ent->v.origin, oldorg);
  VectorAdd(ent->v.origin, move, neworg);

// flying monsters don't step up
  if ((int) ent->v.flags & (FL_SWIM | FL_FLY)) {
    // try one move with vertical motion, then one without
    for (i = 0; i < 2; i++) {
      VectorAdd(ent->v.origin, move, neworg);
      enemy = PROG_TO_EDICT(ent->v.enemy);
      if (i == 0 && enemy != sv.edicts) {
        dz = ent->v.origin[2] - PROG_TO_EDICT(ent->v.enemy)->v.origin[2];
        if (dz > 40)
          neworg[2] -= 8;
        if (dz < 30)
          neworg[2] += 8;
      }
      trace = SV_Move(ent->v.origin, ent->v.mins, ent->v.maxs, neworg, false, ent);

      if (trace.fraction == 1) {
        if (((int) ent->v.flags & FL_SWIM) && SV_PointContents(trace.endpos) == CONTENTS_EMPTY)
          return false;  // swim monster left water

        VectorCopy(trace.endpos, ent->v.origin);
        if (relink)
          SV_LinkEdict(ent, true);
        return true;
      }

      if (enemy == sv.edicts)
        break;
    }

    return false;
  }

// push down from a step height above the wished position
  neworg[2] += STEPSIZE;
  VectorCopy(neworg, end);
  end[2] -= STEPSIZE * 2;

  trace = SV_Move(neworg, ent->v.mins, ent->v.maxs, end, false, ent);

  if (trace.allsolid)
    return false;

  if (trace.startsolid) {
    neworg[2] -= STEPSIZE;
    trace = SV_Move(neworg, ent->v.mins, ent->v.maxs, end, false, ent);
    if (trace.allsolid || trace.startsolid)
      return false;
  }
  if (trace.fraction == 1) {
    // if monster had the ground pulled out, go ahead and fall
    if ((int) ent->v.flags & FL_PARTIALGROUND) {
      VectorAdd(ent->v.origin, move, ent->v.origin);
      if (relink)
        SV_LinkEdict(ent, true);
      ent->v.flags = (int) ent->v.flags & ~FL_ONGROUND;
      //	Con_Printf ("fall down\n");
      return true;
    }

    return false;		// walked off an edge
  }

// check point traces down for dangling corners
  VectorCopy(trace.endpos, ent->v.origin);

  if (!SV_CheckBottom(ent)) {
    if ((int) ent->v.flags & FL_PARTIALGROUND) {  // entity had floor mostly pulled out from underneath it
                                                  // and is trying to correct
      if (relink)
        SV_LinkEdict(ent, true);
      return true;
    }
    VectorCopy(oldorg, ent->v.origin);
    return false;
  }

  if ((int) ent->v.flags & FL_PARTIALGROUND) {
    //	Con_Printf ("back on ground\n");
    ent->v.flags = (int) ent->v.flags & ~FL_PARTIALGROUND;
  }
  ent->v.groundentity = EDICT_TO_PROG(trace.ent);

// the move is ok
  if (relink)
    SV_LinkEdict(ent, true);
  return true;
}

//============================================================================

/*
 ======================
 SV_StepDirection

 Turns to the movement direction, and walks the current distance if
 facing it.

 ======================
 */
void PF_changeyaw(void);
qboolean SV_StepDirection(edict_t *ent, float yaw, float dist) {
  vec3_t move, oldorigin;
  float delta;

  ent->v.ideal_yaw = yaw;
  PF_changeyaw();

  yaw = yaw * M_PI * 2 / 360;
  move[0] = cos(yaw) * dist;
  move[1] = sin(yaw) * dist;
  move[2] = 0;

  VectorCopy(ent->v.origin, oldorigin);
  if (SV_movestep(ent, move, false)) {
    delta = ent->v.angles[YAW] - ent->v.ideal_yaw;
    if (delta > 45 && delta < 315) {		// not turned far enough, so don't take the step
      VectorCopy(oldorigin, ent->v.origin);
    }
    SV_LinkEdict(ent, true);
    return true;
  }
  SV_LinkEdict(ent, true);

  return false;
}

/*
 ======================
 SV_FixCheckBottom

 ======================
 */
void SV_FixCheckBottom(edict_t *ent) {
//	Con_Printf ("SV_FixCheckBottom\n");
  ent->v.flags = (int) ent->v.flags | FL_PARTIALGROUND;
}

/*
 ================
 SV_NewChaseDir

 ================
 */
#define	DI_NODIR	-1
void SV_NewChaseDir(edict_t *actor, edict_t *enemy, float dist) {
  float deltax, deltay;
  float d[3];
  float tdir, olddir, turnaround;

  olddir = anglemod((int) (actor->v.ideal_yaw / 45) * 45);
  turnaround = anglemod(olddir - 180);

  deltax = enemy->v.origin[0] - actor->v.origin[0];
  deltay = enemy->v.origin[1] - actor->v.origin[1];
  if (deltax > 10)
    d[1] = 0;
  else if (deltax < -10)
    d[1] = 180;
  else
    d[1] = DI_NODIR;
  if (deltay < -10)
    d[2] = 270;
  else if (deltay > 10)
    d[2] = 90;
  else
    d[2] = DI_NODIR;

// try direct route
  if (d[1] != DI_NODIR && d[2] != DI_NODIR) {
    if (d[1] == 0)
      tdir = d[2] == 90 ? 45 : 315;
    else
      tdir = d[2] == 90 ? 135 : 215;

    if (tdir != turnaround && SV_StepDirection(actor, tdir, dist))
      return;
  }

// try other directions
  if (((rand() & 3) & 1) || abs((int) deltay) > abs((int) deltax))  // ericw -- explicit int cast to suppress clang suggestion to use fabsf
      {
    tdir = d[1];
    d[1] = d[2];
    d[2] = tdir;
  }

  if (d[1] != DI_NODIR && d[1] != turnaround && SV_StepDirection(actor, d[1], dist))
    return;

  if (d[2] != DI_NODIR && d[2] != turnaround && SV_StepDirection(actor, d[2], dist))
    return;

  /* there is no direct path to the player, so pick another direction */

  if (olddir != DI_NODIR && SV_StepDirection(actor, olddir, dist))
    return;

  if (rand() & 1) /*randomly determine direction of search*/
  {
    for (tdir = 0; tdir <= 315; tdir += 45)
      if (tdir != turnaround && SV_StepDirection(actor, tdir, dist))
        return;
  } else {
    for (tdir = 315; tdir >= 0; tdir -= 45)
      if (tdir != turnaround && SV_StepDirection(actor, tdir, dist))
        return;
  }

  if (turnaround != DI_NODIR && SV_StepDirection(actor, turnaround, dist))
    return;

  actor->v.ideal_yaw = olddir;		// can't move

// if a bridge was pulled out from underneath a monster, it may not have
// a valid standing position at all

  if (!SV_CheckBottom(actor))
    SV_FixCheckBottom(actor);

}

/*
 ======================
 SV_CloseEnough

 ======================
 */
qboolean SV_CloseEnough(edict_t *ent, edict_t *goal, float dist) {
  int i;

  for (i = 0; i < 3; i++) {
    if (goal->v.absmin[i] > ent->v.absmax[i] + dist)
      return false;
    if (goal->v.absmax[i] < ent->v.absmin[i] - dist)
      return false;
  }
  return true;
}

/*
 ======================
 SV_MoveToGoal

 ======================
 */
void SV_MoveToGoal(void) {
  edict_t *ent, *goal;
  float dist;

  ent = PROG_TO_EDICT(pr_global_struct->self);
  goal = PROG_TO_EDICT(ent->v.goalentity);
  dist = G_FLOAT(OFS_PARM0);

  if (!((int) ent->v.flags & (FL_ONGROUND | FL_FLY | FL_SWIM))) {
    G_FLOAT(OFS_RETURN) = 0;
    return;
  }

// if the next step hits the enemy, return immediately
  if ( PROG_TO_EDICT(ent->v.enemy) != sv.edicts && SV_CloseEnough(ent, goal, dist))
    return;

// bump around...
  if ((rand() & 3) == 1 || !SV_StepDirection(ent, ent->v.ideal_yaw, dist)) {
    SV_NewChaseDir(ent, goal, dist);
  }
}
int num_dirs = 0;
vec3 SV_dirs[36];
float SV_angles[36];

vec3 SV_movedir(int facing, float dist) {
  if (facing < 0 || facing >= num_dirs)
    return v3set(0.0, 0.0, 0.0);
  return v3scale(SV_dirs[facing], dist);
}

/*
 float SV_Proximity(edict_t *ent, edict_t *goal, int facing, float dist, float radius) {
 vec2 p, e, d, dnorm;
 p.x = ent->v.origin[0];
 p.y = ent->v.origin[1];
 e.x = goal->v.origin[0];
 e.y = goal->v.origin[1];
 d = v2sub(e, p);
 dnorm = v2norm(d);
 float reward = 1.0 - v2dot(d, d) / (radius * radius);
 reward = CLAMP(0.0, reward, 1.0);

 double facing_reward = v3dot(SV_dirs[facing], v3set(dnorm.x, dnorm.y, 0.0f));
 facing_reward = CLAMP(0.0, facing_reward, 1.0);

 vec3 p1 = V3(ent->v.origin);
 vec3 p2 = v3add(p1, SV_moved(facing, dist * 2.5));
 trace_t trace = SV_Move(p1.xyz, ent->v.mins, ent->v.maxs, p2.xyz, false, ent);
 double hit_penalty = trace.fraction < 0.0;

 return 0.7 * reward + facing_reward - 1.7 * hit_penalty;
 }
 */

static float piecewise_falloff(float v, float a, float b, float c) {
  if (v < a)
    return 1.0f;
  if (v <= b) {
    return 0.666 * (1.0 - (v - a) / (b - a)) + 0.333;
  }
  if (v <= c) {
    return 0.333 * (1.0 - (v - b) / (c - b));
  }
  return 0.0f;
}

#define OBSTRUCTION_SCALE 8.0f

qboolean SV_Obstructed(edict_t *ent, edict_t *goal, int facing, float dist, trace_t *trace) {
  dist *= OBSTRUCTION_SCALE;
  vec3 p = V3(ent->v.origin);
  vec3 p2 = v3add(p, SV_movedir(facing, dist));
  *trace = SV_Move(p.xyz, ent->v.mins, ent->v.maxs, p2.xyz, false, ent);
  if (goal && trace->ent == goal) {
    return false;
  }
  return !(trace->fraction == 1.0f && !trace->allsolid && !trace->startsolid);
}

float SV_Proximity(edict_t *ent, vec3 e, int facing, float a, float b, float c) {
  // trace_t trace;
  vec3 p = V3(ent->v.origin);
  vec3 d = v3point(p, e);
  d.z = 0.0;
  float dist_sq = v3dot(d, d);
  if (dist_sq < a * a)
    return 1.0;
  float dist = sqrtf(dist_sq);
  double distance_reward = piecewise_falloff(dist, a, b, c);
  vec3 dnorm = v3scale(d, 1.0 / dist);
  float facing_reward = v3dot(dnorm, SV_dirs[facing]);
  facing_reward = CLAMP(0.0f, facing_reward, 1.0f);
  // float obstructed_penalty = SV_Obstructed(ent, NULL, facing, 8.0, &trace);
  return 0.8 * distance_reward + 0.5 * facing_reward;  // - 1.3 * obstructed_penalty;;
}

typedef struct {
  edict_t *ent;
  edict_t *goal;
  int facing;
  vec3 c1;
  vec3 c2;
  float dist;
  double reward;
  RL_agent_t agent;
} AI_state_t;

qboolean AI_inited = false;
AI_state_t ai;

void AI_SetInput(RL_agent_state_t state, double *input) {
  AI_state_t *ai = state;
  trace_t trace;
  vec3 mins = V3(sv.models[1]->mins);
  vec3 maxs = V3(sv.models[1]->maxs);
  vec3 o = v3scale(v3add(mins, maxs), 0.5);
  vec3 s = v3sub(maxs, mins);
  double radius = v3mag(v3sub(maxs, o));
  double o_x = (double) o.x;
  double o_y = (double) o.y;
  double s_x = 1.0 / s.x;
  double s_y = 1.0 / s.y;

  vec3 p = V3(ai->ent->v.origin);
  vec3 d = v3point(p, V3(ai->goal->v.origin));
  vec3 dnorm = v3norm(d);
  vec3 c1 = v3point(p, ai->c1);
  vec3 c2 = v3point(p, ai->c2);

  double x = (double) d.x * (double) s_x;
  double y = (double) d.y * (double) s_y;

  int i = 0;
  input[i++] = (double) dnorm.x;
  input[i++] = (double) dnorm.y;
  input[i++] = sqrt(x * x + y * y);
  input[i++] = (double) c1.x * s_x;
  input[i++] = (double) c1.y * s_y;
  input[i++] = (double) c2.x * s_x;
  input[i++] = (double) c2.y * s_y;
  input[i++] = ai->goal->v.velocity[0] / 400.0;
  input[i++] = ai->goal->v.velocity[1] / 400.0;
  for (int j = 0; j < num_dirs; j++) {
    input[i++] = (double) SV_Obstructed(ai->ent, ai->goal, ai->facing, ai->dist, &trace);
  }
}

double AI_Reward(RL_agent_state_t state) {
  AI_state_t *ai = state;
  trace_t trace;
  double near_reward = SV_Proximity(ai->ent, V3(ai->goal->v.origin), ai->facing, 20, 1000, 3000);
  double hit_penalty = (double) SV_Obstructed(ai->ent, ai->goal, ai->facing, ai->dist, &trace);
  ai->reward = 0.3 * near_reward - 0.7 * hit_penalty;
  return ai->reward;
}

//static float vectoyaw(vec3 v) {
//  if (v.x == 0.0f && v.y == 0.0f)
//    return 0.0f;
//
//  float yaw = (int) (atan2(v.y, v.x) * 180 / M_PI);
//  return anglemod(yaw);
//}

void face_towards(float *actual_yaw, float yaw_speed, float ideal_yaw) {

  *actual_yaw = anglemod(*actual_yaw);

  float yaw = *actual_yaw;

  if (yaw == ideal_yaw)
    return;
  float change = ideal_yaw - yaw;

  if (ideal_yaw > yaw) {
    if (change >= 180)
      change -= 360;
  } else {
    if (change <= -180)
      change += 180;
  }
  if (change > 0) {
    if (change > +yaw_speed)
      change = +yaw_speed;
  } else {
    if (change < -yaw_speed)
      change = -yaw_speed;
  }
  *actual_yaw += anglemod(yaw + change);
}

void AI_Step(RL_agent_state_t state, int facing) {
  AI_state_t *ai = state;
  entvars_t *e = &ai->ent->v;

  if (SV_CloseEnough(ai->ent, ai->goal, ai->dist)) {
    return;
    Sys_Printf("%s: close enough?\n", __FUNCTION__);
  }
  ai->facing = facing;
  float yaw = SV_angles[ai->facing];  //vectoyaw(SV_dirs[ai->facing]);
  e->ideal_yaw = yaw;
//face_towards(&e->angles[YAW], e->yaw_speed, e->ideal_yaw);
  e->angles[YAW] = e->ideal_yaw;

  SV_movestep(ai->ent, SV_movedir(facing, ai->dist).xyz, true);

  MyGL_Color lidar[21 * 21];
  for (int y = 0; y < 21; y++) {
    for (int x = 0; x < 21; x++) {
      lidar[y * 21 + x].r = 0;
      lidar[y * 21 + x].g = 0;
      lidar[y * 21 + x].b = 255;
      lidar[y * 21 + x].a = 255;
    }
  }
  int i = 10 * 21 + 10;
  lidar[i].r = lidar[i].g = lidar[i].b = 240;

  for (int i = 0; i < num_dirs; i++) {
    vec3 p = V3(e->origin);
    vec3 p2 = v3add(p, SV_movedir(i, ai->dist * 8));
    trace_t trace;
    SV_Obstructed(ai->ent, ai->goal, i, ai->dist, &trace);
    vec2 d = v2scale(v2set(SV_dirs[i].x, SV_dirs[i].y), OBSTRUCTION_SCALE * trace.fraction);
    d.x = 10 + CLAMP(-10.0f, d.x, +10.0f);
    d.y = 10 + CLAMP(-10.0f, d.y, +10.0f);
    int j = (int) d.y * 21 + (int) d.x;
    lidar[j].r = 255;
    lidar[j].b = 0;
  }
  Image_WriteBMP("lidar.bmp", (void*) lidar, 21, 21, 32, false);
}

void SV_InitAI() {
  num_dirs = 0;
  for (int i = 0; i < 360; i += 20) {
    float angle = (float) i * M_PI / 180.0f;
    vec3 v;
    v.x = cosf(angle);
    v.y = sinf(angle);
    v.z = 0.0f;
    SV_angles[num_dirs] = i;
    SV_dirs[num_dirs++] = v;
  }

  int input_size = 0;
  input_size += 3;  // target vector plus length to target
  input_size += 2;  // ewma 1
  input_size += 2;  // ewma 2
  input_size += 2;  // target velocity
  for (int i = 0; i < num_dirs; i++)
    input_size++;  //can reach direction

  NN_info_t info;
  info.activation = NN_tanh;
  info.learning_rate = 0.1;
  info.l2_decay = 0.0002;
  info.hidden_layers_size = 2;
  info.neurons_per[0] = 60;
  info.neurons_per[1] = 30;
  info.input_size = input_size;
  info.output_size = num_dirs;
  Sys_Printf("%s: input size: %d\n", __FUNCTION__, info.input_size);
  Sys_Printf("%s: output size: %d\n", __FUNCTION__, info.output_size);

  if (AI_inited) {
    RL_term(&ai.agent);
  }
  ai.agent = RL_init(RL_sarsa, 0.4, 0.03, 0.7, &info, AI_SetInput, AI_Reward, AI_Step, &ai);

}

void SV_MoveToGoal2(void) {
  edict_t *ent, *goal;
  float dist;

  ent = PROG_TO_EDICT(pr_global_struct->self);

  goal = G_EDICT(OFS_PARM0);
  dist = G_FLOAT(OFS_PARM1);
  float *u1 = G_VECTOR(OFS_PARM2);
  float *u2 = G_VECTOR(OFS_PARM3);

  if (!((int) ent->v.flags & FL_ONGROUND)) {
    G_FLOAT(OFS_RETURN) = 0;
    return;
  }

// if the next step hits the enemy, return immediately
  if (goal != sv.edicts && SV_CloseEnough(ent, goal, dist)) {
    return;
  }

  static int c = 0;
  c = (c + 1) % 10;
  if (!c) {
//    float distance = v3mag(v3sub(V3(ent->v.origin), V3(goal->v.origin)));
//    Sys_Printf("*****\n");
//    Sys_Printf("dist..: %f\n", distance);
//    Sys_Printf("enemy....: %s\n", PR_GetString(goal->v.classname));
//    Sys_Printf("self loc.: %f %f %f\n", ent->v.origin[0], ent->v.origin[1], ent->v.origin[2]);
//    Sys_Printf("enemy loc: %f %f %f\n", goal->v.origin[0], goal->v.origin[1], goal->v.origin[2]);
//    Sys_Printf("*****\n");
  }

  ai.ent = ent;
  ai.goal = goal;
  ai.c1 = V3(u1);
  ai.c2 = V3(u2);
  ai.dist = dist;
  RL_step(ai.agent);
  RL_export_neural_network(ai.agent, "nn.txt");
}
