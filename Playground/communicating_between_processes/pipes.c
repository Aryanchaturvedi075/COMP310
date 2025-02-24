#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int main(){
    int32_t fd[2];
    if(pipe(fd) == -1){
        perror("An error occured while opening the pipe\n");
        exit(EXIT_FAILURE);              // better inside main() function       
    }

    pid_t id = fork();
    switch(id){
        case(-1):
            perror("An error occured while forking\n");
            return 2;

        case(0):
            close(fd[0]);   // close the read end of the pipe in the child process
            int32_t x;
            printf("%d: Enter a number: ", getpid());
            scanf("%d", &x);

            if(write(fd[1], &x, sizeof(int32_t)) == -1){
                perror("An error occured while writing to the pipe\n");
                return 3;
            }  

            close(fd[1]);   // close the write end of the pipe after finishing writing
            break;

        default:
            close(fd[1]);   // close the write end of the pipe in the parent process
            int32_t y;

            if(read(fd[0], &y, sizeof(int32_t)) == -1){
                perror("An error occured while reading from the pipe\n");
                return 4;
            }

            printf("%d: Message from the child process: %d\n\n", getpid(), y);
            close(fd[0]);   // close the read end of the pipe after finishing reading
    }
    return EXIT_SUCCESS;
}