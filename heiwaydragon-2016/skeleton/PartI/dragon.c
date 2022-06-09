#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

#define NUM_DIRECTIONS 8
#define LEVEL 6

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

/* Returns a vector that describes the initial direction of the turtle. Each iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
	uint8_t mod_iterations = total_iterations % NUM_DIRECTIONS;
	vector_t start_direction;
	if (mod_iterations < 2 || mod_iterations == 7) {
		start_direction.dx = 1;
	} else if (mod_iterations >= 3 && mod_iterations <= 5) {
		start_direction.dx = -1;
	} else {
		start_direction.dx = 0;
	}
	if (mod_iterations >= 1 && mod_iterations <= 3) {
		start_direction.dy = 1;
	} else if (mod_iterations >= 5 && mod_iterations <= 7) {
		start_direction.dy = -1;
	} else {
		start_direction.dy = 0;
	}
	return start_direction;
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst,long x, long y)
{
	assert(dst);
	static const uint8_t values[LEVEL - 1] = { 100, 120, 150, 180, 200 };
	uint8_t level = LEVEL * ((double) drawn_pixels) / (dst->height * dst->height);
	uint8_t colour = level > 4 ? DEPTH : values[level];
	set_pixel(dst, x, y, colour);
}
/* Iterates though the characters of str, recusively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
  assert(dst);
  assert(str);
  if (iterations < 0) {
	  return;
  }
  int ostrlen = strlen(str);
  for (int i = 0; i < ostrlen; i++) {
	  long dx_temp = 0;
	  switch (str[0]) {
		  case '-':
			  dx_temp = direction.dx;
			  direction.dx = - direction.dy;
			  direction.dy = dx_temp;
			  break;
		  case '+':
			  dx_temp = direction.dx;
			  direction.dx = direction.dy;
			  direction.dy = - dx_temp;
			  break;
		  case 'F':
			  drawn_pixels++;
			  draw_greyscale(dst, x / scale, y / scale);
			  x += direction.dx;
			  y += direction.dy;
			  break;
		  case 'X':
			  string_iteration(dst, "X+YF", iterations - 1);
			  break;
		  case 'Y':
			  string_iteration(dst, "FX-Y", iterations - 1);
			  break;
	  }
	  str = str + 1;
  }
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
	image_t **image = malloc(sizeof(image_t **));
	init_image(image, (3 * size) / 2, size, GRAY, DEPTH);
	x = size; 
	y = size;
	scale = 2;
	direction = starting_direction(total_iterations);
	assert(total_iterations >= 0);
	string_iteration(*image, "FX+FX+", total_iterations);
	image_write("../output/twindragon.pgm", *image, PGM_FORMAT);
	image_free(*image);
	free(image);
}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
	int iterations = 0;
	if (argc >= 2) {
		iterations = atoi(argv[1]);
	} else {
		iterations = 9;
	}
	dragon(pow(2, iterations), 2 * iterations);
	return EXIT_SUCCESS;
}
