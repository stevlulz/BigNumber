
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bn.h"
long getMicrotime(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

#define LINES 3
#define COLUMNS 2

bn m[LINES][COLUMNS];
bn v[LINES];

bn part1,part2,part3,part4,part5,tmp,tmp2,a,b,s;

unsigned long max(unsigned long t[], unsigned int len){
    unsigned long max = t[0];
    for (int i = 1; i <len ; ++i) {
        if(t[i]>max)
            max = t[i];
    }
    return max;
}
unsigned long min(unsigned long t[], unsigned int len){
    unsigned long min = t[0];
    for (int i = 1; i <len ; ++i) {
        if(t[i]<min)
            min = t[i];
    }
    return min;
}
long double avg(unsigned long t[], unsigned int len){
    long double sum = 0;
    for (int i = 0; i <len ; ++i)
        sum +=t[i];
    return sum/len;
}

void init(unsigned int len){

    srand((unsigned int) time(NULL));

    char t[MaxInt];
    t[len] = 0;
    for (int l = 0; l <LINES ; ++l) {
        for (int i = 0; i <len ; ++i){int r = (rand()%10)+48;t[i] = (char)r;}//Generating a number PseudoRandomally
        parseStrToBN(&v[l],(const char*)t);
    }
    for (int j = 0; j < LINES; ++j) {
        for (int k = 0; k < COLUMNS ; ++k) {
            for (int i = 0; i <len ; ++i){int r = (rand()%10)+48;t[i] = (char)r;}//Generating a number PseudoRandomally
            parseStrToBN(&m[j][k],(const char*)t);
        }
    }

}
void calculate();
static long start();


int main() {
    FILE *fp;
    char*buffer[30];
    fp = fopen("res.txt", "a");
    for (int i = 1; i <=24 ; ++i) {
        long t[10];
        init(i);
        printf("Len : %d\n",i);

        for (int k = 0; k <10 ; ++k) {
            t[k] = start();
        }
        sprintf(buffer,"%d,%ld,%ld,%lf\n",i,max(t,10),min(t,10),(double) avg(t, 10));
        fprintf(fp, (const char *) buffer);
        print_BN(a);
        print_BN(b);
    }




    fclose(fp);

    return 0;
}
static long start(){
    parseStrToBN(&part1,"0");parseStrToBN(&part2,"0");
    parseStrToBN(&part3,"0");parseStrToBN(&part4,"0");
    parseStrToBN(&s,"6");

    long start = getMicrotime();
    calculate();
    long fin = getMicrotime();
    // printf("Time elapsed : %ld\n",fin-start);
    return  fin-start;

}
void calculate(){
    /*
    * estimator a = (s*part1-(part2*part3))/(s*part4-part5^2)
    */


    //Part1
    for (int k = 0; k < LINES ; ++k) {
        for (int i = 0; i < COLUMNS; ++i) {
            uadd(part1,m[k][i],&part1);//part1
            uadd(part2,m[k][i],&part2);//part2
        }
        umul(part1,v[k],&part1);//part1
    }




    //Part3
    for (int j = 0; j <LINES ; ++j) {
        uadd(part3,m[j][COLUMNS-1],&part3);
    }




    //Part4
    for (int k = 0; k < LINES ; ++k) {
        for (int i = 0; i < COLUMNS; ++i) {
            umul(m[k][i],m[k][i],&tmp);
            uadd(part4,tmp,&part4);
        }
    }




    umul(part4,part4,&part5);//Part5




    // a = (s*part1-(part2*part3))/(s*part4-part5);

    umul(part2,part3,&tmp);
    sub(part1,tmp,&tmp);

    sub(part4,part5,&tmp2);


    // a =
    bc_divide(tmp,tmp2,&a);


    // b = (part4-a*part3)/s

    umul(a,part3,&tmp);
    sub(part4,tmp,&tmp);
    // b =
    bc_divide(tmp,s,&b);

    //print_BN()


}