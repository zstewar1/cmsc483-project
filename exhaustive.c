#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "ncc.h"

int main(int argc, const char *argv[]) {
    Array2D tif1, tif2;

    handle_args(argc, argv, &tif1, &tif2);
    
    fprintf(stderr, "Img1:%ldx%ld\n", tif1.cols, tif1.rows);
    fprintf(stderr, "Img2:%ldx%ld\n", tif2.cols, tif2.rows);

    long xtrans = 0;
    long ytrans = 0;
    double min_ncc = 1.0/0.0;

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

            double result = compute_ncc(&i1slice, &i2slice);

            if(result < min_ncc) {
                xtrans = row_start;
                ytrans = col_start;
            }

            free_2d_array(&i1slice);
            free_2d_array(&i2slice);
        }
    }

    free_2d_array(&tif1);
    free_2d_array(&tif2);

    fprintf(stdout, "XTranslation: %ld\nYTranslation %ld\n", xtrans, ytrans);

    fprintf(stderr, "Done\n");
    
    return 0;
}
