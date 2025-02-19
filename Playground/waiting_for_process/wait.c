#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    // call fork() once, and print numbers 1-10
    // the first 5 should be printed by the child pocess
    // the last five should be printed by the parents process
    pid_t id = fork();
    int8_t n = (id == 0) ? 1 : 6;
    int status;
    if (id != 0) wait(&status);
    
    for (int8_t i = n; i < n+5; i++){
        printf("%d prints: %d\n", getpid(), i);
        fflush(stdout);
    }




    return 0;
}