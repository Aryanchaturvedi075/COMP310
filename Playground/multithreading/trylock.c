#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *a_func(void *arg) {
    long j;
    for (j = 0; j < 100; j++) {
        int got_both_locks = 0;
        
        while (!got_both_locks) {
            // Try to acquire first lock
            if (pthread_mutex_trylock(&lock1) == 0) {
                // Try to acquire second lock
                if (pthread_mutex_trylock(&lock2) == 0) {
                    // Success - got both locks
                    got_both_locks = 1;
                    printf("A");
                    pthread_mutex_unlock(&lock2);
                    pthread_mutex_unlock(&lock1);
                } else {
                    // Couldn't get lock2, release lock1 and try again
                    pthread_mutex_unlock(&lock1);
                    // Small delay to reduce contention
                    usleep(10);
                }
            } else {
                // Couldn't get lock1, wait a bit before retrying
                usleep(10);
            }
        }
    }
    return NULL;
}

void *b_func(void *arg) {
    long j;
    for (j = 0; j < 100; j++) {
        int got_both_locks = 0;
        
        while (!got_both_locks) {
            // Try to acquire first lock
            if (pthread_mutex_trylock(&lock2) == 0) {
                // Try to acquire second lock
                if (pthread_mutex_trylock(&lock1) == 0) {
                    // Success - got both locks
                    got_both_locks = 1;
                    printf("B");
                    pthread_mutex_unlock(&lock1);
                    pthread_mutex_unlock(&lock2);
                } else {
                    // Couldn't get lock1, release lock2 and try again
                    pthread_mutex_unlock(&lock2);
                    // Small delay to reduce contention
                    usleep(10);
                }
            } else {
                // Couldn't get lock2, wait a bit before retrying
                usleep(10);
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t a, b;
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&a, NULL, a_func, NULL);
    pthread_create(&b, NULL, b_func, NULL);

    pthread_join(a, NULL);
    pthread_join(b, NULL);
    
    printf("End!\n");
    
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    
    return 0;
}