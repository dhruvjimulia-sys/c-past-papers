#include "ann.h"
#include "layer.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Creates and returns a new ann. */
ann_t *ann_create(int num_layers, int *layer_outputs)
{
  /**** PART 2 - QUESTION 1 ****/
  assert(num_layers >= 0);
  ann_t *ann = malloc(sizeof(ann_t));
  ann->input_layer = layer_create();
  if (ann->input_layer == NULL) {
    return NULL;
  }
  ann->output_layer = ann->input_layer;
  if (num_layers == 0) {
    return ann;
  }
  layer_init(ann->input_layer, layer_outputs[0], NULL);
  for (uint32_t i = 1; i < num_layers; i++) {
    layer_t *layer = layer_create();
    if (layer == NULL) {
      return NULL;
    }
    layer_init(layer, layer_outputs[i], ann->output_layer);
    ann->output_layer->next = layer;
    ann->output_layer = layer;
  }
  return ann;
}

/* Frees the space allocated to ann. */
void ann_free(ann_t *ann)
{
  /**** PART 2 - QUESTION 2 ****/
  layer_t *next_layer = NULL;
  for (layer_t *layer = ann->input_layer; layer != NULL; layer = next_layer) {
    next_layer = layer->next;
    layer_free(layer);
  }
  free(ann);
}

/* Forward run of given ann with inputs. */
void ann_predict(ann_t const *ann, double const *inputs)
{
  /**** PART 2 - QUESTION 3 ****/
  memcpy(ann->input_layer->outputs, inputs, ann->input_layer->num_outputs * sizeof(double));
  if (ann->input_layer->next == NULL) {
    return;
  }
  for (layer_t *layer = ann->input_layer->next; layer != NULL; layer = layer->next) {
    layer_compute_outputs(layer);
  }
}

/* Trains the ann with single backprop update. */
void ann_train(ann_t const *ann, double const *inputs, double const *targets, double l_rate)
{
  /* Sanity checks. */
  assert(ann != NULL);
  assert(inputs != NULL);
  assert(targets != NULL);
  assert(l_rate > 0);

  /* Run forward pass. */
  ann_predict(ann, inputs);

  /**** PART 2 - QUESTION 4 ****/
  layer_t *olayer = ann->output_layer;
  for (int j = 0; j < olayer->num_outputs; j++) {
    olayer->deltas[j] = sigmoidprime(olayer->outputs[j]) * (targets[j] - olayer->outputs[j]);
  }
  for (layer_t *layer = olayer->prev; layer != ann->input_layer; layer = layer->prev) {
    layer_compute_deltas(layer);
  }
  for (layer_t *layer = ann->input_layer->next; layer != NULL; layer = layer->next) {
    layer_update(layer, l_rate);
  }
}
