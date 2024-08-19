// Daniel Armando Valdez Reyes || 21240
// Universidad del Valle de Guatemala || 8/18/2024
// Description: This program calculates the value of pi on parallel form

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculatePi(int n, int numThreads){
    double factor = 1.0;
    double sum = 0.0;
    #pragma omp parallel for num_threads(numThreads)\reduction(+:sum)
    for (int k = 0; k < n; k++) {
        sum += factor/(2*k+1);
        factor = -factor;
    }
    double piAprox = 4.0*sum;
    return piAprox;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    double pi = calculatePi(n, numThreads);
    printf("The value of pi is: %f\n", pi);
    return 0;
}
