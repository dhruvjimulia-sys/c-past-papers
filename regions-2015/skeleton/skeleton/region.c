#include "region.h"
#include "image.h"
#include "typedefs.h"
#include "list.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/////ALL THESE FUNCTIONS ARE PROVIDED FOR YOU/////////////////////
/////DO NOT MODIFY THEM///////////////////////////////////////////

// Our colour function of choice
uint8_t region_colour(const region_t *region)
{
  const unsigned depth = region->depth;
  return (depth * 1879u) % 255;
}

// Resets a point to (x,y)
void init_point(point_t *point, int x, int y)
{
  point->x = x;
  point->y = y;
}

// Resets an extent to (width, height)
void init_extent(extent_t *extent, int width, int height)
{
  extent->width = width;
  extent->height = height;
}


// Allocates a region. You may assume that the return value is non-NULL.
region_t *region_allocate(void)
{
  region_t *new_region = malloc(sizeof(region_t));

  if (new_region == NULL)
  {
    perror("region_allocate");
    exit(EXIT_FAILURE);
  }

  return new_region;
}

// Prints a textual description of a region to the supplied FILE*.
void print_region(FILE *out, const region_t *region)
{
  assert(out != NULL);
  assert(region != NULL);

  fprintf(out, "Region of depth %i at (%i, %i) of extent (%i, %i)\n",
      region->depth, region->position.x, region->position.y,
      region->extent.width, region->extent.height);
}

// Finds all regions located in "image" and adds them to "regions".
// Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void find_regions(list_t *regions, image_t* image)
{
  region_t *image_region = region_allocate();

  image_region->depth = 0;
  init_point(&image_region->position, 0, 0);
  init_extent(&image_region->extent, image->width, image->height);

  list_insert_ascending(regions, image_region);
  find_sub_regions(regions, image, image_region);
}

///////////////////////////////////////////////////////////////////

/////////////////////////USEFUL FUNCTIONS///////////////////////////
///////////////////////////////////////////////////////////////////

// Compares two points lexicographically.
// Returns 1 if first is less than second, otherwise 0.
// Ordering of comparison is [y, x].
//
// You are NOT required to implement this function. However you may find
// it helpful to do so for Part I, Q1.
int point_compare_less(const point_t *first, const point_t *second)
{
  return first->y < second->y || (first->y == second->y && first->x < second->x);;
}

// Deallocates a region.
//
// You are NOT required to implement this function. However, you may
// find it helpful to do so for Part I, Q3.
void region_destroy(region_t *region)
{
	assert(region);
	free(region);
}
///////////////////////////////////////////////////////////////////

////////////////TO BE IMPLEMENTED///////////////////////////////////
////////////////////////////////////////////////////////////////////

// Compares two regions.
// Returns 1 if position of first is lexicographically less than that
// of the second, otherwise returns 0.
// Ordering of the position comparison is [y, x].
int region_compare(const region_t *r1, const region_t *r2)
{
  return point_compare_less(&r1->position, &r2->position);
}

// Prints all regions in "regions" to "out".
// print_region (above) prints a textual description of a region
// to the supplied FILE*
void print_regions(FILE *out, list_t *regions)
{
  for (list_elem_t *l = list_begin(regions); l != list_end(regions); l = l->next) {
	  print_region(out, l->region);
  }
}

//
// Sets the specified region of image "image" to the intensity value "value".
//
void image_fill_region(image_t *image, const region_t *region, uint8_t value)
{
	for (int x = region->position.x; x < image->width && x < region->position.x + region->extent.width; x++) {
		for (int y = region->position.y; y < image->height && y < region->position.y + region->extent.height; y++) {
			set_pixel(image, x, y, value);
		}
	}
}

// Determines the extent of a region.
// position: the location of the top-left-hand corner of the region.
// image: the image to be searched.
// extent: this will be populated with the width and height of a region.
void find_extent(extent_t *extent, image_t *image, const point_t *position)
{
	int j = 0;
	uint8_t init_colour = get_pixel(image, position->x, position->y);
	for (; j < image->width && get_pixel(image, position->x + j, position->y) == init_colour; j++);
	int width = j;
	j = 0;
	for (; j < image->height && get_pixel(image, position->x, position->y + j) == init_colour; j++);
	int height = j;
	init_extent(extent, width, height);
}

// Finds all regions located in the region "current" of "image" and adds them
// to "regions".  Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void find_sub_regions(list_t* regions, image_t *image, const region_t *current)
{
	int ix = current->position.x;
	int iy = current->position.y;
	uint8_t icolour = get_pixel(image, ix, iy);
	for (int x = ix; x < ix + current->extent.width; x++) {
		for (int y = iy; y < iy + current->extent.height; y++) {
			if (x < image->width && y < image->height && get_pixel(image, x, y) != icolour) {
				extent_t *extent = malloc(sizeof(extent_t *));
				point_t top_left = (point_t) {x, y};
				find_extent(extent, image, &top_left);
				region_t *region = region_allocate();
				*region = (region_t) {top_left, *extent, current->depth + 1};
				free(extent);
				list_insert_ascending(regions, region);
				find_sub_regions(regions, image, region);
				image_fill_region(image, region, icolour);
			}
		}
	}
}

// Renders all regions to an image using the supplied colour_function_t
// (declared in typedefs.h) to select pixel intensity.
void render_regions(image_t *image, list_t *regions,
                    colour_function_t get_colour)
{
	for (list_iter elem = list_begin(regions); elem != regions->footer; elem = elem->next) {
		image_fill_region(image, elem->region, get_colour(elem->region));
	}
}
///////////////////////////////////////////////////////////////////////////////
