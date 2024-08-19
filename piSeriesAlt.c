// Daniel Armando Valdez Reyes || 21240
// Universidad del Valle de Guatemala || 8/18/2024
// Description: This program calculates the value of pi in parallel form and prints the speedup, efficiency, strong and weak scalability.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculatePi(int n, int numThreads){
    double sum = 0.0;
    // Sum odd and even terms of the series on the same loop, do loop on pairs
    #pragma omp parallel for num_threads(numThreads) reduction(+:sum) schedule(guided, 64) 
    for (int k = 0; k < n; k=k+2) {
        sum += 1.0/(2*k+1) - 1.0/(2*k+3);
    }
    
    double piAprox = 4.0*sum;
    return piAprox;
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("Usage: %s <iterations> <num_threads>\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]); // Number of iterations
    int numThreads = atoi(argv[2]); // Number of threads

    // Timing for sequential execution (1 thread)
    double startSeq = omp_get_wtime();
    double piSeq = calculatePi(n, 1);
    double endSeq = omp_get_wtime();
    double timeSeq = endSeq - startSeq;

    // Timing for parallel execution
    double startPar = omp_get_wtime();
    double piPar = calculatePi(n, numThreads);
    double endPar = omp_get_wtime();
    double timePar = endPar - startPar;

    // Calculate speedup and efficiency
    double speedup = timeSeq / timePar;
    double efficiency = speedup / numThreads;

    // Print results
    printf("The value of pi (sequential) is: %f\n", piSeq);
    printf("The value of pi (parallel) is: %f\n", piPar);
    printf("Time (sequential): %f seconds\n", timeSeq);
    printf("Time (parallel): %f seconds\n", timePar);
    printf("Speedup: %f\n", speedup);
    printf("Efficiency: %f\n", efficiency);

    // Strong scalability: fixed problem size, compare with different thread counts
    printf("Strong scalability (fixed problem size): Speedup: %f, Efficiency: %f\n", speedup, efficiency);

    // Weak scalability: to implement weak scalability, we would vary the problem size with the number of threads

    return 0;
}
