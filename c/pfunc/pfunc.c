#include <stdio.h>

void pfunc1(void) {
    printf("Blue ocean.\n");
    return;
}

void pfunc2(void) {
    printf("Gold ocean.\n");
    return;
}

void pfunc3(void) {
    printf("Sky-blue ocean.\n");
    return;
}

int main(void) {
    int i;

    void (*ptf[3])(void);
    ptf[0]=pfunc1;
    ptf[1]=pfunc2;
    ptf[2]=pfunc3;

    for (i=0; i<3; i++ ) {
        (*ptf[i])();
    }
    return 0;
}
