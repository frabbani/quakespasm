#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "neural.h"
#include "reinforce.h"

/*
 static RL_type_t rl_type = RL_sarsa;
 static RL_bool rl_inited = RL_false;
 static NN_neural_network_t *rl_nn = RL_nullptr;
 static double *rl_qs[2] = { RL_nullptr, RL_nullptr };
 static int rl_qcount = 0;
 static RL_act_cb rl_act = RL_nullptr;
 static RL_set_input_cb rl_set = RL_nullptr;
 static RL_reward_cb rl_reward = RL_nullptr;
 static void *rl_param = RL_nullptr;

 static double rl_alpha = 0.0;
 static double rl_epsilon = 0.0;
 static double rl_gamma = 0.0;
 static RL_action_t rl_action = { 0, RL_false };
 */

typedef struct RL_ctx_s {
  RL_type_t type;
  NN_neural_network_t *nn;
  double *qs[2];
  int qcount;
  RL_act_cb act;
  RL_set_input_cb set;
  RL_reward_cb reward;
  double alpha;
  double epsilon;
  double gamma;
  RL_action_t action;
  RL_agent_state_t agent;
  RL_bool inited;
} RL_ctx_t;

#define CURR_QS  0
#define NEXT_QS  1

RL_agent_t RL_init(RL_type_t type, double alpha, double epsilon, double gamma, const NN_info_t *nn_info, RL_set_input_cb set, RL_reward_cb reward, RL_act_cb act, void *agent_info) {
  RL_ctx_t *ctx = malloc(sizeof(RL_ctx_t));

  ctx->nn = malloc(sizeof(NN_neural_network_t));
  NN_info_t info;
  memcpy(&info, nn_info, sizeof(NN_info_t));
  info.input_size += 2;  // action
  NN_init_neural_network(ctx->nn, &info);

  ctx->type = type;
  ctx->alpha = alpha;
  ctx->epsilon = epsilon;
  ctx->gamma = gamma;

  ctx->qcount = ctx->nn->output_size;
  ctx->qs[0] = malloc(sizeof(double) * ctx->qcount);
  ctx->qs[1] = malloc(sizeof(double) * ctx->qcount);

  ctx->act = act;
  ctx->set = set;
  ctx->reward = reward;
  ctx->agent = agent_info;

  ctx->action = (RL_action_t ) { 0, RL_true };
  ctx->nn->input[0] = (double) ctx->action.exploratory;
  ctx->nn->input[1] = (double) ctx->action.taken;
  //ctx->set(ctx->agent, &ctx->nn->input[2]);
  ctx->inited = RL_true;
  return ctx;
}

static int q_max(RL_ctx_t *ctx, int type) {
  double *qs = type == CURR_QS ? ctx->qs[CURR_QS] : ctx->qs[NEXT_QS];
  int best = 0;
  double q = qs[0];
  for (int i = 1; i < ctx->qcount; i++) {
    if (qs[i] > q) {
      best = i;
      q = qs[i];
    }
  }
  return best;
}

static RL_action_t e_greedy(RL_ctx_t *ctx) {
  RL_action_t action = { 0, RL_false };
  if (NN_random(1.0, 0.0) < ctx->epsilon) {
    action.exploratory = RL_true;
    action.taken = (int) NN_random((double) ctx->qcount, 0.0);
  } else {
    action.taken = q_max(ctx, CURR_QS);
    action.exploratory = RL_false;
  }
  return action;

}

double RL_step(RL_agent_t agent) {
  RL_ctx_t *ctx = agent;
  NN_neural_network_t *nn = ctx->nn;
  if (!ctx->inited)
    return 0.0;

  ctx->nn->input[0] = (double) ctx->action.exploratory;
  ctx->nn->input[1] = (double) ctx->action.taken;
  ctx->set(ctx->agent, &nn->input[2]);

  NN_forward_propagate(nn);
  for (int i = 0; i < ctx->qcount; i++) {
    ctx->qs[CURR_QS][i] = nn->output_layer.neurons[i].value;
  }

  ctx->action = e_greedy(ctx);

  ctx->act(ctx->agent, ctx->action.taken);
  double reward = ctx->reward(ctx->agent);

  nn->input[0] = (double) ctx->action.exploratory;
  nn->input[1] = (double) ctx->action.taken;
  ctx->set(ctx->agent, &nn->input[2]);

  NN_forward_propagate(nn);
  double mse = 0.0f;
  for (int j = 0; j < nn->output_size; j++) {
    double delta = nn->prediction[j] - nn->target[j];
    mse += delta * delta;
  }
  mse /= (double) nn->output_size;

  for (int i = 0; i < ctx->qcount; i++) {
    ctx->qs[NEXT_QS][i] = nn->output_layer.neurons[i].value;
  }

  double target = 0.0;
  for (int i = 0; i < ctx->qcount; i++)
    nn->target[i] = ctx->qs[CURR_QS][i];

  if (RL_sarsa == ctx->type) {
    target = reward + ctx->gamma * ctx->qs[NEXT_QS][ctx->action.taken];  // SARSA
  }
  if (RL_qlearn == ctx->type) {
    int best = q_max(ctx, NEXT_QS);
    target = reward + ctx->gamma * ctx->qs[NEXT_QS][best];  // Q-Learning
  }
  nn->target[ctx->action.taken] += ctx->alpha * (target - ctx->qs[CURR_QS][ctx->action.taken]);
  NN_backward_propagate(nn);
  return mse;
}

void RL_term(RL_agent_t *agent_ptr) {
  RL_ctx_t *ctx = *agent_ptr;
  if (ctx->nn)
    free(ctx->nn);

  if (ctx->qs[0])
    free(ctx->qs[0]);
  if (ctx->qs[1])
    free(ctx->qs[1]);

  free(*agent_ptr);
  *agent_ptr = RL_nullptr;
}

void RL_export_neural_network(RL_agent_t agent, const char *filename) {
  RL_ctx_t *ctx = agent;
  NN_export_neural_network(ctx->nn, filename);
}
