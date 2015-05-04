#include <complex.h>
#include <fftw3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, const char *argv[]) {
    Array2D tif1, tif2;

    handle_args(argc, argv, &tif1, &tif2);

    fprintf(stdout, "Img1:%ldx%ld\n", tif1.cols, tif1.rows);
    fprintf(stdout, "Img2:%ldx%ld\n", tif2.cols, tif2.rows);

    if(tif1.cols != tif2.cols || tif1.rows != tif2.rows) {
        fprintf(stderr, "Images must be the same size\n");
        exit(-1);
    }

    double complex *image1 = fftw_alloc_complex(tif1.rows * tif1.cols);
    double complex *image2 = fftw_alloc_complex(tif2.rows * tif2.cols);

    for(long i = 0; i < tif1.rows * tif1.cols; i++){
        image1[i] = ((uint16_t*)tif1.array[0])[i];
        image2[i] = ((uint16_t*)tif2.array[0])[i];
    }

    fftw_plan fft = fftw_plan_dft_2d(tif1.rows, tif1.cols, image1, image1, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(fft);
    fftw_destroy_plan(fft);
    fft = fftw_plan_dft_2d(tif2.rows, tif2.cols, image2, image2, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(fft);
    fftw_destroy_plan(fft);

    for(long i = 0; i < tif2.rows * tif2.cols; i++) {
        image1[i] *= conj(image2[i]);
        image1[i] /= cabs(image1[i]);
    }

    fft = fftw_plan_dft_2d(tif1.rows, tif1.cols, image1, image1, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(fft);
    fftw_destroy_plan(fft);

    long rmax = -1;
    long cmax = -1;
    double max = -1.0/0.0;

    fprintf(stdout, "Initial max: %f\n", max);
    for(long r = 0; r < tif1.rows; r++){
        for(long c = 0; c < tif1.cols; c++) {
            if(creal(image1[r * tif1.cols + c]) > max){
                max = creal(image1[r * tif1.cols + c]);
                rmax = r;
                cmax = c;
            }
        }
    }

    fprintf(stderr, "(x,y,ncc) = (%ld, %ld, %g)\n",rmax,cmax,0.0/0.0);

    fprintf(stdout, "Done\n");
}
