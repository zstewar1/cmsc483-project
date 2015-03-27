#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, const char *argv[]) {
    Array2D tif1, tif2;

    handle_args(argc, argv, &tif1, &tif2);
    
    fprintf(stderr, "Img1:%ldx%ld\n", tif1.cols, tif1.rows);
    fprintf(stderr, "Img2:%ldx%ld\n", tif2.cols, tif2.rows);

    double inverse_count = 1.0/(tif1.rows * tif1.cols);
    double tif1_average = 0;

    // Take the whole array operate over it -- note: this will not work with a sliced array
    uint16_t *array = (uint16_t*)(*tif1.array);
    for(size_t i = 0; i < tif1.rows * tif1.cols; i++) {
        tif1_average += array[i] * inverse_count;
    }

    fprintf(stderr, "Img1 average: %f\n", tif1_average);

    inverse_count = 1.0/(tif2.rows * tif2.cols);
    double tif2_average = 0;

    // Take the whole array operate over it -- note: this will not work with a sliced array
    array = (uint16_t*)(*tif2.array);
    for(size_t i = 0; i < tif2.rows * tif2.cols; i++) {
        tif2_average += array[i] * inverse_count;
    }

    fprintf(stderr, "Img2 average: %f\n", tif2_average);

    for(long row_start = 1 - tif2.rows; row_start < tif1.rows; row_start++) {
        for(long col_start = 1 - tif2.cols; col_start < tif1.cols; col_start++) {
            long i1rs = clamp64(row_start, 0, tif1.rows);
            long i1re = clamp64(row_start + tif2.rows, 0, tif1.rows);
            long i1cs = clamp64(col_start, 0, tif1.cols);
            long i1ce = clamp64(col_start + tif2.cols, 0, tif1.cols);

            long i2rs = clamp64(-row_start, 0, tif2.rows);
            long i2re = clamp64(-row_start + tif1.rows, 0, tif2.rows);
            long i2cs = clamp64(-col_start, 0, tif2.cols);
            long i2ce = clamp64(-col_start + tif1.cols, 0, tif2.cols);

            Array2D i1slice, i2slice;

            slice_2d_array(&tif1, i1rs, i1re, i1cs, i1ce, &i1slice);
            slice_2d_array(&tif2, i2rs, i2re, i2cs, i2ce, &i2slice);

            free_2d_array(&i1slice);
            free_2d_array(&i2slice);
        }
    }

    free_2d_array(&tif1);
    free_2d_array(&tif2);

    fprintf(stderr, "Done\n");
    
    return 0;
}
