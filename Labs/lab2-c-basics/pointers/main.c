#include <stdio.h>
#include <stdint.h>

int main(void){
    int8_t a = 5;
    int8_t *b = &a;

    printf("a = %d\n", a);
    *b = 10;
    printf("a = %d\n", a);
    return 0;
}