#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>  // Incluir este encabezado para strcmp

double calculatePi(int n, int numThreads, const char* scheduleType, int blockSize) {
    double sum = 0.0;
    int k;
    double factor;

    // Seleccionar el mecanismo de calendarización basado en los parámetros
    if (blockSize > 0) {
        #pragma omp parallel num_threads(numThreads) private(k, factor)
        {
            if (strcmp(scheduleType, "static") == 0) {
                #pragma omp for schedule(static, blockSize) reduction(+:sum)
                for (k = 0; k < n; k++) {
                    factor = (k % 2 == 0) ? 1.0 : -1.0;
                    sum += factor / (2 * k + 1);
                }
            } else if (strcmp(scheduleType, "dynamic") == 0) {
                #pragma omp for schedule(dynamic, blockSize) reduction(+:sum)
                for (k = 0; k < n; k++) {
                    factor = (k % 2 == 0) ? 1.0 : -1.0;
                    sum += factor / (2 * k + 1);
                }
            } else if (strcmp(scheduleType, "guided") == 0) {
                #pragma omp for schedule(guided, blockSize) reduction(+:sum)
                for (k = 0; k < n; k++) {
                    factor = (k % 2 == 0) ? 1.0 : -1.0;
                    sum += factor / (2 * k + 1);
                }
            }
        }
    } else {
        #pragma omp parallel num_threads(numThreads) private(k, factor)
        {
            if (strcmp(scheduleType, "auto") == 0) {
                #pragma omp for schedule(auto) reduction(+:sum)
                for (k = 0; k < n; k++) {
                    factor = (k % 2 == 0) ? 1.0 : -1.0;
                    sum += factor / (2 * k + 1);
                }
            }
        }
    }

    return 4.0 * sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <numThreads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);        // Tamaño del problema
    int numThreads = atoi(argv[2]); // Número de hilos fijos

    FILE *file = fopen("schedule_results.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Cabecera del CSV
    fprintf(file, "Scheduling,Block_Size,Hilos,Tiempo\n");

    // Configuraciones de scheduling y block sizes
    const char* schedules[] = {"static", "dynamic", "guided", "auto"};
    int blockSizes[] = {16, 64, 128};

    // Ejecutar para cada mecanismo de calendarización y cada block size
    for (int i = 0; i < 4; i++) {
        const char* scheduleType = schedules[i];

        // Para 'static', 'dynamic' y 'guided', probamos con diferentes block sizes
        if (strcmp(scheduleType, "auto") != 0) {
            for (int j = 0; j < 3; j++) {
                int blockSize = blockSizes[j];
                double startTime = omp_get_wtime();
                double pi = calculatePi(n, numThreads, scheduleType, blockSize);
                double endTime = omp_get_wtime();
                double timeTaken = endTime - startTime;

                // Imprimir y registrar en CSV
                printf("Scheduling: %s, Block Size: %d, Pi: %f, Tiempo: %f segundos\n", 
                       scheduleType, blockSize, pi, timeTaken);
                fprintf(file, "%s,%d,%d,%f\n", scheduleType, blockSize, numThreads, timeTaken);
            }
        } else {
            // Para 'auto', no se necesita block size
            double startTime = omp_get_wtime();
            double pi = calculatePi(n, numThreads, scheduleType, -1); // Block size no aplica
            double endTime = omp_get_wtime();
            double timeTaken = endTime - startTime;

            // Imprimir y registrar en CSV
            printf("Scheduling: %s, Block Size: N/A, Pi: %f, Tiempo: %f segundos\n", 
                   scheduleType, pi, timeTaken);
            fprintf(file, "%s,N/A,%d,%f\n", scheduleType, numThreads, timeTaken);
        }
    }

    fclose(file);
    printf("Resultados guardados en schedule_results.csv\n");

    return 0;
}
