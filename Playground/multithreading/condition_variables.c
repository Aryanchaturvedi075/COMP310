#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cv;  // Added condition variable declaration
int a, b, x;

static inline int f (int a, int b) { return a * b; }

void *t1_func(void *arg) {
    pthread_mutex_lock(&mutex);
    x = f(a, b);
    if ( x < 0 || x > 9)
        pthread_cond_wait (&cv, &mutex);
    // else
    //     pthread_cond_signal(&cv);  // Signal if condition is met
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *t2_func(void *arg) {
    pthread_mutex_lock(&mutex);
    a = 2, b = 4;
    x = f(a, b);
    if ( x < 0 || x > 9)
        pthread_cond_wait (&cv, &mutex);
    // else
    //     pthread_cond_signal(&cv);  // Signal if condition is met
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);  // Initialize condition variable
    
    a = 3, b = -5;

    pthread_create(&t1, NULL, t1_func, NULL);
    pthread_create(&t2, NULL, t2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("End!\n");

    // clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);
    return 0;
}