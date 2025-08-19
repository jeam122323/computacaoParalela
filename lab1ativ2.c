#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_FILHOS 4

int main() {

    for (int i = 0; i < N_FILHOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Erro ao criar processo filho.\n");
            return 1;
        } 
        if (pid == 0) {
            printf("FILHO: Meu PID é %d e o PID do meu pai é %d.\n", getpid(), getppid());
            exit(0);
        }
    }
    for (int i = 0; i < N_FILHOS; i++) {
        wait(NULL);
    }
    printf("PAI: Todos os %d filhos terminaram. O processo pai vai encerrar.\n", N_FILHOS);
    return 0;
}
