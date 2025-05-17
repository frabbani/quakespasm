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
  double tanh_x = tanh(x);
  return 1.0 - tanh_x * tanh_x;
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

static void init_neural_layer(NN_neural_layer_t *layer, int size,
                              NN_neural_layer_t *feed, int is_output) {
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

static void init_neural_first_hidden_layer(NN_neural_layer_t *layer, int size,
                                           int input_size, const double *input) {
  layer->type = NN_first;
  layer->size = size;
  CLAMP(layer->size, 1, NN_MAX_NEURONS);
  layer->input = input;
  for (int i = 0; i < layer->size; i++)
    init_neuron(&layer->neurons[i], input_size);
}

static void neural_layer_propagate(NN_neural_layer_t *layer, int input_size,
                                   NN_activation_type_t act_type) {
  for (int i = 0; i < layer->size; i++) {
    NN_neuron_t *neuron = &layer->neurons[i];
    neuron->value = neuron->bias;
    if (layer->type == NN_first) {
      for (int j = 0; j < input_size; j++)
        neuron->value += neuron->weights[j] * layer->input[j];
    } else {
      for (int j = 0; j < layer->feed->size; j++)
        neuron->value += neuron->weights[j] * layer->feed->neurons[j].value;
    }
    neuron->value = act_func(neuron->value, act_type);
  }
}

static void neural_layer_propagate_regress(NN_neural_layer_t *layer) {
  for (int i = 0; i < layer->size; i++) {
    NN_neuron_t *neuron = &layer->neurons[i];
    neuron->value = neuron->bias;
    if (layer->type == NN_output) {
      for (int j = 0; j < layer->feed->size; j++)
        neuron->value += neuron->weights[j] * layer->feed->neurons[j].value;
    }
    //no activation!
  }
}

void NN_init_neural_network(NN_neural_network_t *nn, const NN_info_t *params) {
  nn->info.hidden_layers_size = params->hidden_layers_size;
  CLAMP(nn->info.hidden_layers_size, 1, NN_MAX_HIDDEN_LAYERS);
  nn->info.input_size = params->input_size;
  CLAMP(nn->info.input_size, 1, NN_MAX_NEURONS);
  nn->info.output_size = params->output_size;
  CLAMP(nn->info.output_size, 1, NN_MAX_NEURONS);
  for (int i = 0; i < nn->info.hidden_layers_size; i++) {
    nn->info.neurons_per[i] = params->neurons_per[i];
    CLAMP(nn->info.neurons_per[i], 1, NN_MAX_NEURONS);
  }
  nn->input_size = nn->info.input_size;
  nn->output_size = nn->info.output_size;
  nn->info.learning_rate = fabs(params->learning_rate);
  nn->info.l2_decay = fabs(params->l2_decay);

  init_neural_first_hidden_layer(&nn->hidden_layers[0], nn->info.neurons_per[0],
                                 nn->info.input_size, nn->input);

  int nls = nn->info.hidden_layers_size;
  for (int i = 1; i < nls; i++) {
    init_neural_layer(&nn->hidden_layers[i], nn->info.neurons_per[i],
                      &nn->hidden_layers[i - 1], 0);
  }
  init_neural_layer(&nn->output_layer, nn->info.output_size,
                    &nn->hidden_layers[nls - 1], 1);
}

void NN_forward_propagate(NN_neural_network_t *nn) {
  for (int i = 0; i < nn->info.hidden_layers_size; i++) {
    neural_layer_propagate(&nn->hidden_layers[i], nn->input_size,
                           nn->info.activation);
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

  // ew, stack memory!
  double errors[NN_MAX_HIDDEN_LAYERS + 1][NN_MAX_NEURONS];

  double *output_error = errors[nn->info.hidden_layers_size];

  /*
   double mse = 0.0;
   for (int i = 0; i < output_size; i++) {
   double error = output_neurons[i].value - nn->target[i];
   mse += error * error;
   }
   */

  // compute output layer error
  for (int i = 0; i < output_size; i++) {
    double output = output_neurons[i].value;
    output_error[i] = (output - nn->target[i]);
  }

  // update output layer weights and biases
  NN_neural_layer_t *last_hidden_layer = output_layer->feed;
  NN_neuron_t *last_hidden_neurons = last_hidden_layer->neurons;
  for (int i = 0; i < output_size; i++) {
    for (int j = 0; j < last_hidden_layer->size; j++) {
      output_neurons[i].weights[j] -= learning_rate * output_error[i]
          * last_hidden_neurons[j].value;
    }
    output_neurons[i].bias -= learning_rate * output_error[i];
  }

  NN_neural_layer_t *curr_layer = NULL;
  NN_neural_layer_t *next_layer = output_layer;

  for (int l = nn->info.hidden_layers_size - 1; l >= 0; l--) {
    curr_layer = &nn->hidden_layers[l];  //next_layer->feed
    NN_neuron_t *next_neurons = next_layer->neurons;

    // this layer's neuron 0 is the input to and scaled by next layer's weight 0
    // this layer's neuron 1 is the input to and scaled by next layer's weight 1
    // this layer's neuron 2 is the input to and scaled by next layer's weight 2
    // etc..
    // the ith neuron is fed into all the next layer's neurons, so we
    // iterate over all of the next layer neurons (using their respective weight)
    // and operate accordingly
    double *hidden_error = errors[l];
    double *hidden_error_next = errors[l + 1];

    // compute hidden layer error:
    for (int i = 0; i < curr_layer->size; i++) {
      hidden_error[i] = 0;
      for (int j = 0; j < next_layer->size; j++) {
        hidden_error[i] += hidden_error_next[j] * next_neurons[j].weights[i];
      }
      hidden_error[i] *= act_deriv(curr_layer->neurons[i].value,
                                   nn->info.activation);
    }

    // update weights and bias
    for (int i = 0; i < curr_layer->size; i++) {
      NN_neuron_t *neuron = &curr_layer->neurons[i];
      neuron->bias -= learning_rate * hidden_error[i];
      if (curr_layer->type > 0) {  // feed is previous layer
        NN_neural_layer_t *prev_layer = curr_layer->feed;
        for (int j = 0; j < prev_layer->size; j++) {
          neuron->weights[j] -= learning_rate
              * (hidden_error[i] * prev_layer->neurons[j].value
                  - lambda * neuron->weights[j]);
        }
      } else if (curr_layer->type == 0) {  // feed in the input
        for (int j = 0; j < nn->info.input_size; j++) {
          neuron->weights[j] -= learning_rate
              * (hidden_error[i] * nn->input[j] - lambda * neuron->weights[j]);
        }
      }
    }
    next_layer = curr_layer;
  }

}

void NN_train_neural_network(NN_neural_network_t *nn) {
  NN_forward_propagate(nn);
  NN_backward_propagate(nn);
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

#pragma GCC diagnostic pop
