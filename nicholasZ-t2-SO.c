#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>

void createTreeOfProcess(int height);

double elapsedTime(struct timespec start, struct timespec end);

void testTree(int treeHeight);

void testChain(int processToCreate);

void createChainOfProcess(int processToCreate);

int main(int argc, char *argv[]){
    int userInput = atoi(argv[1]);
    if(userInput == NULL || userInput <= 0){
        printf("Entrada invalida!\n");
        return 0;
    }
    testTree(userInput);

    int processToCreate = pow(2, userInput+1) - 2;
    testChain(processToCreate);

    return 0;
}

double elapsedTime(struct timespec start, struct timespec end){
    return (end.tv_sec - start.tv_sec) + 
    ((end.tv_nsec - start.tv_nsec) / 1e9);
}

void testChain(int processToCreate){
    printf("\n\nProcessos em cadeia:\n");
    struct timespec start, end;
    printf("Sou o processo pai meu pid eh: %d, meu pai eh: %d\n",getpid(), getppid());
    clock_gettime(CLOCK_MONOTONIC, &start);
    createChainOfProcess(processToCreate);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsedTimex = elapsedTime(start,end);
    printf("Tempo demorado criando processos em cadeia: %.6f segundos\n", elapsedTimex);
}

void createChainOfProcess(int processToCreate){
    if(processToCreate != 0){
        fflush(stdout);
        pid_t currentPid = fork();
        int status;
        if(currentPid == 0){//eh o filho chamar funcao recursivamente
            createChainOfProcess(processToCreate-1);
            printf("Sou o processo de pid: %d, meu pai eh: %d, estou finalizando!\n", getpid(), getppid());
            exit(0);
        } else { //eh o pai, precisa esperar com wait
            wait(&status);
        }
    }
}

void testTree(int treeHeight){
    printf("Processos em arvore:\n");
    struct timespec start, end;
    printf("Sou o processo pai meu pid eh: %d, meu pai eh: %d\n",getpid(), getppid());
    clock_gettime(CLOCK_MONOTONIC, &start);
    createTreeOfProcess(treeHeight);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsedTimex = elapsedTime(start,end);
    printf("Tempo demorado criando árvore: %.6f segundos\n", elapsedTimex);
}

void createTreeOfProcess(int height){
    if(height != 0){
        fflush(stdout);
        pid_t c1 = fork();
        if (c1 == 0) {
            // Filho 1
            printf("sou filho (1), pid: %d, pai: %d, altura da arvore: %d\n", getpid(), getppid(), height);
            createTreeOfProcess(height - 1);
            printf("Processo de pid: %d, está terminando \n", getpid());
            exit(0);
        } else {
            fflush(stdout);
            pid_t c2 = fork();
            if (c2 == 0) {
                // Filho 2
                printf("sou filho (2), pid: %d, pai: %d, altura da arvore: %d\n", getpid(), getppid(), height);
                createTreeOfProcess(height - 1);
                printf("Processo de pid: %d, está terminando \n", getpid());
                exit(0);
            } else {
                // Só o pai chega aqui
                waitpid(c1, NULL, 0);
                waitpid(c2, NULL, 0);
            }
        }
    }
}

