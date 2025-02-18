#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void){
    int id = fork();                     // Create a child process, this function call returns an id
    printf("Hello World from id: %d!\n", id);
    return 0;
}