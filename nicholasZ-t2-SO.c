#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void createTreeOfProcess(int height);

double elapsedTime(struct timespec start, struct timespec end);

void testTree(pid_t originalPid,int treeHeight);

int main(int argc, char *argv[]){
    int userInput = atoi(argv[1]);
    pid_t originalPid = getpid();
    
    testTree(originalPid, userInput);
    
    return 0;
}

void testTree(pid_t originalPid, int treeHeight){
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    createTreeOfProcess(treeHeight);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Tempo demorado criando árvore: %.6f segundos\n", elapsedTime(start,end));
}

double elapsedTime(struct timespec start, struct timespec end){
    return (end.tv_sec - start.tv_sec) + 
    ((end.tv_nsec - start.tv_nsec) / 1e9);
}

void createTreeOfProcess(int height){
    if(height == 0) return;

    pid_t c1 = fork();
    if (c1 == 0) {
        // Filho 1
        printf("sou filho (1), pid: %d, pai: %d\n", getpid(), getppid());
        createTreeOfProcess(height - 1);
        exit(0);
    } else {
        printf("sou o pai, pid: %d, meu pai eh: %d\n", getpid(), getppid());
        pid_t c2 = fork();
        if (c2 == 0) {
            // Filho 2
            printf("sou filho (2), pid: %d, pai: %d\n", getpid(), getppid());
            createTreeOfProcess(height - 1);
            exit(0);
        } else {
            // Só o pai chega aqui
            waitpid(c1, NULL, 0);
            waitpid(c2, NULL, 0);
        }
    }
}

