#include <stdio.h>
#include <pthread.h>

int count;

void mythread() {
    int j;
    for (j = 0; j < 2000000000; j++) count++;
}

int main(int argc, char *argv[]) {
    count = 0;
    mythread();
    printf("%d \n", count);
}
