#include <iostream>
#include <sys/wait.h>

using namespace std;

int main(){
    pid_t id = fork();
    if (id != 0) fork();
    cout << "Hello World!\n";
    return 0;
}
