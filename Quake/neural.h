#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define NN_MAX_NEURONS 128
#define NN_MAX_HIDDEN_LAYERS  8

typedef struct {
  double weights[NN_MAX_NEURONS];
  double bias;
  double value;
} NN_neuron_t;

typedef enum {
  NN_first,
  NN_hidden,
  NN_output
} NN_layer_type_t;

typedef struct NN_neural_layer_s {
  int size;
  NN_layer_type_t type;
  NN_neuron_t neurons[NN_MAX_NEURONS];
  union {
    struct NN_neural_layer_s *feed;
    const double *input;
  };
} NN_neural_layer_t;

typedef enum {
  NN_sigmoid = 0,
  NN_tanh,
  NN_relu,
  NN_leakyrelu,
} NN_activation_type_t;

typedef struct {
  NN_activation_type_t activation;
  double learning_rate;
  double l2_decay;
  int input_size;
  int output_size;
  int hidden_layers_size;
  int neurons_per[NN_MAX_HIDDEN_LAYERS];
} NN_info_t;

typedef struct {
  NN_info_t info;
  double input[NN_MAX_NEURONS];
  int input_size;
  NN_neural_layer_t hidden_layers[NN_MAX_HIDDEN_LAYERS];
  NN_neural_layer_t output_layer;
  double target[NN_MAX_NEURONS];
  double prediction[NN_MAX_NEURONS];
  int output_size;

} NN_neural_network_t;

void NN_seed_random(unsigned long seed);
double NN_random(double scale, double offset);  // open range [offset, offset + scale)
void NN_init_neural_network(NN_neural_network_t *nn, const NN_info_t *params);
void NN_export_neural_network(NN_neural_network_t *nn, const char *filename);
void NN_import_neural_network(NN_neural_network_t **nn, const char *filename);
void NN_forward_propagate(NN_neural_network_t *nn);
void NN_backward_propagate(NN_neural_network_t *nn);
void NN_train_neural_network(NN_neural_network_t *nn);

#ifdef __cplusplus
}
#endif
