#include <stdio.h>
#include <unistd.h>
#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES ;

int decrease_count (int count ) {
    if ( available_resources < count ) {
        return -1;
    } else {
        available_resources -= count ;
        return 0;
    }
}

int increase_count (int count ) {
    available_resources += count ;
    return 0;
}


int main()
{
    if (fork() != 0)
    {
        if (fork() != 0)    // parent
        {
            printf(" hello1 \n");
        }
        else                // parent --> second
        {
            fork();         // parent --> second --> first
        }
        printf(" hello2 \n");
    }
    else
    {
        if (fork() != 0)    // parent --> first
        {
            printf(" hello3 \n");
        }
        else                // parent --> first --> first
        {
            fork();         // parent --> first --> first --> first
        }
        printf(" hello4 \n");
    }
    printf(" hello5 \n"); // <-- this one ! should print 6 times
}
