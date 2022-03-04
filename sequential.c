/*
Sequential code for the calculation of heat dissipation
*/
     


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define  N 10 

double calculateT(int pos, double rod[N] ,double C, double lapse);
int main(int argc, char* argv[]){

    time_t start, end;
    double lapsed_time;
    //start = clock();
    //end = clock();
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    double rod[N];
    double err = 10e-200;
    double T0 = 27;
    double Tl = 60;
    double Tr = 140;
    double L = 100;
    double C = 10e-5;

    //Fill the array with the first temp
    for(int i = 0; i < N; ++i){
        rod[i] = T0;
    }

    //Set the frontier conditions
    rod[0] = Tl;
    rod[N-1] = Tr;


    //For now the code runs for a set amount of time, should change it to a different stop condition
    int iters = 0;
    start = time(0);
    while(iters < 100000){
        for(int j = 1; j < N -1; ++j){
            end = time(0);
            lapsed_time = difftime(end, start);
            double oldT = rod[j];
            double newT = calculateT(j, rod, C, lapsed_time);
            printf("T in %d in time %f is %f\n", j, lapsed_time, newT);
            rod[j] = newT;
        } 
        sleep(1);
        iters += 1;
    }

    // printing final state
    for( int k = 0; k < N; ++k){
        printf("%f\n", rod[k]);
    }
}

double calculateT(int pos, double rod[N] ,double C, double lapse){
    double t = rod[pos] + C * (lapse / (pos*pos)) * (rod[pos -1] - 2*rod[pos]+ rod[pos +1]);
    return  t;
}