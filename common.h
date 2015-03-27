#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Array2D {
    size_t rows;
    size_t cols;
    size_t elem_size;
    void **array;
    bool _is_slice;
} Array2D;

/* Handle loading the two tiff arguments. Allocates tif1 and tif2. They must be freed with
 * free_2d_array by the caller. */
void handle_args(int argc, const char **argv, Array2D *tif1, Array2D *tif2); 

/* Allocate a 2d array as a contiguous block of memory, with an outer array pointing into
   it at each row -- the entire inner array can be accessed as the index of the first
   element */
void alloc_2d_array(size_t rows, size_t columns, size_t element_size, Array2D *array);

/* Free an array created with alloc_2d_array */
void free_2d_array(Array2D *array);

/* Create a view into an existing 2d array -- does not copy. Only use free_2d_array
 * start is inclusive, end is exclusive */
void slice_2d_array(Array2D *array_in, size_t start_row, size_t end_row, size_t start_col,
                    size_t end_col, Array2D *array_out);
