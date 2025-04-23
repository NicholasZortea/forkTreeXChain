#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void createTreeOfProcess(int height);
double elapsedTime(struct timespec start, struct timespec end);

int main(int argc, char *argv[]){
    int treeHeight = atoi(argv[1]);
    pid_t originalPid = getpid();
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    createTreeOfProcess(treeHeight);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if(getpid() == originalPid){
        printf("Tempo demorado criando árvore: %.6f segundos\n", elapsedTime(start,end));
    }
    
    return 0;
}

double elapsedTime(struct timespec start, struct timespec end){
    return (end.tv_sec - start.tv_sec) + 
    (end.tv_nsec - start.tv_nsec) / 1e9;
}

void createTreeOfProcess(int height){
    if(height == 0) return;
    
    int status;
    pid_t pid;
    pid = fork();
    
    if(pid == 0){// eh um filho deve criar dois filhos
        printf("sou filho, meu pid eh: %d, meu pai eh: %d \n", getpid(), getppid());
        createTreeOfProcess(height-1);
        exit(0);
    } else if(pid > 0){//eh o pai deve criar outro filho
        printf("sou o pai, meu pid eh: %d, meu pai eh: %d \n", getpid(), getppid());
        pid_t outroPid = fork();
        if(outroPid == 0){ //eh o outro filho, deve criar dois filhos
            printf("sou filho, meu pid eh: %d, meu pai eh: %d \n", getpid(), getppid());
            createTreeOfProcess(height-1);
            exit(0);
        } else if(pid > 0){//eh o pai deve esperar
            wait(&status);
        }
    }
}
