/*
 Sequential code for the calculation of heat dissipation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 100000
#define NX 100000
#define T0 20.0
#define Tl 60.0
#define Tr 120.0
#define L 10.0
#define STOP_CONDITION 1e-3


void initialize_arrays(double rod_0[], double rod_1[], int nx);

int main(int argc, char* argv[]){
    time_t start, end;
    double lapsed_time;

    // Rod arrays
    double * rod_0 = malloc(sizeof(double) * (NX+2));
    double * rod_1 = malloc(sizeof(double) * (NX+2));
    double *temp; 
    
    int iter;
    double max_diff = STOP_CONDITION;

    double dx = (L/NX);
    double dt = (0.5) * dx * dx;

    // TIME START
    start = clock();

    // Initialize rod arrays
    initialize_arrays(rod_0, rod_1, NX);

    for (iter = 0; (iter < N) && (max_diff >= STOP_CONDITION); ++iter) {
        max_diff = 0.0;
        {
	        /* Compute values and check stop condition on convergence */
            double max_diff_local = 0.0;
            double diff;
            for (int i = 1; i <= NX; ++i) {
                rod_1[i] = rod_0[i] + (dt/(dx*dx)) * (rod_0[i+1] - 2 * rod_0[i] + rod_0[i-1]);
                diff = fabs(rod_0[i] - rod_1[i]);
                if (diff > max_diff_local) max_diff_local = diff;
            }
            
            if (max_diff_local > max_diff) max_diff = max_diff_local;
        }

        /* Copy rod_1 to rod_0 by swapping pointers */
        temp = rod_1; rod_1 = rod_0; rod_0 = temp;
    }

    // TIME END
    end = clock();
    lapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Printing final state
    for( int k = 0; k < NX + 2; k+=5){
        printf("%f\n", rod_1[k]);
    }
    printf("\nTime: %f\n", lapsed_time);
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

    // Fill the array with the first temp
    for (int i = 1; i <= nx; ++i)
        rod_0[i] = rod_1[i] = T0;
}