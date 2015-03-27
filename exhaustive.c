#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, const char *argv[]) {
    Array2D tif1, tif2;

    handle_args(argc, argv, &tif1, &tif2);

    fprintf(stderr, "Img1:%dx%d\n", tif1.cols, tif1.rows);
    fprintf(stderr, "Img2:%dx%d\n", tif2.cols, tif2.rows);

    free_2d_array(&tif1);
    free_2d_array(&tif2);

    fprintf(stderr, "Done\n");
    
    return 0;
}
