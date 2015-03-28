//
//  ncc.c
//  
//
//  Created by Mike Majurski on 3/28/15.
//
//

#include "ncc.h"


double compute_ncc(uint16_t* a_in, uint16_t* b_in, size_t numel) {
    double ncc = -1;
    double a_mean, b_mean, n, d, d1, d2;
    int i;
    double* a;
    double* b;
    
    a = malloc(numel*sizeof(double));
    b = malloc(numel*sizeof(double));
    
    // up convert to double
    for(i = 0; i < numel; i++)
        a[i] = (double)a_in[i];
    for(i = 0; i < numel; i++)
        b[i] = (double)b_in[i];
    
    
    // compute the mean of a
    a_mean = 0;
    for(i = 0; i < numel; i++)
        a_mean += a[i];
    a_mean = a_mean / (double)numel;
    
    // normalize a w.r.t mean
    for(i = 0; i < numel; i++)
        a[i] = a[i] - a_mean;
    
    // compute the mean of b
    b_mean = 0;
    for(i = 0; i < numel; i++)
        b_mean += b[i];
    b_mean = b_mean / (double)numel;
    
    // normalize b w.r.t mean
    for(i = 0; i < numel; i++)
        b[i] = b[i] - b_mean;
    
    // compute numerator
    n = 0;
    for(i = 0; i < numel; i++) {
        n += a[i] * b[i];
    }
    
    d1 = 0;
    for(i = 0; i < numel; i++)
        d1 += a[i] * a[i];
    d2 = 0;
    for(i = 0; i < numel; i++)
        d2 += b[i] * b[i];
    
    d = sqrt(d1) * sqrt(d2);
    if(d == 0) {
        ncc = -1;
    }else{
        ncc = n/d;
    }
    
    
    free(a);
    free(b);
    
    return ncc;
}