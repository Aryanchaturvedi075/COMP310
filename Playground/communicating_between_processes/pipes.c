#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


int main(){
    int8_t fd[2];

    if (pipe(fd) == -1){
        perror("An error occurred while opening the pipe\n");
        fprint(stderr, "An error occurred while opening the pipe\n");
        return EXIT_FAILURE;
    }

    pid_t id = fork();

    if (id == 0)
    {
        close(fd[0]);                   // close the read end of the pipe for the child process
        // close(fd[1]);
    } else 
    {
        close(fd[1]);                   // close the write end of the pipe for the parent process
        printf("Message from child process: %s\n", read(fd[0]));
    }

    fclose(fd);
    return 0;
}