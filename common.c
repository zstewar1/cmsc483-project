#include "common.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>

void handle_args(int argc, const char **argv, Array2D *tif1, Array2D *tif2) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <tiff1> <tiff2>\n", argv[0]);
        fprintf(stderr, "Requires two arguments, which are the names of the tiff files to align.\n");
        fprintf(stderr, "Output is translation to apply to the second image to align it with the first.\n");
        exit(1);
    }

    TIFF* tif = TIFFOpen(argv[1], "r");
    if(!tif) {
        fprintf(stderr, "First argument file (%s) is not a valid tiff file.\n", argv[1]);
        exit(2);
    }
    
    uint32 imagelength;
    uint32 scanlinesize;
    
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
    scanlinesize = TIFFScanlineSize(tif);

    alloc_2d_array(imagelength, scanlinesize / 2, sizeof(uint16_t), tif1);

    for(uint32 row = 0; row < tif1->rows; row++) {
        TIFFReadScanline(tif, tif1->array[row], row, 0);
    }
    TIFFClose(tif);

    tif = TIFFOpen(argv[2], "r");
    if(!tif) {
        fprintf(stderr, "Second argument file (%s) is not a valid tiff file.\n", argv[2]);
        exit(2);
    }

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
    scanlinesize = TIFFScanlineSize(tif);

    alloc_2d_array(imagelength, scanlinesize / 2, sizeof(uint16_t), tif2);

    for(uint32 row = 0; row < tif2->rows; row++) {
        TIFFReadScanline(tif, tif2->array[row], row, 0);
    }
    TIFFClose(tif);
}

/* Allocate a 2d array as a contiguous block of memory, with an outer array pointing into
   it at each row -- the entire inner array can be accessed as the index of the first
   element */
void alloc_2d_array(int64_t rows, int64_t columns, size_t element_size, Array2D *array) {
    array->rows = rows;
    array->cols = columns;
    array->elem_size = element_size;
    array->_is_slice = false;
    
    array->array = (void**)calloc(array->rows, sizeof(void**));
    void *inner_array = calloc(array->rows, array->cols * array->elem_size);

    for(int64_t row = 0; row < array->rows; row++) {
        array->array[row] = inner_array + (row * array->cols * array->elem_size);
    }
}

/* Free an array created with alloc_2d_array */
void free_2d_array(Array2D *array) {
    if(!array->_is_slice)
        free(*array->array);
    free(array->array);
}

void slice_2d_array(Array2D *array_in, int64_t start_row, int64_t end_row, int64_t start_col,
                    int64_t end_col, Array2D *array_out) {
    array_out->rows = end_row - start_row;
    array_out->cols = end_col - start_col;
    array_out->elem_size = array_in->elem_size;
    array_out->_is_slice = true;

    array_out->array = (void**)calloc(array_out->rows, sizeof(void**));

    for(int64_t row = 0; row < array_out->rows; row++) {
        array_out->array[row] = array_in->array[row + start_row] + (start_col * array_in->elem_size);
    }
}
