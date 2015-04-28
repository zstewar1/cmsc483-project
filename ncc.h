#pragma once
//
//  ncc.h
//
//
//  Created by Mike Majurski on 3/28/15.
//
//

struct Array2D;

double compute_cost(struct Array2D *i1, struct Array2D *i2, long x, long y);

/* Takes two 2D arrays of uint16_t */
double compute_ncc(struct Array2D *a_in, struct Array2D *b_in);
