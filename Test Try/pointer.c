#include <stdio.h>

int main(){
    int m = 28;
    int *p = m;
    p = m;

    printf("%d", *p);

    return 0;
}