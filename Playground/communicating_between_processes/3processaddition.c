#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

int fd1[2], fd2[2];


// how function inlining looks in C
static inline int fork_fail(){
    close(fd1[0]), close(fd1[1]), close(fd2[0]), close(fd2[1]);
    perror("An error occured while forking\n");
    return 2;
}

// no passing by reference in C, so we pass by pointers here. In C++ we'd do (int (&fd)[2])
static inline int write_fail(int (*fd)[2]){
    close((*fd)[1]);
    perror("An error occured while writing to the pipe\n");
    return 3;
}

static inline int read_fail(int (*fd)[2]){
    close((*fd)[0]);
    perror("An error occured while reading from the pipe\n");
    return 4;
}

static inline int wait_fail(){
    perror("An error occured while waiting for a child process");
    return 5;
}

int main(){
    // in this version do the addition with 3 processes and 2 pipes
    uint8_t arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 2, 3, 4, 5, 4};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    
    // approach 2, create 2 pipes between processes -> better avoids race condition, and only the main process forks.
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
        if (write(fd1[1], &sum, sizeof(sum)) == -1) return write_fail(&fd1);
        close(fd1[1]);
        exit(EXIT_SUCCESS);         // exit status for child processes
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
            if (write(fd2[1], &sum, sizeof(sum)) == -1) return write_fail(&fd2);
            close(fd2[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Wait for both child processes to complete
            if (wait(0) == -1) return wait_fail();
            if (wait(0) == -1) return wait_fail();
            
            // Read results from both pipes
            close(fd1[1]), close(fd2[1]);
            if (read(fd1[0], &child1, sizeof(child1)) == -1) return read_fail(&fd1);
            if (read(fd2[0], &child2, sizeof(child2)) == -1) return read_fail(&fd2);
            close(fd1[0]), close(fd2[0]);
            
            start = (len / 3) * 2, end = len;
            for(size_t i = start; i < end; i++) sum += arr[i];
            
            // Add all parts together and print result
            printf("The sum of the array is %d\n\n", sum + child1 + child2);
        }
    }

    return EXIT_SUCCESS;
}