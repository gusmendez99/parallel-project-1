/*
Sequential code for the calculation of heat dissipation
*/

#include <stdio.h>
#include <stdlib.h>

#define  N 5000 

double calculateT(int pos, double rod[N] ,double C);
int main(int argc, char* argv[]){
    double rod[N];
    double err = 10e-200;
    double T0 = 200;
    double Tl = 60;
    double Tr = 140;
    double L = 100;
    double C = 0.5;

    //Fill the array with the first temp
    for(int i = 0; i < N; ++i){
        rod[i] = T0;
    }

    //Set the frontier conditions
    rod[0] = Tl;
    rod[N-1] = Tr;


    //For now the code runs for a set amount of time, should change it to a different stop condition
    int iters = 0;
    while(iters < 1000000){
        for(int j = 1; j < N -1; ++j){
            double oldT = rod[j];
            double newT = calculateT(j, rod, C);
            rod[j] = newT;
        } 
        iters += 1;
    }

    // printing final state
    for( int k = 0; k < N; ++k){
        printf("%f\n", rod[k]);
    }
}

double calculateT(int pos, double rod[N] ,double C){
    double t = rod[pos] + C * (rod[pos -1] - 2*rod[pos]+ rod[pos +1]);
    return  t;
}