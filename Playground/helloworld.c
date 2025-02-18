#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void){
    fork();                     // Create a child process
    printf("Hello World!\n");
    return 0;
}