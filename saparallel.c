#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

#include "common.h"
#include "ncc.h"

const double alpha = 0.999;

const long reduce_iter_count = 15;

int perform_hc_step(Array2D *i1, Array2D *i2, long* x, long *y, double *ncc);
void generate_neighbor(long rows, long cols, long *newx, long *newy);
void bound(long *x, long *y, Array2D *i);

void reduce(double *bncc, long *bx, long *by, int rank);

int main(int argc, const char *argv[]) {
    int rank, commsz;
    MPI_Init(&argc, (char***)&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Array2D tif1, tif2;

    handle_args(argc, argv, &tif1, &tif2);

    srand48(time(NULL) * rank);

    // Create initial valid translation
    long x = clamp64((long)(drand48() * tif1.rows), 1-tif1.rows, tif1.rows-1);
    long y = clamp64((long)(drand48() * tif1.cols), 1-tif1.cols, tif1.cols-1);

    double ncc = compute_cost(&tif1, &tif2, x, y);

    double temperature = 1000.0;
    long iter_count = 0;

    long bx = x, by = y;
    double bncc = ncc;

    while(temperature > 0.00001) {
        //fprintf(stderr, "Temperature: %f\n", temperature);

        temperature *= alpha;
        ++iter_count;

        if(iter_count % reduce_iter_count == 0) {
            if(rank == 0) {
                printf("%ld Iterations, ncc: %f, temperature: %f\n", iter_count, bncc, temperature);
            }
            reduce(&bncc, &bx, &by, rank);
        }

        if(perform_hc_step(&tif1, &tif2, &x, &y, &ncc)) {

            if(ncc > bncc) {
                bncc = ncc;
                bx = x;
                by = y;
            }
            continue;
        }

        long newx, newy;
        generate_neighbor(tif1.rows, tif1.cols, &newx, &newy);

        double newncc = compute_cost(&tif1, &tif2, newx, newy);

        if(newncc > ncc) {
            ncc = newncc;
            x = newx;
            y = newy;

            if(ncc > bncc) {
                bncc = ncc;
                bx = x;
                by = y;
            }
        }
        else {
            double energy = exp(fabs(ncc - newncc) / temperature);

            if(energy > drand48()) {
                ncc = newncc;
                x = newx;
                y = newy;

                if(ncc > bncc) {
                    bncc = ncc;
                    bx = x;
                    by = y;
                }
            }
        }
    }

    reduce(&bncc, &bx, &by, rank);

    if(rank == 0) {
        printf("Best: (x, y, c) = (%ld, %ld, %g)\n", bx, by, bncc);

        printf("%ld Iterations\n", iter_count);
    }

    MPI_Finalize();

    return 0;
}

int perform_hc_step(Array2D *i1, Array2D *i2, long* rx, long *ry, double *rncc) {
    long xvals[] = {0, 0, 0, 0};
    long yvals[] = {0, 0, 0, 0};
    double nccvals[] = {0, 0, 0, 0};

    xvals[0] = *rx - 1;
    yvals[0] = *ry;
    bound(&xvals[0], &yvals[0], i1);

    nccvals[0] = compute_cost(i1, i2, xvals[0], yvals[0]);

    xvals[1] = *rx + 1;
    yvals[1] = *ry;
    bound(&xvals[1], &yvals[1], i1);

    nccvals[1] = compute_cost(i1, i2, xvals[1], yvals[1]);

    xvals[2] = *rx;
    yvals[2] = *ry - 1;
    bound(&xvals[2], &yvals[2], i1);

    nccvals[2] = compute_cost(i1, i2, xvals[2], yvals[2]);

    xvals[3] = *rx;
    yvals[3] = *ry + 1;
    bound(&xvals[3], &yvals[3], i1);

    nccvals[3] = compute_cost(i1, i2, xvals[3], yvals[3]);

    int new = 0;
    for(int i = 0; i < 4; i++) {
        if(nccvals[i] > *rncc) {
            *rncc = nccvals[i];
            *rx = xvals[i];
            *ry = yvals[i];
            new = 1;
        }
    }

    return new;
}

void generate_neighbor(long rows, long cols, long *newx, long *newy) {
    *newx = (long)((rows-1) * drand48());
    *newy = (long)((cols-1) * drand48());

    if(drand48() > 0.5)
        *newx = -(*newx);
    if(drand48() > 0.5)
        *newy = -(*newy);

    *newx = clamp64(*newx, 1-rows, rows-1);
    *newy = clamp64(*newy, 1-cols, cols-1);
}

void bound(long *x, long *y, Array2D *i) {
    *x = clamp64(*x, 1-i->rows, i->rows-1);
    *y = clamp64(*y, 1-i->cols, i->cols-1);
}

void reduce(double *bncc, long *bx, long *by, int rank) {
    struct {double ncc; int rank;} reduce_data;
    reduce_data.ncc = *bncc;
    reduce_data.rank = rank;

    MPI_Allreduce(&reduce_data, &reduce_data, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);

    long locxy[2] = {*bx, *by};

    MPI_Bcast(locxy, 2, MPI_LONG, reduce_data.rank, MPI_COMM_WORLD);

    *bncc = reduce_data.ncc;
    *bx = locxy[0];
    *by = locxy[1];
}
