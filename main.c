
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "bn.h"
long getMicrotime(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main() {
    bn a,b,c,d,e;
    divBNTuple z;
    /*
     *      1234556
     *     *
     *        34567
     * =============
     * = (1234*10^3 + 556) * (34*10^3 + 67 )
     * = (1234*34*10^6 + 1234*67*10^3 + 556*34*10^3 + 556*67)
     * =
     *
     *
     *
     */
    parseStrToBN(&a,"1");
    parseStrToBN(&b,"42");
    long start = getMicrotime();
    udiv(a,b,&z);
    long fin = getMicrotime();

   // printf("Start : %ld\nFin : %ld\n ",start,fin);

    printf("Time elapsed : %ld\n",fin-start);
    print_BN(z.div);
    print_BN(z.mod);
    return 0;
}