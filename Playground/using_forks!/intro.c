#include <stdio.h>
// #include <unistd.h>
#include <sys/types.h>

int main(void){
    pid_t id = fork();                     // Create a child process, this function call returns an id
    printf("Hello World from id: %d!\t", id);

    if (id == 0) printf("\t- This message came from the child process, it's own id is: %d\n", getpid());
    else printf("- This message came from the parent process and displays the process id of its child\n");

    return 0;
}