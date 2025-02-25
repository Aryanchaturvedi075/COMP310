#include <iostream>
#include <cstdlib>
#include <sys/wait.h>

int fd[2];

inline int fork_fail(){
    close(fd[0]), close(fd[1]);
    return 2;
}

int main(){
    // in this version do the addition with 3 processes and 1 pipe
    u_int8_t arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1, 2, 3, 4, 5, 4};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    // approach one, use 1 pipe to communicate between all 2 process
    if (pipe(fd) == -1) exit(EXIT_FAILURE);

    u_int16_t sum = 0, child, sub_child;
    size_t start, end;

    pid_t id1 = fork();
    if (id1 == -1) return fork_fail();

    if (id1 == 0)
    {
        pid_t id2 = fork();
        if (id2 == -1) return fork_fail();

        if (id2 == 0)
        {
            close(fd[0]);
            start = 0, end = len / 3;
            for(size_t i = start; i < end; i++) sum += arr[i];
            write(fd[1], &sum, sizeof(sum));
            close(fd[1]);
        }
        else
        {
            if (wait(nullptr) == -1) return 3;
            start = len / 3, end = (len / 3) * 2;
            for(size_t i = start; i < end; i++) sum += arr[i];
            read(fd[0], &sub_child, sizeof(sub_child));
            close(fd[0]);
            sum += sub_child;
            write(fd[1], &sum, sizeof(sum));
            close(fd[1]);
        }
    }
    else
    {
        if (wait(nullptr) == -1) return -1;
        close(fd[1]);
        start = (len / 3) * 2, end = len;
        for(size_t i = start; i < end; i++) sum += arr[i];
        read(fd[0], &child, sizeof(child));
        close(fd[0]);
        std::cout << "The sum of the array is " << sum + child << "\n\n";
    }


    return EXIT_SUCCESS;
}