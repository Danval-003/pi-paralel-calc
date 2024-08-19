// Daniel Armando Valdez Reyes || 21240
// Universidad del Valle de Guatemala || 8/18/2024
// Description: This program calculates the value of pi on parallel form

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculatePi(int n, int numThreads){
        double sum = 0.0;
    int k;  // Declaración de 'k' antes de la directiva OpenMP
    double factor;  // Declaración de 'factor' antes de la directiva OpenMP

    #pragma omp parallel num_threads(numThreads) private(k, factor)
    {
        #pragma omp for reduction(+:sum)
        for (k = 0; k < n; k++) {
            factor = 1.0;
            if (k % 2 == 1) factor = -1.0;
            sum += factor / (2 * k + 1);
        }
    }
    
    double piAprox = 4.0 * sum;
    return piAprox;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    double pi = calculatePi(n, numThreads);
    printf("The value of pi is: %f\n", pi);
    return 0;
}
