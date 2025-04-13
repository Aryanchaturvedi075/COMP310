#include <stdio.h>
#include <pthread.h>

int count;

void *mythread(void *arg) {
    int j;
    for (j = 0; j < 1000000; j++) count +=1;
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    count = 0;
    pthread_create(&p1, NULL, mythread, NULL);
    pthread_create(&p2, NULL, mythread, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("%d \n", count);
}
