#include "common.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>


void handle_args(int argc, const char **argv, uint16_t ***tif1, uint16_t ***tif2, uint32_t *tif1_height,
                 uint32_t *tif1_width, uint32_t *tif2_height, uint32_t *tif2_width) {
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

    *tif1_height = imagelength;
    *tif1_width = scanlinesize / 2;

    *tif1 = (uint16_t**)alloc_2d_array(*tif1_height, *tif1_width, sizeof(uint16_t));

    for(uint32 row = 0; row < *tif1_height; row++) {
        TIFFReadScanline(tif, (*tif1)[row], row, 0);
    }
    TIFFClose(tif);

    tif = TIFFOpen(argv[2], "r");
    if(!tif) {
        fprintf(stderr, "Second argument file (%s) is not a valid tiff file.\n", argv[2]);
        exit(2);
    }

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
    scanlinesize = TIFFScanlineSize(tif);

    *tif2_height = imagelength;
    *tif2_width = scanlinesize / 2;

    *tif2 = (uint16_t**)alloc_2d_array(*tif2_height, *tif2_width, sizeof(uint16_t));

    for(uint32 row = 0; row < *tif2_height; row++) {
        TIFFReadScanline(tif, (*tif2)[row], row, 0);
    }
    TIFFClose(tif);
}

/* Allocate a 2d array as a contiguous block of memory, with an outer array pointing into
   it at each row -- the entire inner array can be accessed as the index of the first
   element */
void **alloc_2d_array(size_t rows, size_t columns, size_t element_size) {
    void **outer_array = (void**)calloc(rows, sizeof(void**));
    void *inner_array = calloc(rows, columns * element_size);

    for(int row = 0; row < rows; row++) {
        outer_array[row] = inner_array + (row * columns * element_size);
    }

    return outer_array;
}

/* Free an array created with alloc_2d_array */
void free_2d_array(void **array) {
    free(*array);
    free(array);
}
