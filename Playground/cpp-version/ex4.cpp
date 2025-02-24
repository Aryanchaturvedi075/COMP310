#include <iostream>
#include <system_error>  // probably won't use going forward
#include <unistd.h>

using namespace std;

int main() {
    int fd[2] = {0, 0};
    if (pipe(fd) == -1)
        throw system_error(errno, generic_category(), "Failed to create pipe");

    if (pid_t id = fork(); id == -1)
        throw system_error(errno, generic_category(), "Failed to fork");

    else if (id == 0) {  // Child
        close(fd[0]);
        int x;
        cout << getpid() << ": Enter a number: ";
        cin >> x;
        
        if (write(fd[1], &x, sizeof(x)) == -1){
            close(fd[1]);
            throw system_error(errno, generic_category(), "Failed to write");
        }
        close(fd[1]);
    } 
    else {  // Parent
        close(fd[1]);
        int y;
        
        if (read(fd[0], &y, sizeof(y)) == -1){
            close(fd[0]);
            throw system_error(errno, generic_category(), "Failed to read");
        }
        cout << getpid() << ": Message from child process: " << y << "\n\n";
        close(fd[0]);
    }

    return EXIT_SUCCESS;
}
