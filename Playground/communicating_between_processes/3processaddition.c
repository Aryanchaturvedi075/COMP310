#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

int fd1[2], fd2[2];

static inline int fork_fail(){
    close(fd1[0]), close(fd1[1]), close(fd2[0]), close(fd2[1]);
    return 2;
}


int main(){
    // in this version do the addition with 3 processes and 2 pipes
    uint8_t arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 2, 3, 4, 5, 4};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    
    // approach 2, create 2 pipes between processes
    if(pipe(fd1) == -1 || pipe(fd2)) exit(EXIT_FAILURE);

    uint16_t sum = 0, child1, child2;
    size_t start, end;

    pid_t id1 = fork();             // create first child process
    if (id1 == -1) return fork_fail();

    if (id1 == 0)
    {
        close(fd1[0]);
        start = 0, end = len / 3;
        for(size_t i = start; i < end; i++) sum += arr[i];
        write(fd1[1], &sum, sizeof(sum));
        close(fd1[1]);
        exit(EXIT_SUCCESS);
    }

    if (id1 != 0)
    {
        pid_t id2 = fork();         // create second child process
        if (id2 == -1) return fork_fail();

        if (id2 == 0)
        {
            close(fd2[0]);
            start = len / 3, end = (len / 3) * 2;
            for(size_t i = start; i < end; i++) sum += arr[i];
            write(fd2[1], &sum, sizeof(sum));
            close(fd2[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Wait for both child processes to complete
            wait(0), wait(0);
            
            // Read results from both pipes
            close(fd1[1]), close(fd2[1]);
            read(fd1[0], &child1, sizeof(child1));
            read(fd2[0], &child2, sizeof(child2));
            close(fd1[0]), close(fd2[0]);
            
            start = (len / 3) * 2, end = len;
            for(size_t i = start; i < end; i++) sum += arr[i];
            
            // Add all parts together and print result
            printf("The sum of the array is %d\n\n", sum + child1 + child2);
        }
    }

    return EXIT_SUCCESS;
}