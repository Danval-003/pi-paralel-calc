// Daniel Armando Valdez Reyes || 21240
// Universidad del Valle de Guatemala || 8/18/2024
// Description: This program calculates the value of pi on parallel form

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculatePi(int n, int numThreads) {
    double sum = 0.0;
    int k;
    double factor;

    #pragma omp parallel num_threads(numThreads) private(k, factor)
    {
        #pragma omp for reduction(+:sum)
        for (k = 0; k < n; k++) {
            factor = 1.0;
            if (k % 2 == 1) factor = -1.0;
            sum += factor / (2 * k + 1);
        }
    }

    return 4.0 * sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <maxThreads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);        // Tamaño del problema base
    int maxThreads = atoi(argv[2]);

    FILE *file = fopen("results.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Cabecera del CSV
    fprintf(file, "Hilos,Tiempo,Speedup,Eficiencia,Escalabilidad_Fuerte,Escalabilidad_Debil\n");

    // Cálculo secuencial con 1 hilo (para escalabilidad fuerte)
    double startTime = omp_get_wtime();
    double pi = calculatePi(n, 1);
    double endTime = omp_get_wtime();
    double T1 = endTime - startTime;
    printf("Pi (1 hilo): %f, Tiempo: %f segundos\n", pi, T1);
    fprintf(file, "1,%f,1,1,1,1\n", T1);

    // Cálculo con varios hilos y tamaño constante (escalabilidad fuerte)
    for (int numThreads = 2; numThreads <= maxThreads; numThreads++) {
        startTime = omp_get_wtime();
        pi = calculatePi(n, numThreads); // Problema de tamaño n para escalabilidad fuerte
        endTime = omp_get_wtime();
        double Tp = endTime - startTime;

        // Cálculo de Speedup, Eficiencia, y Escalabilidad Fuerte
        double speedup = T1 / Tp;
        double eficiencia = speedup / numThreads;
        double escalabilidadFuerte = speedup;

        // Cálculo de Escalabilidad Débil: usamos un problema de tamaño n * numThreads
        startTime = omp_get_wtime();
        pi = calculatePi(n * numThreads, 1); // Problema de tamaño n * numThreads
        endTime = omp_get_wtime();
        double T1_weaker = endTime - startTime; // Tiempo secuencial para tamaño grande
        double escalabilidadDebil = T1_weaker / Tp;

        // Imprimir y guardar en CSV
        printf("Pi (%d hilos): %f, Tiempo: %f segundos, Speedup: %f, Eficiencia: %f, Escalabilidad_Fuerte: %f, Escalabilidad_Debil: %f\n", 
               numThreads, pi, Tp, speedup, eficiencia, escalabilidadFuerte, escalabilidadDebil);
        fprintf(file, "%d,%f,%f,%f,%f,%f\n", numThreads, Tp, speedup, eficiencia, escalabilidadFuerte, escalabilidadDebil);
    }

    fclose(file);
    printf("Resultados guardados en results.csv\n");
    
    return 0;
}
