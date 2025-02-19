#include <sys/wait.h>
#include <bits/stdc++.h>

using namespace std;


int main(){
    pid_t id = fork();
    int8_t n = (id == 0) ? 1 : 6;
    if (id != 0) wait(0);
    
    for (int i = n; i < n+5; i++){
        cout << getpid() << " prints: " <<  i << endl;
        fflush(stdout);
    }

    return 0;
}