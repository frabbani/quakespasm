#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "mtwister.h"
#include "neural.h"

#define CLAMP( v, l, h ){ v = v < (l) ? (l) : v > (h) ? (h) : v; }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static MTRand mt_rand;
int mt_inited = 0;

void NN_seed_random(unsigned long seed) {
  mt_rand = seedRand(seed);
  mt_inited = 1;
}

double NN_random(double scale, double offset) {
  if (!mt_inited) {
    mt_rand = seedRand(123);
    mt_inited = 1;
  }
  return genRand(&mt_rand) * scale + offset;
}

static double sigmoid_act(double x) {
  return 1.0 / (1.0 + exp(-x));
}

static double sigmoid_deriv(double x) {
  return x * (1 - x);
}

static double tanh_act(double x) {
  return tanh(x);
}

static double tanh_deriv(double x) {
  //1 - tanh(x)^2, assume x is x = tanh(y)
  return 1.0 - x * x;
}

static double relu_act(double x) {
  return fmax(0.0, x);
}

static double relu_deriv(double x) {
  return x > 0 ? 1.0 : 0.0;
}

static double leaky_relu_act(double x, double alpha) {
  return x > 0 ? x : alpha * x;
}

static double leaky_relu_deriv(double x, double alpha) {
  return x > 0 ? 1.0 : alpha;
}

static double act_func(double x, NN_activation_type_t type) {
  switch (type) {
    case (NN_sigmoid):
      return sigmoid_act(x);
    case (NN_tanh):
      return tanh_act(x);
    case (NN_relu):
      return relu_act(x);
    case (NN_leakyrelu):
      return leaky_relu_act(x, 0.01);
  }
  return x;  //???
}

static double act_deriv(double x, NN_activation_type_t type) {
  switch (type) {
    case (NN_sigmoid):
      return sigmoid_deriv(x);
    case (NN_tanh):
      return tanh_deriv(x);
    case (NN_relu):
      return relu_deriv(x);
    case (NN_leakyrelu):
      return leaky_relu_deriv(x, 0.01);
  }
  return x;  //???
}

static void init_neuron(NN_neuron_t *neuron, int n) {
  for (int i = 0; i < n; i++)
    neuron->weights[i] = NN_random(2.0, -1.0);
  neuron->bias = 0.0;
}

static void init_neural_layer(NN_neural_layer_t *layer, int size, NN_neural_layer_t *feed, int is_output) {
  layer->type = is_output ? NN_output : NN_hidden;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->feed = feed;
  if (layer->feed) {
    for (int i = 0; i < layer->size; i++) {
      init_neuron(&layer->neurons[i], feed->size);
    }
  }
}

static void init_neural_first_hidden_layer(NN_neural_layer_t *layer, int size, int input_size, const double *input) {
  layer->type = NN_first;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->input = input;
  for (int i = 0; i < layer->size; i++)
    init_neuron(&layer->neurons[i], input_size);
}

static void neural_layer_propagate(NN_neural_layer_t *layer, int input_size, NN_activation_type_t act_type) {
  for (int i = 0; i < layer->size; i++) {
    NN_neuron_t *neuron = &layer->neurons[i];
    neuron->value_pre = neuron->bias;
    if (layer->type == NN_first) {
      for (int j = 0; j < input_size; j++)
        neuron->value_pre += neuron->weights[j] * layer->input[j];
    } else {
      for (int j = 0; j < layer->feed->size; j++)
        neuron->value_pre += neuron->weights[j] * layer->feed->neurons[j].value;
    }
    neuron->value = act_func(neuron->value_pre, act_type);
  }
}

static void neural_layer_propagate_regress(NN_neural_layer_t *layer) {
  for (int i = 0; i < layer->size; i++) {
    NN_neuron_t *neuron = &layer->neurons[i];
    neuron->value_pre = neuron->bias;
    if (layer->type == NN_output) {
      for (int j = 0; j < layer->feed->size; j++)
        neuron->value_pre += neuron->weights[j] * layer->feed->neurons[j].value;
    }
    //no activation!
    neuron->value = neuron->value_pre;
  }
}

void NN_init_neural_network(NN_neural_network_t *nn, const NN_info_t *params) {
  nn->info = *params;
  nn->info.learning_rate = fabs(nn->info.learning_rate);
  nn->info.l2_decay = fabs(nn->info.l2_decay);
  CLAMP(nn->info.hidden_layers_size, 1, NN_MAX_HIDDEN_LAYERS);
  CLAMP(nn->info.input_size, 1, NN_MAX_NEURONS);
  CLAMP(nn->info.output_size, 1, NN_MAX_NEURONS);
  for (int i = 0; i < nn->info.hidden_layers_size; i++)
    CLAMP(nn->info.neurons_per[i], 1, NN_MAX_NEURONS);

  nn->input_size = nn->info.input_size;
  nn->output_size = nn->info.output_size;

  init_neural_first_hidden_layer(&nn->hidden_layers[0], nn->info.neurons_per[0], nn->info.input_size, nn->input);

  int nls = nn->info.hidden_layers_size;
  for (int i = 1; i < nls; i++) {
    init_neural_layer(&nn->hidden_layers[i], nn->info.neurons_per[i], &nn->hidden_layers[i - 1], 0);
  }
  init_neural_layer(&nn->output_layer, nn->info.output_size, &nn->hidden_layers[nls - 1], 1);
}

void NN_forward_propagate(NN_neural_network_t *nn) {
  for (int i = 0; i < nn->info.hidden_layers_size; i++) {
    neural_layer_propagate(&nn->hidden_layers[i], nn->input_size, nn->info.activation);
  }
  neural_layer_propagate_regress(&nn->output_layer);
  for (int i = 0; i < nn->info.output_size; i++)
    nn->prediction[i] = nn->output_layer.neurons[i].value;
}

// the effing meat and potatoes of this whol thing
void NN_backward_propagate(NN_neural_network_t *nn) {
  double learning_rate = nn->info.learning_rate;
  double lambda = nn->info.l2_decay;

  int output_size = nn->info.output_size;
// calculate output layer errors and gradients
  NN_neural_layer_t *output_layer = &nn->output_layer;
  NN_neuron_t *output_neurons = output_layer->neurons;

  /*
   double mse = 0.0;
   for (int i = 0; i < output_size; i++) {
   double error = output_neurons[i].value - nn->target[i];
   mse += error * error;
   }
   */

  // compute output layer error
  for (int i = 0; i < output_size; i++)
    output_neurons[i].delta = output_neurons[i].value - nn->target[i];

  // compute hidden layers error
  NN_neural_layer_t *next_layer = output_layer;
  for (int l = nn->info.hidden_layers_size - 1; l >= 0; l--) {
    NN_neural_layer_t *curr_layer = &nn->hidden_layers[l];
    NN_neuron_t *curr_neurons = curr_layer->neurons;
    NN_neuron_t *next_neurons = next_layer->neurons;

    for (int i = 0; i < curr_layer->size; i++) {
      double sum = 0.0;
      for (int j = 0; j < next_layer->size; j++)
        sum += next_neurons[j].delta * next_neurons[j].weights[i];
      curr_neurons[i].delta = sum * act_deriv(curr_neurons[i].value, nn->info.activation);
    }
    next_layer = curr_layer;
  }

  // update output layer weights and biases

  NN_neural_layer_t *last_hidden_layer = output_layer->feed;
  NN_neuron_t *last_hidden_neurons = last_hidden_layer->neurons;
  for (int i = 0; i < output_size; i++) {
    for (int j = 0; j < last_hidden_layer->size; j++)
      output_neurons[i].weights[j] -= learning_rate * output_neurons[i].delta * last_hidden_neurons[j].value;
    output_neurons[i].bias -= learning_rate * output_neurons[i].delta;
  }

  next_layer = output_layer;
  for (int l = nn->info.hidden_layers_size - 1; l >= 0; l--) {
    NN_neural_layer_t *curr_layer = &nn->hidden_layers[l];  //next_layer->feed

    // update weights and bias
    for (int i = 0; i < curr_layer->size; i++) {
      NN_neuron_t *neuron = &curr_layer->neurons[i];
      neuron->bias -= learning_rate * neuron->delta;

      if (curr_layer->type > 0) {  // feed is previous layer
        NN_neural_layer_t *prev_layer = curr_layer->feed;
        for (int j = 0; j < prev_layer->size; j++) {
          neuron->weights[j] -= learning_rate * (neuron->delta * prev_layer->neurons[j].value - lambda * neuron->weights[j]);
        }
      } else if (curr_layer->type == 0) {  // feed in the input
        for (int j = 0; j < nn->info.input_size; j++) {
          neuron->weights[j] -= learning_rate * (neuron->delta * nn->input[j] - lambda * neuron->weights[j]);
        }
      }
    }
    next_layer = curr_layer;
  }
}

double NN_train_neural_network(NN_neural_network_t *nn) {
  NN_forward_propagate(nn);
  NN_backward_propagate(nn);
  double mse = 0.0f;
  for (int j = 0; j < nn->output_size; j++) {
    double delta = nn->prediction[j] - nn->target[j];
    mse += delta * delta;
  }
  return mse / (double) nn->output_size;
}

void NN_export_neural_network(NN_neural_network_t *nn, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp)
    return;
  fprintf(fp, "AC %d\n", nn->info.activation);
  fprintf(fp, "L2 %+.17g\n", nn->info.l2_decay);
  fprintf(fp, "LR %+.17g\n", nn->info.learning_rate);
  fprintf(fp, "NI %d\n", nn->info.input_size);
  fprintf(fp, "NO %d\n", nn->info.output_size);
  fprintf(fp, "NH %d\n", nn->info.hidden_layers_size);
  for (int i = 0; i < nn->info.hidden_layers_size; i++) {
    fprintf(fp, "HID:\n");
    int feed_size = (i == 0) ? nn->input_size : nn->hidden_layers[i - 1].size;
    for (int j = 0; j < nn->hidden_layers[i].size; j++) {
      for (int k = 0; k < feed_size; k++)
        fprintf(fp, "W:%+.17g ", nn->hidden_layers[i].neurons[j].weights[k]);
      fprintf(fp, "B:%+.17g\n", nn->hidden_layers[i].neurons[j].bias);
    }
  }
  fprintf(fp, "OUT:\n");
  for (int j = 0; j < nn->output_layer.size; j++) {
    for (int k = 0; k < nn->output_layer.feed->size; k++)
      fprintf(fp, "W:%.17g ", nn->output_layer.neurons[j].weights[k]);
    fprintf(fp, "B:%.17g\n", nn->output_layer.neurons[j].bias);
  }

  fclose(fp);
}

static void read_neuron_values(NN_neuron_t *neuron, const char *str) {
  int w = 0;
  neuron->bias = 0.0;  // Default in case B: isn't found

  char tmp[8 * 1024];
  strcpy(tmp, str);
  char *tok = strtok(tmp, " ");
  while (tok) {
    if (tok[0] == 'W' && tok[1] == ':')
      neuron->weights[w++] = atof(tok + 2);
    if (tok[0] == 'B' && tok[1] == ':')
      neuron->bias = atof(tok + 2);
    tok = strtok(NULL, " \n");
  }
}

void NN_import_neural_network(NN_neural_network_t **nn, const char *filename) {
  if (!nn)
    return;

  if (*nn) {
    free(*nn);
    *nn = NULL;
  }

  FILE *fp = fopen(filename, "r");
  if (!fp)
    return;
  char line[8 * 1024];

  NN_info_t info;
  memset(&info, 0, sizeof(NN_info_t));

  while (fgets(line, sizeof(line), fp)) {
    if (0 == strncmp(line, "AC", 2)) {
      sscanf(line, "AC %d", (int*) &info.activation);
    } else if (0 == strncmp(line, "L2", 2)) {
      sscanf(line, "L2 %lg", &info.l2_decay);
    } else if (0 == strncmp(line, "LR", 2)) {
      sscanf(line, "LR %lg", &info.learning_rate);
    } else if (0 == strncmp(line, "NI", 2)) {
      sscanf(line, "NI %d", &info.input_size);
    } else if (0 == strncmp(line, "NO", 2)) {
      sscanf(line, "NO %d", &info.output_size);
    } else if (0 == strncmp(line, "HID:", 4)) {
      info.hidden_layers_size++;
    }
  }
  fseek(fp, 0, SEEK_SET);
  int l = -1;
  while (fgets(line, sizeof(line), fp)) {
    if (0 == strncmp(line, "OUT:", 4))
      break;
    if (0 == strncmp(line, "HID:", 4)) {
      l++;
      info.neurons_per[l] = 0;
    } else if ('W' == line[0])
      info.neurons_per[l]++;
  }
  fseek(fp, 0, SEEK_SET);

  *nn = (NN_neural_network_t*) malloc(sizeof(NN_neural_network_t));
  NN_neural_network_t *network = *nn;
  NN_init_neural_network(network, &info);

  l = -1;
  int n = 0;
  while (fgets(line, sizeof(line), fp)) {
    if (0 == strncmp(line, "OUT:", 4))
      break;
    if (0 == strncmp(line, "HID:", 4)) {
      l++;
      n = 0;
    } else if ('W' == line[0])
      read_neuron_values(&(*nn)->hidden_layers[l].neurons[n++], line);
  }
  n = 0;
  while (fgets(line, sizeof(line), fp)) {
    if ('W' == line[0])
      read_neuron_values(&(*nn)->output_layer.neurons[n++], line);
  }

  fclose(fp);
}

static void init_recurrent_neuron(RNN_neuron_t *neuron, int n, int d) {
  neuron->bias = 0.0;
  for (int i = 0; i < n; i++)
    neuron->weights[i] = NN_random(2.0, -1.0);
  for (int i = 0; i < d; i++)
    neuron->recurrent_weights[i] = NN_random(2.0, -1.0);
}

static void init_recurrent_neural_first_hidden_layer(RNN_neural_layer_t *layer, int size, int input_size, const double *input) {
  layer->type = NN_first;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->input = input;
  for (int i = 0; i < layer->size; i++)
    init_recurrent_neuron(&layer->neurons[i], input_size, layer->size);
}

static void init_recurrent_neural_hidden_layer(RNN_neural_layer_t *layer, RNN_neural_layer_t *previous_layer, int size) {
  layer->type = NN_hidden;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->feed = previous_layer;
  for (int i = 0; i < layer->size; i++)
    init_recurrent_neuron(&layer->neurons[i], layer->feed->size, layer->size);
}

static void init_recurrent_neural_output_layer(NN_neural_layer_t *layer, RNN_neural_layer_t *previous_layer, int size) {
  layer->type = NN_output;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->recurrent_feed = previous_layer;
  for (int i = 0; i < layer->size; i++)
    init_neuron(&layer->neurons[i], layer->recurrent_feed->size);
}

static void recurrent_neural_layer_propagate_hidden(RNN_neural_layer_t *layer, int input_size, int t, int depth) {
  int t_previous = (t - 1 + depth) % depth;
  for (int i = 0; i < layer->size; i++) {
    RNN_neuron_t *neuron = &layer->neurons[i];
    double sum = neuron->bias;

    // input sum
    if (layer->type == NN_first) {
      for (int j = 0; j < input_size; j++)
        sum += neuron->weights[j] * layer->input[j];
    } else {
      for (int j = 0; j < layer->feed->size; j++)
        sum += neuron->weights[j] * layer->feed->neurons[j].history[t];
    }

    // recurrent sum
    for (int j = 0; j < layer->size; j++)
      sum += neuron->recurrent_weights[j] * layer->neurons[j].history[t_previous];

    neuron->history[t] = tanh_act(sum);
  }
}

static void recurrent_neural_layer_propagate_output(NN_neural_layer_t *layer, int t) {
  if (layer->type != NN_output)
    return;

  for (int i = 0; i < layer->size; i++) {
    NN_neuron_t *neuron = &layer->neurons[i];
    double sum = neuron->bias;
    for (int j = 0; j < layer->feed->size; j++)
      sum += neuron->weights[j] * layer->recurrent_feed->neurons[j].history[t];

    neuron->value = sigmoid_act(sum);
  }
}

void RNN_init_neural_network(RNN_neural_network_t *rnn, const RNN_info_t *params) {
  rnn->info.hidden_layers_size = params->hidden_layers_size;
  CLAMP(rnn->info.hidden_layers_size, 1, NN_MAX_HIDDEN_LAYERS);
  rnn->info.input_size = params->input_size;
  CLAMP(rnn->info.input_size, 1, NN_MAX_NEURONS);
  rnn->info.output_size = params->output_size;
  CLAMP(rnn->info.output_size, 1, NN_MAX_NEURONS);
  for (int i = 0; i < rnn->info.hidden_layers_size; i++) {
    rnn->info.neurons_per[i] = params->neurons_per[i];
    CLAMP(rnn->info.neurons_per[i], 1, NN_MAX_NEURONS);
  }
  rnn->info.learning_rate = fabs(params->learning_rate);
  rnn->info.l2_decay = fabs(params->l2_decay);
  rnn->info.bptt_depth = params->bptt_depth;
  CLAMP(rnn->info.bptt_depth, 1, RNN_MAX_DEPTH);

  init_recurrent_neural_first_hidden_layer(&rnn->hidden_layers[0], rnn->info.neurons_per[0], rnn->info.input_size, rnn->input);
  int nls = rnn->info.hidden_layers_size;
  for (int i = 1; i < nls; i++) {
    init_recurrent_neural_hidden_layer(&rnn->hidden_layers[i], &rnn->hidden_layers[i - 1], rnn->info.neurons_per[i]);
  }
  init_recurrent_neural_output_layer(&rnn->output_layer, &rnn->hidden_layers[nls - 1], rnn->info.output_size);
}

void RNN_forward_propagate(RNN_neural_network_t *rnn) {
  rnn->t = (rnn->t + 1) % rnn->info.bptt_depth;
  for (int i = 0; i < rnn->info.hidden_layers_size; i++) {
    recurrent_neural_layer_propagate_hidden(&rnn->hidden_layers[i], rnn->info.input_size, rnn->t, rnn->info.bptt_depth);
  }

  recurrent_neural_layer_propagate_output(&rnn->output_layer, rnn->t);
  for (int i = 0; i < rnn->info.output_size; i++)
    rnn->prediction[i] = rnn->output_layer.neurons[i].value;
}

#pragma GCC diagnostic pop
