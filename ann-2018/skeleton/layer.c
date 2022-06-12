#include "layer.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>


/* The sigmoid function and derivative. */
double sigmoid(double x)
{
  return 1 / (1 + exp(-x));
}

double sigmoidprime(double x)
{
  return x*(1 - x);
}

/* Creates a single layer. */
layer_t *layer_create()
{
  layer_t *layer = malloc(sizeof(layer_t));
  if (layer == NULL) {
    return NULL;
  }
  layer->num_inputs = 0;
  layer->num_outputs = 0;
  layer->prev = NULL;
  layer->next = NULL;
  layer->weights = NULL;
  layer->biases = NULL;
  layer->deltas = NULL;
  return layer;
}

/* Initialises the given layer. */
bool layer_init(layer_t *layer, int num_outputs, layer_t *prev)
{
  assert(layer);
  layer->num_outputs = num_outputs;
  layer->prev = prev;
  layer->outputs = calloc(num_outputs, sizeof(double));
  if (layer->outputs == NULL) {
    return true;
  }
  // if not input layer
  if (prev != NULL) {
    layer->num_inputs = prev->num_outputs;
    prev->next = layer;
    layer->biases = calloc(num_outputs, sizeof(double));
    if (layer->biases == NULL) {
      return true;
    }
    layer->deltas = calloc(num_outputs, sizeof(double));
    if (layer->deltas == NULL) {
      return true;
    }
    layer->weights = malloc(sizeof(double *) * layer->num_inputs);
    if (layer->weights == NULL) {
      return true;
    }
    for (uint32_t i = 0; i < layer->num_inputs; i++) {
      layer->weights[i] = malloc(sizeof(double *) * layer->num_outputs);
      if (layer->weights[i] == NULL) {
        return true;
      }
      for (uint32_t j = 0; j < layer->num_outputs; j++) {
        layer->weights[i][j] = ANN_RANDOM();
      }
    }
  }
  return false;
}

/* Frees a given layer. */
void layer_free(layer_t *layer)
{
  free(layer->outputs);
  if (layer->prev != NULL) {
    free(layer->biases);
    free(layer->deltas);
    for (uint32_t i = 0; i < layer->num_inputs; i++) {
      free(layer->weights[i]);
    }
    free(layer->weights);
  }
  free(layer);
}

/* Computes the outputs of the current layer. */
void layer_compute_outputs(layer_t const *layer)
{
  assert(layer->prev);
  for (uint32_t j = 0; j < layer->num_outputs; j++) {
    double sum = 0;
    for (uint32_t i = 0; i < layer->num_inputs; i++) {
      sum += layer->weights[i][j] * layer->prev->outputs[i];
    }
    layer->outputs[j] = sigmoid(layer->biases[j] + sum);
  }
}

/* Computes the delta errors for this layer. */
void layer_compute_deltas(layer_t const *layer)
{
  assert(layer->next);
  for (uint32_t i = 0; i < layer->next->num_inputs; i++) {
    double sum = 0;
    for (uint32_t j = 0; j < layer->next->num_outputs; j++) {
      sum += layer->next->weights[i][j] * layer->next->deltas[j];
    }
    layer->deltas[i] = sigmoidprime(layer->outputs[i]) * sum;
  }
}

/* Updates weights and biases according to the delta errors given learning rate. */
void layer_update(layer_t const *layer, double l_rate)
{
  /* objective: update layer->weights and layer->biases */
  for (uint32_t j = 0; j < layer->num_outputs; j++) {
    for (uint32_t i = 0; i < layer->num_inputs; i++) {
      layer->weights[i][j] = layer->weights[i][j] + l_rate * layer->prev->outputs[i] * layer->deltas[j];
    }
    layer->biases[j] = layer->biases[j] + l_rate * layer->deltas[j];
  }
}
