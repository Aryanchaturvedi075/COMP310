#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t threads[5];
int tid[5];
sem_t sem;

void * thread_func(void *arg){
    int tid_ = tid[* (int *) arg];
    printf("Thread %d created\n", tid_);
    sem_wait(&sem);
    for (int j=0; j<3; j++){
        printf("T%d run %d\n", tid_, j);
        usleep(2000);
    }
    sem_post(&sem);
}

int main(){
    sem_init(&sem, 0, 2);
    // sem initialized for all threads in the process; allow only 2 threads in the critical section at a time;
    int i;
    for (i=0; i<6; i++){
        tid[i]=i;
        pthread_create(&threads[i], NULL, thread_func, &tid[i]);
    }
    for (i=0; i<6; i++) pthread_join(threads[i], NULL);
    sem_destroy(&sem);
    return 0;
}