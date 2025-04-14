#include <stdio.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t m;
    pthread_cond_t c;
    int val, turn, num_threads;
} counter_t;

typedef struct {
    int id;
    counter_t *ctr;
} thread_arg_t;

void *worker(void *arg) {
    thread_arg_t *targ = (thread_arg_t*)arg;
    int id = targ->id;
    counter_t *ctr = targ->ctr;
    
    for (;;) {
        pthread_mutex_lock(&ctr->m);
        
        if (ctr->val <= 0) {
            pthread_cond_signal(&ctr->c);
            pthread_mutex_unlock(&ctr->m);
            return NULL;
        }
        
        if (ctr->turn == id) {
            // printf("%d[T%d]\t", --ctr->val, id + 1);
            printf("%d\t", --ctr->val);
            ctr->turn = (ctr->turn + 1) % ctr->num_threads;
        }
        
        pthread_mutex_unlock(&ctr->m);
    }
}

int main(void) {
    counter_t ctr = {.val = 1000, .num_threads = 2};
    pthread_t t[ctr.num_threads];
    thread_arg_t args[ctr.num_threads];
    
    pthread_mutex_init(&ctr.m, NULL);
    pthread_cond_init(&ctr.c, NULL);
    
    printf("Start waiting in main...\n");

    // Create 3 threads
    for (int i = 0; i < ctr.num_threads; i++){
        args[i].id = i, args[i].ctr = &ctr;
        pthread_create(&t[i], NULL, worker, &args[i]);
    }

    pthread_mutex_lock(&ctr.m);
    while (ctr.val > 0) pthread_cond_wait(&ctr.c, &ctr.m);
    pthread_mutex_unlock(&ctr.m);
    
    printf("\nDone waiting in main!\n");

    // Joining all threads
    for (int i = 0; i < ctr.num_threads; i++)
        pthread_join(t[i], NULL);
    
    pthread_mutex_destroy(&ctr.m);
    pthread_cond_destroy(&ctr.c);
    
    return 0;
}