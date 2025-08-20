#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 200000000
#define NUM_PROCESSES 4

// Funcao que simula uma carga de trabalho pesada
void heavy_work(double* vector, int start, int end) {
    for (int i = start; i < end; ++i) {
        vector[i] = sin(vector[i]) * cos(vector[i]) + sqrt(vector[i]);
    }
}

int main() {
    double* vector = (double*)malloc(VECTOR_SIZE * sizeof(double));
    if (vector == NULL) {
        perror("malloc failed");
        return 1;
    }
    for(long i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = (double)i;
    }
    struct timespec start, end;
    int chunk_size = VECTOR_SIZE / NUM_PROCESSES;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            int start_index = i * chunk_size;
            int end_index = (i + 1) * chunk_size;
            if (i == NUM_PROCESSES - 1) {
                end_index = VECTOR_SIZE;
            }
            heavy_work(vector, start_index, end_index);
            exit(0);
        }
    }
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Versao paralela executou em %f segundos\n", time_spent);

    // Para verificar o resultado (Atividade 3)
    // printf("Resultado de verificacao: vector[10] = %f\n", vector[10]);
    free(vector);
    return 0;
}
