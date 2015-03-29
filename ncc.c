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

double compute_ncc(Array2D *a_in, Array2D *b_in) {
    int numel = a_in->rows * a_in->cols;

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