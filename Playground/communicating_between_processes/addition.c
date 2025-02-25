#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int8_t arr[] = {1, 2, 3, 4, 1}; // making sure that the size of the array and the values inside the array are bounded to -128 to 127
    size_t len = sizeof(arr) / sizeof(arr[0]); 

    int fd[2];
    if(pipe(fd) == -1) exit(EXIT_FAILURE);

    pid_t id = fork();
    if (id == -1) {
        close(fd[0]), close(fd[1]);
        return 2;
    }

    int16_t sum = 0, child_sum;
    size_t start = (id == 0) ? 0 : len / 2;
    size_t end = (id == 0) ? len / 2 : len;
    for(size_t i = start; i < end; i++) sum += arr[i];
    
    if(id == 0){
        close(fd[0]);
        if (write(fd[1], &sum, sizeof(sum)) == -1) return 3;
        close(fd[1]);
    }
    else{
        wait(0);            // waiting for the child process to finish
        close(fd[1]);
        if (read(fd[0], &child_sum, sizeof(child_sum)) == -1) return 4;
        close(fd[0]);
        printf("The sum of the array is %d\n\n", sum + child_sum);
    }

    return EXIT_SUCCESS;
}