#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int fd1[2], fd2[2]; // P => C, C => P

// no passing by reference in C, so we pass by pointers here. In C++ we'd do (int (&fd)[2])
static inline int write_fail(int (*fd)[2]){
    close((*fd)[1]);
    perror("An error occured while writing to the pipe");
    return 4;
}

static inline int read_fail(int (*fd)[2]){
    close((*fd)[0]);
    perror("An error occured while reading from the pipe");
    return 5;
}


// not using uintN_t anymore
int main(){
    srand(time(NULL));
    int8_t x;

    if(pipe(fd1) == -1 || pipe(fd2) == -1) 
    {
        close(fd1[0]), close(fd1[1]), close(fd2[0]), close(fd2[1]);
        perror("An error occured while creating the pipe");
        exit(EXIT_FAILURE);
    }

    pid_t id = fork();
    if (id == -1) return 2;

    if (id != 0)
    {
        close(fd1[0]), close(fd2[1]);
        x = rand() % 10 + 1;         // range from [1, 10]

        // parent writes to fd1
        if (write(fd1[1], &x, sizeof(x)) == -1) return write_fail(&fd1);
        close(fd1[1]);
        printf("Wrote %d to child\n", x);
        if (wait(NULL) == -1) return 3;

        // parent reads from fd2
        if (read(fd2[0], &x, sizeof(x)) == -1) return read_fail(&fd2);
        close(fd2[0]);
        printf("Result is %d\n\n", x);
    }
    else
    {
        close(fd1[1]), close(fd2[0]);

        // child reads from fd1
        if (read(fd1[0], &x, sizeof(x)) == -1) return read_fail(&fd1);
        close(fd1[0]);
        printf("Read %d\n", x);
        x *= 4;

        // child writes to fd2
        if (write(fd2[1], &x, sizeof(x)) == -1) return write_fail(&fd2);
        close(fd2[1]);
        printf("Wrote back %d\n", x);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
