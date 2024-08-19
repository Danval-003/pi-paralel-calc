// Daniel Armando Valdez Reyes || 21240
// Universidad del Valle de Guatemala || 8/18/2024
// Description: This program calculates the value of pi on secuential form

#include <stdio.h>
#include <stdlib.h>


double calculatePi(int n){
    double factor = 1.0;
    double sum = 0.0;
    for (int k = 0; k < n; k++) {
    sum += factor/(2*k+1);
    factor = -factor;
    }
    double piAprox = 4.0*sum;
    return piAprox;
}


int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    double pi = calculatePi(n);
    printf("The value of pi is: %f\n", pi);
    return 0;
}

