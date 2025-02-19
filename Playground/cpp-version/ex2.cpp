#include <sys/wait.h>
#include <bits/stdc++.h>

int main(){
    pid_t id1 = fork();
    printf("id1: %d\t", id1);
    pid_t id2 = fork();
    printf("id2: %d\n", id2);
    printf("Parent pid: %d\t", getppid());
    printf("Current pid: %d\t", getpid());
    printf("Child pid: %d : %d\n", id1, id2);
    wait(0);                            // This here is a little problematic
    return 0;
}
