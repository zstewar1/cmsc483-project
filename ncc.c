//
//  ncc.c
//
//
//  Created by Mike Majurski on 3/28/15.
//
//

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "ncc.h"

double compute_cost(Array2D *i1, Array2D *i2, long x, long y) {
    long i1rs = clamp64(x, 0, i1->rows);
    long i1re = clamp64(x + i2->rows, 0, i1->rows);
    long i1cs = clamp64(y, 0, i1->cols);
    long i1ce = clamp64(y + i2->cols, 0, i1->cols);

    long i2rs = clamp64(-x, 0, i2->rows);
    long i2re = clamp64(-x + i1->rows, 0, i2->rows);
    long i2cs = clamp64(-y, 0, i2->cols);
    long i2ce = clamp64(-y + i1->cols, 0, i2->cols);

    Array2D i1slice, i2slice;

    slice_2d_array(i1, i1rs, i1re, i1cs, i1ce, &i1slice);
    slice_2d_array(i2, i2rs, i2re, i2cs, i2ce, &i2slice);

    double ncc = compute_ncc(&i1slice, &i2slice);

    free_2d_array(&i1slice);
    free_2d_array(&i2slice);

    return ncc;
}

double compute_ncc(Array2D *a_in, Array2D *b_in) {
    int numel = a_in->rows * a_in->cols;

    if(numel == 1) {
        // Disregard 1x1 overlaps -- they would have an NCC of 1, but they are not counted here.
        return -2.0;
    }

    double *a = malloc(numel*sizeof(double));
    double *b = malloc(numel*sizeof(double));

    // up convert to double
    for(int row = 0; row < a_in->rows; row++) {
        for(int col = 0; col < a_in->cols; col++) {
            a[row * a_in->cols + col] = (double)((uint16_t**)a_in->array)[row][col];
            b[row * b_in->cols + col] = (double)((uint16_t**)b_in->array)[row][col];
        }
    }

    // compute the mean of a and b
    double a_mean = 0;
    double b_mean = 0;
    for(int i = 0; i < numel; i++) {
        a_mean += a[i];
        b_mean += b[i];
    }
    a_mean /= numel;
    b_mean /= numel;

    // numerator
    double n = 0;
    double d1 = 0, d2 = 0;

    // normalize a w.r.t mean, b w.r.t mean and compute numerator
    for(int i = 0; i < numel; i++) {
        // normalize a w.r.t mean
        a[i] = a[i] - a_mean;

        // normalize b w.r.t mean
        b[i] = b[i] - b_mean;

        // compute numerator
        n += a[i] * b[i];

        d1 += a[i] * a[i];
        d2 += b[i] * b[i];
    }

    free(a);
    free(b);

    double d = sqrt(d1) * sqrt(d2);
    if(d == 0) {
        return -1;
    }
    else {
        return n/d;
    }
}
