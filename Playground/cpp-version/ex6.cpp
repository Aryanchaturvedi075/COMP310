#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sys/wait.h>

using namespace std;

int fd1[2], fd2[2]; // P => C, C => P

// In C++ we'd do (int (&fd)[2])
inline int write_fail(int (&fd)[2]){
    close(fd[1]);
    cerr << "An error occured while writing to the pipe\n";
    return 4;
}

inline int read_fail(int (&fd)[2]){
    close(fd[0]);
    cerr << "An error occured while reading from the pipe\n";
    return 5;
}

int main(){
    cout.setf(ios::unitbuf);
    srand(time(NULL));
    // u_intN_t or intN_t not working here for some reason
    int x;

    if(pipe(fd1) == -1 || pipe(fd2) == -1) 
    {
        close(fd1[0]), close(fd1[1]), close(fd2[0]), close(fd2[1]);
        cerr << "An error occured while creating the pipe\n";
        exit(EXIT_FAILURE);
    }

    pid_t id = fork();
    if (id == -1) return 2;

    if (id != 0)
    {
        close(fd1[0]), close(fd2[1]);
        x = rand() % 10 + 1;         // range from [1, 10]

        // parent writes to fd1
        if (write(fd1[1], &x, sizeof(x)) == -1) return write_fail(fd1);
        close(fd1[1]);
        cout << "Wrote " << x << " to child\n";
        if (wait(nullptr) == -1) return 3;

        // parent reads from fd2
        if (read(fd2[0], &x, sizeof(x)) == -1) return read_fail(fd2);
        close(fd2[0]);
        cout << "Result is " << x << endl << endl;
    }
    else
    {
        close(fd1[1]), close(fd2[0]);

        // child reads from fd1
        if (read(fd1[0], &x, sizeof(x)) == -1) return read_fail(fd1);
        close(fd1[0]);
        cout << "Read " << x << endl;
        x *= 4;

        // child writes to fd2
        if (write(fd2[1], &x, sizeof(x)) == -1) return write_fail(fd2);
        close(fd2[1]);
        cout << "Wrote back " << x << endl;
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
