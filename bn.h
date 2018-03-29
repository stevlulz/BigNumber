    /************************************************************************************
     *   * * *   *******                            *_* *-*      *-* §*           ^$$^  *
     *   *     $   ---       * * < *<    **          |   $        $  §  *         $ $   *
     *   *     $    *     *$             *  &        $   $        $  §   $       $  $   *
     *   *   $      *    $               *   $       |   $        $  §    $     $   $   *
     *   $$$$$      *    $               $    $      $   $        $  §     *-_-*    $   *
     *   *   $      *    $               &     *     |   $        $  §              $   *
     *   *     *    *    $      $$$$$$ * *      *    $   $        $  §              $   *
     *   *      *   *    $             ___       *   |   $        $  §              $   *
     *   *      *   *    *             * $       *   |   $        $  §              $   *
     *   *     *    *      $          $  $        *  |    \      /   §              $   *
     *   *    *     *        $ $ $ $ $  *_*        *_*     $^__^$   *-*           ^__.^ *
     *  *_* *    ********                                                               *
     ************************************************************************************
     */




/**
 * @authors {Kheroubi Elaid-Derouiwech-brahim,Seyyidahmed Lahmer}
 * @date 2018
 * @file bn.h
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

/*!
 ********************************************************************
 *                      @Helpers_Structures                         *
 ********************************************************************
 */
struct splittedNum{
    bn first;
    bn second;
    unsigned int shift;
};
typedef struct splittedNum splittedNum_;

struct dlt{
    bn div;
    unsigned long mod;
};
typedef struct dlt divLongTuple;


struct dit{
    unsigned int div;
    unsigned int mod;
};
typedef struct dit divIntTuple;

struct dbnt{
    bn div;
    bn mod;
};
typedef struct dbnt divBNTuple;






/*!
 * ***********************************************************************
 *                    @BigNumber_Zeroying_Interface                      *
 * ***********************************************************************
 */
void fillZ(bn*);





/*!
 * ***********************************************************************
 *                    @Printing_BigNumber_Interface                      *
 * ***********************************************************************
 */
void print_BN(bn);





/*!
 * ***********************************************************************
 *                    @BigNumber_Parsing_Interface                       *
 * ***********************************************************************
 */
void parseStrToBN(bn*,const char*);





/*!
 * ***********************************************************************
 *           Unsigned Integer Arithmetic Operation Interfaces            *
 * ***********************************************************************
 */
void uiadd(bn,bn,bn*);
void uisub(bn,bn,bn*);
void uimul(bn,bn,bn*);





/*!
 * ***********************************************************************
 *                  @Unsigned_Arithmetic_Operation_Interfaces            *
 * ***********************************************************************
 */
void uadd(bn,bn,bn*);
void usub(bn,bn,bn*);
void umul(bn,bn,bn*);
void udiv(bn,bn,divBNTuple*);





/**
 **************************************************************************
 *              @Signed_Arithmetic_Operation_Interfaces                   *
 **************************************************************************
 */
void add(bn,bn,bn*);
void sub(bn,bn,bn*);
void mul(bn,bn,bn*);
void div_(bn,bn,divBNTuple*);





/*!
 * ***********************************************************************
 *                   @Unsigned_Shifting_Interfaces                       *
 * ***********************************************************************
 */
void times10(bn*);
void div10(bn*);
void timeP10(bn*,int);
void divP10(bn*,int);





/*!
 * ***********************************************************************
 *                          @Comparaison_Interface                       *
 * ***********************************************************************
 */
int compareBN(bn,bn);





/*!
 * ***********************************************************************
 *                   @Type_Conversion_Interfaces                         *
 * ***********************************************************************
 */
int toInt(bn);
unsigned int toUInt(bn);
long toLong(bn);
unsigned long toULong(bn);





/*!
 * ***********************************************************************
 *                          @Temperare_Interfaces                        *
 * ***********************************************************************
 */
void cpy(bn num,bn*dest);
void simpleMul(bn a,bn b,bn*res);
void toBN(bn *num,const int num1);
void split(bn num);
void floatP(bn);

#endif //OP_BN_H
