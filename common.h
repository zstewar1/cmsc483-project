#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Array2D {
    int64_t rows;
    int64_t cols;
    size_t elem_size;
    void **array;
    bool _is_slice;
} Array2D;

static inline uint16_t clamp16u(uint16_t val, uint16_t min, uint16_t max) {
    if(val < min) return min;
    else if(val > max) return max;
    else return val;
}

static inline int64_t clamp64(int64_t val, int64_t min, int64_t max) {
    if(val < min) return min;
    else if(val > max) return max;
    else return val;
}

/* Handle loading the two tiff arguments. Allocates tif1 and tif2. They must be freed with
 * free_2d_array by the caller. */
void handle_args(int argc, const char **argv, Array2D *tif1, Array2D *tif2); 

/* Allocate a 2d array as a contiguous block of memory, with an outer array pointing into
   it at each row -- the entire inner array can be accessed as the index of the first
   element */
void alloc_2d_array(int64_t rows, int64_t columns, size_t element_size, Array2D *array);

/* Free an array created with alloc_2d_array */
void free_2d_array(Array2D *array);

/* Create a view into an existing 2d array -- does not copy. Only use free_2d_array to cleanup.
 * start is inclusive, end is exclusive */
void slice_2d_array(Array2D *array_in, int64_t start_row, int64_t end_row, int64_t start_col,
                    int64_t end_col, Array2D *array_out);
