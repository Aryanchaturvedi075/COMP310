#include <iostream>
#include <sys/wait.h>

using namespace std;

int main(){
    pid_t id = fork();
    cout << "Hello World from id: " << id << "\t";
    wait(0);

    if (id == 0) cout << "\t- This message came from the child process, it's own id is: " << getpid();
    else cout << "- This message came from the parent process and displays the process id of its child";

    cout << endl;
    return 0;
}
