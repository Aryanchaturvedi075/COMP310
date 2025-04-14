#include <stdio.h>
#include <pthread.h>

pthread_mutex_t count_mutex;
int count;

void *mythread(void *arg) {
    int j;
    for (j = 0; j < 1000000000; j++) 
    {
        pthread_mutex_lock(&count_mutex);
        count +=1;
        pthread_mutex_unlock(&count_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    pthread_mutex_init(&count_mutex, NULL);

    count = 0;
    pthread_create(&p1, NULL, mythread, NULL);
    pthread_create(&p2, NULL, mythread, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("%d \n", count);
}
