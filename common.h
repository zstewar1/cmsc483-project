#pragma once

#include <stdint.h>
#include <stdlib.h>

/* Handle loading the two tiff arguments. Allocates tif1 and tif2. They must be freed with
 * free_2d_array by the caller. */
void handle_args(int argc, const char **argv, uint16_t ***tif1, uint16_t ***tif2, uint32_t *tif1_height,
                 uint32_t *tif1_width, uint32_t *tif2_height, uint32_t *tif2_width);

/* Allocate a 2d array as a contiguous block of memory, with an outer array pointing into
   it at each row -- the entire inner array can be accessed as the index of the first
   element */
void **alloc_2d_array(size_t rows, size_t columns, size_t element_size);

/* Free an array created with alloc_2d_array */
void free_2d_array(void **array);
