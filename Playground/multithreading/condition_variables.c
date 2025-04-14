#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cv;
int a, b, x;

static inline int f(int a, int b) { return a * b; }

void *t1_func(void *arg) {
    pthread_mutex_lock(&mutex);
    printf("Thread A: locking mutex\n");
    
    // Calculate x
    x = f(a, b);
    printf("Thread A: x = %d\n", x);
    
    // Use while loop to handle spurious wakeups
    while (x < 0 || x > 9) {
        printf("Thread A: waiting for condition\n");
        pthread_cond_wait(&cv, &mutex);
        // Recalculate x after waking up
        x = f(a, b);
        printf("Thread A: woke up, x = %d\n", x);
    }
    
    printf("Thread A: condition satisfied\n");
    pthread_mutex_unlock(&mutex);
    
    printf("Thread A: unlocking mutex\n");
    return NULL;
}

void *t2_func(void *arg) {
    pthread_mutex_lock(&mutex);
    printf("Thread B: locking mutex\n");

    // Change a and b
    printf("Thread B: changing a and b\n");
    a = 2;
    b = 4;
    
    // Calculate new x
    x = f(a, b);
    printf("Thread B: x = %d\n", x);
    
    // If condition is now met, signal waiting thread
    if (x >= 0 && x <= 9) {
        printf("Thread B: signaling condition variable\n");
        pthread_cond_signal(&cv);
    }
    
    pthread_mutex_unlock(&mutex);
    printf("Thread B: unlocking mutex\n");
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);
    
    // Set initial values
    a = 3;
    b = -5;
    
    // Create threads
    pthread_create(&t1, NULL, t1_func, NULL);
    
    // Small delay to ensure Thread A runs first
    // usleep(100000);
    
    pthread_create(&t2, NULL, t2_func, NULL);
    
    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("End! Final x = %d\n", x);
    
    // Clean up resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);
    return 0;
}