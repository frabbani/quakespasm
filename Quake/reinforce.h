#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "neural.h"

/** Reinforcement Learning using SARSA **/

#ifndef RL_nullptr
#define RL_nullptr ((void *)0)
#endif

#ifndef RL_bool
#define RL_bool char
#define RL_true 1
#define RL_false 0
#endif

typedef enum RL_type_e {
	RL_qlearn, RL_sarsa,
} RL_type_t;

typedef struct RL_action_s {
	int taken;
	RL_bool exploratory;
} RL_action_t;

typedef void *RL_agent_t;
typedef void *RL_agent_state_t;

typedef void (*RL_set_input_cb)(RL_agent_state_t, double*);
typedef void (*RL_act_cb)(RL_agent_state_t, int);
typedef double (*RL_reward_cb)(RL_agent_state_t);

RL_agent_t RL_init(RL_type_t type /* RL type SARSA or Q-LEARN*/,
		double alpha /*Bellman learning rate (0 to 1)*/,
		double epsilon /*greedy exploration rate*/,
		double gamma /*discount factor (0 to 1)*/, const NN_info_t *nn_info,
		RL_set_input_cb set, RL_reward_cb reward, RL_act_cb act,
		RL_agent_state_t state);

void RL_term(RL_agent_t *agent_ptr);
void RL_step(RL_agent_t agent); // executes one RL update step using the specified algorithm (SARSA or Q-Learning)
void RL_export_neural_network(RL_agent_t agent, const char *filename);

#ifdef __cplusplus
}
#endif
