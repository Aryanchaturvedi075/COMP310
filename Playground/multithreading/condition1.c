#include <stdio.h>
#include <pthread.h>

pthread_cond_t is_zero;
pthread_mutex_t mutex;
int shared_data = 1000;

void *thread_func(void *arg){
    while(shared_data > 0) {
        pthread_mutex_lock(&mutex);
        printf("%d\t", --shared_data);
        pthread_mutex_unlock(&mutex);
    }

    // printf("\nSignaling main...\n");
    pthread_cond_signal(&is_zero);
    return NULL;
}

int main (void){
    pthread_t tid;
    void * exit_status;
    int i;

    pthread_cond_init(&is_zero, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid, NULL, thread_func, NULL);
    printf("Start waiting in main...\n");

    pthread_mutex_lock(&mutex);
    while(shared_data!=0) pthread_cond_wait(&is_zero, &mutex);
    pthread_mutex_unlock(&mutex);

    printf("\nDone waiting in main!\n");
    pthread_join(tid, &exit_status);

    // Clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&is_zero);

    return 0;
}
