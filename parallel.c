/*
 Parallel code for the calculation of heat dissipation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define  N 5000
int calculateT(int n, double *uk1, double *uk0,
double alpha, double dx, double dt, double bc0, double bc1, double * condition);
double check_condition(double * new, double * old, int size);

int main(int argc, char* argv[]){

    time_t start, end;
    double lapsed_time;
    //start = clock();
    //end = clock();
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    double * rod = (double *) malloc(N * sizeof(double));
    double * rod_1 = (double *) malloc(N * sizeof(double));
    double err = 10e-200;
    double T0 = 20;
    double Tl = 60;
    double Tr = 120;
    double L = 10;
    double C = 9e-3;
    //double C = 0.5;

    //Fill the array with the first temp
    for(int i = 0; i < N; ++i){
        rod[i] = T0;
        rod_1[i] = T0;
    }

    //Set the frontier conditions
    rod[0] = Tl;
    rod[N-1] = Tr;
    rod_1[0] = Tl;
    rod_1[N-1] = Tr;


    //For now the code runs for a set amount of time, should change it to a different stop condition
    double iters = 1;
    double condition = 100;
    double d_time = (0.5 / C)* (L/N) * (L/N);
    double start_time = omp_get_wtime();
    while(condition > 1e-4){

        calculateT(N, rod_1, rod, C, (L/N), d_time, Tl, Tr, (double *) &condition);
        //printf("Iter: %f Condition: %f \n", iters, condition);
        iters += 1;
        //condition = check_condition(rod_1, rod, N);
    }

    double time = omp_get_wtime() - start_time;
    printf("Time: %f\n", time);
    // printing final state
    for( int k = 0; k <= N; k+=100){
        printf("%f\n", rod_1[k]);
    }

    free(rod_1);
    free(rod);
}

double check_condition(double * new, double * old, int size) {
    double max = sizeof(int);
    max = -1 * max;
    for(int i=0; i<=size-1; ++i) {
        double val = new[i] - old[i];
        if (val > max) {
            max = val;
        }
    }
    // printf("---> %f\n", max);
    return max;
}

int calculateT(int n, double *uk1, double *uk0,
double alpha, double dx, double dt, double bc0, double bc1, double * condition){
    double r = alpha * dt / (dx * dx);
    // Sanity check for stability
    if (r > 0.5) return 0;

    #pragma omp parallel for 
    for(int i=1; i<n-1; ++i){
        uk1[i] = uk0[i] + r * (uk0[i-1] -2* uk0[i] + uk0[i+1]);
    }

    //condition
    
    double cond_value = check_condition(uk1, uk0, n);
    *condition = cond_value;

    for(int i=1; i<n-1; ++i) {
        uk0[i] = uk1[i];
    }
    // Enforce boundary conditions
    uk1[0] = bc0;
    uk1[n] = bc1;

    return 1;
}