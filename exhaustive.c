#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, const char *argv[]) {
    uint16_t **tif1, **tif2;
    uint32_t tif1_height, tif1_width, tif2_height, tif2_width;

    handle_args(argc, argv, &tif1, &tif2, &tif1_height, &tif1_width, &tif2_height, &tif2_width);

    fprintf(stderr, "Img1:%dx%d\n", tif1_width, tif1_height);
    fprintf(stderr, "Img2:%dx%d\n", tif2_width, tif2_height);

    free_2d_array((void**)tif1);
    free_2d_array((void**)tif2);

    fprintf(stderr, "Done\n");
    
    return 0;
}
