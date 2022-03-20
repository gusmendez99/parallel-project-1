/*
 Parallel code for the calculation of heat dissipation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int N = 100000;
int NX = 100000;
double T0  = 20.0;
double Tl = 60.0;
double Tr = 120.0;
double L = 10.0;
double STOP_CONDITION = 1e-3;


void initialize_arrays(double rod_0[], double rod_1[], int nx);

int main(int argc, char* argv[]){


       if(argc < 4) {
        fprintf(stderr, "%s", "Few args. Args: N, T0, Tl, Tr\n");
        fprintf(stderr, "%s","Using prev defined vals\n");
    }
    else {
        NX = atoi(argv[1]);
        T0 = atof(argv[2]);
        Tl = atof(argv[3]);
        Tr = atof(argv[4]);
        N = NX;
    }


    // Rod arrays
    double * rod_0 = malloc(sizeof(double) * (NX+2));
    double * rod_1 = malloc(sizeof(double) * (NX+2));
    double *temp; 
    
    int iter;
    int nthreads;
    double max_diff = STOP_CONDITION;

    double dx = (L/NX);
    double dt = (0.5) * dx * dx;

    #pragma omp parallel 
    {
        #pragma omp single 
        {
            nthreads = omp_get_num_threads();
        }
    }

    // TIME START
    double start_time = omp_get_wtime();

    // Initialize rod arrays
    initialize_arrays(rod_0, rod_1, NX);

    for (iter = 0; (iter < N) && (max_diff >= STOP_CONDITION); ++iter) {
        max_diff = 0.0;
        #pragma omp parallel 
        {
	        /* Compute values and check stop condition on convergence */
            double max_diff_local = 0.0;
            double diff;
            #pragma omp for schedule(static)
            for (int i = 1; i <= NX; ++i) {
                rod_1[i] = rod_0[i] + (dt/(dx*dx)) * (rod_0[i+1] - 2 * rod_0[i] + rod_0[i-1]);
                diff = fabs(rod_0[i] - rod_1[i]);
                if (diff > max_diff_local) max_diff_local = diff;
            }
            #pragma omp critical
            {
                if (max_diff_local > max_diff) max_diff = max_diff_local;
            }
        }

        /* Copy rod_1 to rod_0 by swapping pointers */
        temp = rod_1; rod_1 = rod_0; rod_0 = temp;
    }

    // TIME END
    double lapsed_time = omp_get_wtime() - start_time;
    
    // Printing final state
    for( int k = 0; k < NX + 2; k+=100){
        printf("%f\n", rod_1[k]);
    }
    printf("\nTime: %f s\n", lapsed_time);
    printf("OpenMP working with %d threads\n", nthreads);
    printf("NX (points) = %d, N(max iters) = %d, stop condition = %g\n", NX, N, STOP_CONDITION);
    if (max_diff < STOP_CONDITION) {
        printf("-> Algorithm converged in %d iterations\n", iter);
    }
    else {
        printf("-> Fail to converge in %d iterations, max diff between values was %g\n", iter, max_diff);
    }

    free(rod_0);
    free(rod_1);
}


void initialize_arrays(double rod_0[], double rod_1[], int nx) {
    // Set the frontier conditions
    rod_0[0] = rod_1[0] = Tl; 
    rod_0[nx+1] = rod_1[nx+1] = Tr;

    //F ill the array with the first temp
    #pragma omp parallel for schedule(static)
    for (int i = 1; i <= nx; ++i)
        rod_0[i] = rod_1[i] = T0;
}