#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef long double lld;

struct HelloStruct {
    int8_t a;
    float b;
    char c[16];
    char *d;
};

union Data {
    int8_t intVal;
    float floatVal;
    lld longDoubleVal;
};

int main(void){
    struct HelloStruct hs1;
    hs1.a = 10;
    strcpy(hs1.c, "Hello");

    struct HelloStruct hs2 = {
        .b = 2.5f,
        .d = "World"
    };

    printf("hs1.a = %d\n", hs1.a);
    printf("hs2.b = %f\n", hs2.b);
    printf("hs1.c = %s\n", hs1.c);
    printf("hs2.d = %s\n", hs2.d);

    union Data data = {
        .intVal = 32
    };
    printf("data.intVal = %d\n", data.intVal);

    data.longDoubleVal = 234.13243546576876543L;
    printf("data.longDoubleVal = %Lf\n", data.longDoubleVal);
    printf("data.intVal = %d\n", data.intVal);

    return 0;
}
