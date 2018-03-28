/**
 * @authors {Kheroubi Elaid,Seyyidahmed Lahmer}
 * @date 2018
 *
 */

#ifndef OP_BN_H
#define OP_BN_H

#include <stdbool.h>
#include "config.h"

/**
 * @struct bnstruct
 * @brief This is general structure of our BigNumber .
 *          @property intNum[MaxInt+1]      : this will holds the integer part of BigNumber
 *          @property floatNum[MaxFloat+1]  : this will holds the floating part of BigNumber
 *          @property  intlen               : this will holds the number of digits of Integer part
 *          @property signNum               : this will holds the sign of our BigNumber
 */
struct bnstruct{
    char intNum[MaxInt+1];
    char floatNum[MaxFloat+1];
    unsigned short intlen;
    bool signNum;

};



typedef struct bnstruct bn;

void fillZ(bn*);

void parseStrToBN(bn*,const char*);


void print_BN(bn);
//Unsigned Int Arithemetic operations
void uiadd(bn,bn,bn*);
void uisub(bn,bn,bn*);
void uimul(bn,bn,bn*);


//Unsigned Arithemetic operations
void uadd(bn,bn,bn*);
void usub(bn,bn,bn*);
void umul(bn,bn,bn*);


//Signed Arithmetic operations
void add(bn,bn,bn*);
void sub(bn,bn,bn*);
void mul(bn,bn,bn*);

void times10(bn*);
void div10(bn*);
void timeP10(bn*,int);
void divP10(bn*,int);


//comparaison
int compareBN(bn,bn);


//Conversion

int toInt(bn);
unsigned int toUInt(bn);
long toLong(bn);
unsigned long toULong(bn);



/*
 * Temp functions
 */

void cpy(bn num,bn*dest);
void simpleMul(bn a,bn b,bn*res);
void toBN(bn *num,const int num1);
void split(bn num);
void floatP(bn);
#endif //OP_BN_H
