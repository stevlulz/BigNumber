#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <zconf.h>
#include "bn.h"
long getMicrotime(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}
long currentTimeMillis() {
    struct timeval time;
    gettimeofday(&time, NULL);

    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

int main() {
    bn a,b,c,d,e;
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
     */
    parseStrToBN(&a,"25252342324.2312");
    parseStrToBN(&b,"43242343.93");
    long start = getMicrotime();
    umul(a,b,&c);
    usub(a,b,&d);
    uadd(a,b,&e);
    long fin = getMicrotime();

   // printf("Start : %ld\nFin : %ld\n ",start,fin);

    printf("Time elapsed : %ld\n",fin-start);
   //
     print_BN(c);
    return 0;
}