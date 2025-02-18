#include <stdio.h>

int main(void){
    int a = 5;
    int *b = &a;

    printf("a = %d\n", a);
    *b = 10;
    printf("a = %d\n", a);
    return 0;
}