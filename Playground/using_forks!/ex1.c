#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t id1 = fork();
    printf("id1: %d\t", id1);
    pid_t id2 = fork();
    printf("id2: %d\n", id2);
    wait(NULL);                             // the addition of this is what made it determinate
    printf("Parent pid: %d\t", getppid());
    printf("Current pid: %d\t", getpid());
    printf("Child pid: %d : %d\n", id1, id2);
    // fflush(stdout);         // avoiding print buffer -> but order is still indeterminate

    // pid_t id = fork();
    // if (id != 0) fork();
    // printf("Hello world\n");

    return 0;
}

// ppid that starts the first process
// pid[82096]
//      |
// pid[82109]----------------
//      |                   |
// pid[82113]           pid[82115]
//      |
// pid[82114]