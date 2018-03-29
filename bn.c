
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "bn.h"
#include "stdio.h"
#include <stdbool.h>
void fillZ(bn*num){
    num->intlen = 1;
    for (int k = 0; k <MaxInt ; ++k)num->intNum[k] = '0';
    num->intNum[MaxInt]=NULL;
    for (int l = 0; l <MaxFloat ; ++l)num->floatNum[l]='0';
    num->floatNum[MaxFloat]=NULL;
}
void print_BN(bn num){
    char sign = (char) ((num.signNum == 0) ? '+' : '-');
    if(num.intlen == 0){
        printf("%c 0.%s",sign,num.floatNum);
        return;
    }
    printf("%c ",sign);
    printf("%s",num.intNum);
    printf(".%s\n",num.floatNum);
}


/**
 * @functionName parseStrToBN
 * @param bn* : a pointer to bn that will hold the number parsed by this function
 * @param const char* : signed or unsiged BigNumber
 * @return void
 * @details -this function is considered as main interface for parsing a const char to BigNumber
 *            if signed is included in first position then function will take care of this signed
 *            then the next part will be parsed and stored in first @param
 *            if signed is not included then function will consider this number as a positive number
 *            const char must have just numbers + optional signed at beginning + separator for float number
 *
 */
void parseStrToBN(bn* num,const char*num_){
    fillZ(num);
    char*ptr = (char *) num_;
    int Intdigits=0;
    int Floatdigits=0;
    int sign=0;
    bool zero_int=false;
    char*iPtr= (char *) num_, *fPtr = NULL;

    if ( (*ptr == '+') || (*ptr == '-'))  sign++,ptr++,iPtr++,num->signNum = (num_[0] == '-')?true:false;/* Sign */
    else num->signNum = false;
    while (*ptr == '0') ptr++;			/* Skip leading zeros. */
    iPtr= ptr;
    while (isdigit((int)*ptr)) ptr++, Intdigits++;	/* digits ints */

    if (*ptr == '.'||*ptr == ',') fPtr =++ptr ;			/* decimal point */
    while (isdigit((int)*ptr)) ptr++, Floatdigits++;	/* digits floats! */
    if ((*ptr != '\0') || (Intdigits+Floatdigits == 0))
    {

        num->signNum = 0;
        return;
    }

    Floatdigits = MIN(Floatdigits,MaxFloat);
    if(Intdigits > MaxInt)
        perror("[!] Number [Integer Part] cannot be fitted into our dataStructure\n"),fflush(stderr),exit(1);


    num->intlen = (unsigned short) Intdigits;
    for (int i = 0; i <Intdigits ; ++i){
        num->intNum[MaxInt-Intdigits+i] = iPtr[i];
    }
    for (int j = 0; j <Floatdigits ; ++j)num->floatNum[j] = fPtr[j];
    if(num->intlen == 0)
        num->intlen = 1;
}

static unsigned short cal_Len(bn num_){
    unsigned short firstIndexOfLastNonZeroNum = MaxInt-1;
    for (unsigned short  i = MaxInt-2; i !=0xFFFF ; --i) {
        if(num_.intNum[i] !='0')
            firstIndexOfLastNonZeroNum = i;
    }
    return (unsigned short) (MaxInt-firstIndexOfLastNonZeroNum );
}
static unsigned short __get_f_len(bn num_){
    int i=MaxFloat-1;
    for (; i >=0 ; --i)
        if(num_.floatNum[i] != '0')
            break;
    return (unsigned short) (++i);

}
static unsigned short __float_part_to_bn(bn num1, bn*res){
    fillZ(res);
    res->intlen = __get_f_len(num1);
    if(res->intlen<=MaxInt){
        //123.233   .
        for (int i = MaxInt-1,j=res->intlen-1;j>=0; --i,--j) {
            res->intNum[i] = num1.floatNum[j];
        }

        return res->intlen;
    }
    perror("[!] Float Part cannot be hold by IntegerPart it causes An overFlow");
    exit(1);
}
static bool __is_simple_num(bn num_){
    return (num_.intlen <=SIMPLENUM);
}
static bool __not_simple_num(bn num_){
    return ! __is_simple_num(num_);
}
static unsigned int __to_u_int(bn num_){
    unsigned int ret = 0;
    unsigned int v=0;
    if(num_.intlen <= UINTMAX){
        for (int i = MaxInt-num_.intlen; i <=MaxInt-1; ++i) {
            v = (unsigned int) (num_.intNum[i] - '0');
            ret =ret*BASE +v;
        }
        return ret;
    }
    perror("[!] Conversion OverFlow");
    exit(1);
}
static unsigned long __to_u_long(bn num_){
    unsigned long ret = 0;
    unsigned long v=0;
    if(num_.intlen <= ULONGMAX){
        for (int i = MaxInt-num_.intlen; i <=MaxInt-1; ++i) {
            v = (unsigned int) (num_.intNum[i] - '0');
            ret =ret*BASE +v;
        }
        return ret;
    }
    perror("[!] Conversion OverFlow");
    exit(1);
}
static int __to_int(bn num_){
    int ret = 0;
    int v=0;
    if(num_.intlen <= INTMAX){
        for (int i = MaxInt-num_.intlen; i <=MaxInt-1; ++i) {
            v = (unsigned int) (num_.intNum[i] - '0');
            ret =ret*BASE +v;
        }
        return (num_.signNum == 0)?ret:-ret;
    }
    perror("[!] Conversion OverFlow");
    exit(1);
}
static long __to_long(bn num_){
    long ret = 0;
    long v=0;
    if(num_.intlen <= LONGMAX){
        for (int i = MaxInt-num_.intlen; i <=MaxInt-1; ++i) {
            v = (unsigned int) (num_.intNum[i] - '0');
            ret =ret*BASE +v;
        }
        return (num_.signNum == 0)?ret:-ret;
    }
    perror("[!] Conversion OverFlow");
    exit(1);
}
static void __uint_to_BG(const unsigned int num_,bn* dest){
    char t[MaxInt];
    sprintf(t,"%u",num_);
    parseStrToBN(dest,(const char*)t);
}
static void __int_to_BG(const int num_,bn* dest){
    char t[MaxInt];
    sprintf(t,"%d",num_);
    parseStrToBN(dest,(const char*)t);
}
static void __long_to_BG( const long num_,bn* dest){
    char t[MaxInt];
    sprintf(t,"%ld",num_);
    parseStrToBN(dest,(const char*)t);
}
static void __ulong_to_BG(const long num_,bn* dest){
    char t[MaxInt];
    sprintf(t,"%lu",num_);
    parseStrToBN(dest,(const char*)t);
}

static bool isZer(bn num){
    if(num.intlen == 1){
        for (int i = 0; i <MaxFloat ; ++i) {
            if(num.floatNum[i] !='0')return false;
        }
        return num.intNum[MaxInt-1] == '0';
    }
    return false;
}
static bool __isLong(bn num2_){
    return num2_.intlen <=LONGMAX;
}
static void __cpy_BN(bn num_,bn* dest){
    dest->floatNum[MaxFloat] = '\x00';
    dest->intNum[MaxInt]='\x00';
    dest->signNum = num_.signNum;
    dest->intlen = (unsigned short) num_.intNum;
    for (int i = 0; i <=MaxInt ; ++i)
        dest->intNum[i] = num_.intNum[i];
    for (int j = 0; j <=MaxFloat ; ++j) {
        dest->floatNum[j] = num_.floatNum[j];
    }
}

static void __split_BN(bn num_,struct splittedNum* res){
    fillZ(&res->first);
    fillZ(&res->second);
    unsigned int d = (unsigned int) (num_.intlen / 2);
    res->shift = d;
    res->second.signNum=false;
    res->first.signNum=false;
    res->first.intlen = (unsigned short) d;
    res->second.intlen = (unsigned short) (num_.intlen - d);
    for (int i = MaxInt-1; i >=MaxInt-d ; --i) {
            res->first.intNum[i] = num_.intNum[i];
    }
    for (int i = MaxInt-d-1,j=MaxInt-1; i >=0 ;--j,--i) {
        res->second.intNum[j] = num_.intNum[i];
    }
}

static bool __u_equal(bn num1,bn num2){
    if(num1.intlen == num2.intlen){
        for (int i = 0; i<MaxInt; ++i){
            if(num1.intNum[i] != num2.intNum[i])return false;
        }
        for (int i = 0; i<MaxFloat; ++i){
            if(num1.floatNum[i] != num2.floatNum[i])return false;
        }
        return true;
    }
    return false;
}
static bool __u_greater(bn num1,bn num2){
    if(num1.intlen > num2.intlen) return true;
    else if(num2.intlen > num1.intlen) return false;
    else{
        for (int i = 0; i<MaxInt; ++i){
            if(num1.intNum[i] > num2.intNum[i])return true;
            if(num2.intNum[i] > num1.intNum[i])return false;
        }
        for (int i = 0; i<MaxFloat; ++i){
            if(num1.floatNum[i] > num2.floatNum[i])return true;
            if(num2.floatNum[i] > num1.floatNum[i])return false;
        }
        return false;
    }
}
static bool __u_lower(bn num1,bn num2){
    return !(__u_greater(num1,num2)||__u_equal(num1,num2));}
static bool __is_equal(bn num1,bn num2){
    if(num1.signNum == num2.signNum)
        return __u_equal(num1,num2);
    else
        return false;
}
static bool __is_greater(bn num1,bn num2){
    if(num1.signNum == num2.signNum){
        if(num1.signNum == 0)
            return __u_greater(num1,num2);
        else
            return __u_greater(num2,num1);
    }
    else
        return (num1.signNum == 0)?true:false;
}
static bool __is_lower(bn num1,bn num2){
    return !(__is_equal(num1,num2)||__is_greater(num1,num2));
}

static void __init_BN_POS_INT(bn* num1,const char* intNum){
    unsigned short len = (unsigned short) strlen(intNum);
    if(len <= MaxInt){
        num1->intlen = len;
        for (unsigned short i = (unsigned short) 0; i < len ; ++i){
            num1->intNum[len-i-1] = intNum[i];
        }
        return;
    }
    perror("[!] Number [Integer Part] cannot be fitted into our dataStructure\n");
}
static void __init_BN_FLOAT(bn*num,const char* floatNum){
    unsigned len = (unsigned int) strlen(floatNum);
    if(len <=MaxFloat){
        unsigned int i = 0;
        for (; i <len ; ++i){
            num->floatNum[i] = floatNum[i];
        }
        for( ; i<MaxFloat;++i)
            num->floatNum[i]='0';

        return;
    }
    perror("[!] Number [Float Part] cannot be fitted into our dataStructure\n");
}
static void __setSignBn(bn* num,bool sign){
    num->signNum = sign;
}

static void __simple_int_div(bn num1_,bn num2_,divIntTuple* res){
    if(num1_.intlen <=MUMALLOW && num2_.intlen <= MUMALLOW) {
        unsigned int num1 = toUInt(num1_);
        unsigned int num2 = toUInt(num2_);
        if(num2 !=0){
            res->div = num1/num2;
            res->mod = num1%num2;
            return;
        }
        else{
            perror("[!] Simple division can't be done cause of ZeroDivision\n");
        }
    }
    perror("[!] Simple division can't be done cause of args's length\n");
    exit(1);
}//TODO SEE =================!
static void __simple_int_mul(bn num1_,bn num2_,bn*res){
    if(num1_.intlen <=MUMALLOW && num2_.intlen <= MUMALLOW){
        unsigned int num1 = toUInt(num1_);
        unsigned int num2 = toUInt(num2_);
        int ress = num1*num2;
        __int_to_BG(ress,res);
        return;
    }
    perror("[!] Simple multiplication can't be done cause of args's length\n");
    exit(1);
}
static void __mul_by_10(bn* num1){
    if(num1->intlen == MaxInt)perror("[!] Multiplication10 cannot be done cause of overFlow \n"),exit(1);
    if(num1->intNum[0] !='0'){
        perror("[!] Mul by ten cannot be done cause of overFlow");
        exit(0);
    }
    int i;
    for (int i = 0; i <MaxInt-1 ; ++i) {
        num1->intNum[i] = num1->intNum[i+1];
    }
    num1->intNum[MaxInt-1] = num1->floatNum[0];
    for (i = 0; i <MaxFloat-1 ; ++i) {
        num1->floatNum[i] = num1->floatNum[i+1];
    }
    num1->floatNum[MaxFloat-1] = '0';
    num1->intlen++;
}
static void __div_by_10(bn* num_){
    if(isZer(*num_))return;

    char a = num_->intNum[MaxInt-1];
    if(num_->intlen>=1){
        for (int i = MaxInt-1; i >0 ; --i){
            num_->intNum[i] = num_->intNum[i-1];
        }
        num_->intlen--;
    }
    num_->intNum[0]='0';
    for (int j = MaxFloat-1; j >0 ; --j) {
        num_->floatNum[j] = num_->floatNum[j-1];
    }
    num_->floatNum[0] = a;
}
static void __mul_by_pow10(bn* num_,int sh){
    if(sh == 0)return;
    if(sh>0)
        while (sh--)
            __mul_by_10(num_);
    else
        while (sh++ !=0)
            __div_by_10(num_);

}
static void __div_by_pow10(bn* num_,int sh){
    __mul_by_pow10(num_,-sh);
}
static void __udiv_BN_Long(bn num_,unsigned long div,divLongTuple*res){
    unsigned long rem = 0;
    char result[DIV];
    for(int indx=MaxInt-num_.intlen, len = MaxInt; indx<len; ++indx)
    {
        rem = (rem * 10) + (num_.intNum[indx] - '0');
        result[indx] = (char) (rem / div + '0');
        rem %= div;
    }
    res->mod =rem;
    parseStrToBN(&res->div,result);
    print_BN(res->div);
}

static void __unsigned_addition(bn num1_,bn num2_,bn*res){
    fillZ(res);
    res->signNum = false;
    int len =   MAX(num1_.intlen,num2_.intlen);
    res->intlen = (unsigned short) len;
    int carry = 0;
    int r;
    for (int i = MaxFloat-1; i >=0; --i){
        r = num1_.floatNum[i]+num2_.floatNum[i]-'0'*2+carry;
        res->floatNum[i] += r%10;
        carry = r/10;
    }
    int j = MaxInt-1;
    for (; j >=0 ; --j){
        len--;
        r = num1_.intNum[j]+num2_.intNum[j]-'0'*2+carry;
        res->intNum[j] += r%10;
        carry = r/10;
        if(len == 0 || len == -1)
            if (carry == 0)break;

    }
    if(len == -1)
        res->intlen++;
    if(carry>0){
        if(j==-1)
            perror("[!] Addition Cannot be performed cause of overFlow"),fflush(stderr),exit(1);
    }
}
static void __unsigned_int_addition(bn num1_,bn num2_,bn*res){
    __unsigned_addition(num1_,num2_,res);
    for (int i = 0; i <MaxFloat ; ++i) {
        res->floatNum[i] = '0';
    }
}

static void __unsigned_substration(bn num1_,bn num2_,bn*res){
    if(__u_greater(num2_,num1_)){
        __unsigned_substration(num2_,num1_,res);

    }
    else{

        fillZ(res);
        int r,borrow=0;
        int i;
        for (i = MaxFloat-1; i >=0 ; --i) {
            r = (num1_.floatNum[i]-num2_.floatNum[i]-borrow);
            if(r<0){
                r +=10;
                borrow = 1;
            }else
                borrow=0;

            res->floatNum[i] +=r;
        }
        for (i = MaxInt-1; i >=0 ; --i) {
            r = (num1_.intNum[i]-num2_.intNum[i]-borrow);
            if(r<0){
                r +=10;
                borrow = 1;
            }else
                borrow=0;

            res->intNum[i] +=r;
        }
    }
}
static void __unsigned_int_substration(bn num1_,bn num2_,bn*res){
    __unsigned_substration(num1_,num2_,res);
    for (int i = 0; i <MaxFloat ; ++i) {
        res->floatNum[i] = '0';
    }
}

static void __unsigned_int_multiplication(bn num1_, bn num2_, bn*res ){
    bn tmp1,tmp2,tmp3,tmp4;
    struct splittedNum tmp5,tmp6;
    if(__is_simple_num(num1_) && __is_simple_num(num2_)){
        __simple_int_mul(num1_,num2_,res);
    }
    else if(__is_simple_num(num1_) && __not_simple_num(num2_)){
        /*
         * As * Bc = As * (Bh *10^n + Bl) = As*Bh*10^n + As*Bl
        */

        __split_BN(num2_,&tmp5);
        __unsigned_int_multiplication(num1_,tmp5.first,&tmp3);
        __unsigned_int_multiplication(num1_,tmp5.second,&tmp4);
        __mul_by_pow10(&tmp4,tmp5.shift);
        __unsigned_addition(tmp4,tmp3,res);//to unsigned int addition

    }
    else if(__not_simple_num(num1_)&&__is_simple_num(num2_)){
        __unsigned_int_multiplication(num2_,num1_,res);
    }
    else {
        /*
         * Ac * Bc = (Ah * 10^n1 + Al) + (Bh *10^n2 + Bl)
         *         = Ah*Bh * 10^(n1+n2) + Ah*Bl*10^n1 + Al*Bh*10^n2 + Al*Bl
         */
        __split_BN(num1_,&tmp5);
        __split_BN(num2_,&tmp6);

        //Ah*Bh * 10^(n1+n2) = tmp1
        __unsigned_int_multiplication(tmp5.second,tmp6.second,&tmp1);
        __mul_by_pow10(&tmp1,tmp5.shift+tmp6.shift);

        //Ah*Bl*10^n1 = tmp2;
        __unsigned_int_multiplication(tmp5.second,tmp6.first,&tmp2);
        __mul_by_pow10(&tmp2,tmp5.shift);

        //Al*Bh*10^n2 = tmp3
        __unsigned_int_multiplication(tmp6.second,tmp5.first,&tmp3);
        __mul_by_pow10(&tmp3,tmp6.shift);

        //Al*Bl = tmp4
        __unsigned_int_multiplication(tmp5.first,tmp6.first,&tmp4);

         bn res1,res2;
        __unsigned_addition(tmp1,tmp2,&res1);//TODO
        __unsigned_addition(tmp3,tmp4,&res2);
        __unsigned_addition(res1,res2,res);
    }

}
static void __unsigned_multiplication(bn num1_,bn num2_,bn*res ){
    /*
     *  1.3*2.2 = 1*2 + 1*0.2+ 2*0.3 + 0.3*0.2
     *
     */
     bn tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;

     // int*int = tmp1
    __unsigned_int_multiplication(num1_,num2_,&tmp1);

    unsigned short shift1 = __float_part_to_bn(num2_,&tmp2);
    unsigned short shift2 = __float_part_to_bn(num1_,&tmp3);

    // int * toInt(floatPart) * 10^n1 = tmp3
    __unsigned_int_multiplication(num1_,tmp2,&tmp4);
    divP10(&tmp4,shift1);


    // toInt(floatPart)*int *10^n2
    __unsigned_int_multiplication(num2_,tmp3,&tmp5);
    divP10(&tmp5,shift2);

    //toInt(floatPart)*toInt(floatPart)*10(n2+n1)
    fillZ(&tmp6);
    __unsigned_int_multiplication(tmp2,tmp3,&tmp6);
    divP10(&tmp6,shift1+shift2);



    bn res1,res2;
    __unsigned_addition(tmp1,tmp4,&res1);

    __unsigned_addition(tmp5,tmp6,&res2);
    __unsigned_addition(res1,res2,res);
}

static void __unsigned_int_division(bn num1_,bn num2_,divBNTuple* res){//TODO===================
    fillZ(&res->mod);
    fillZ(&res->div);
    if(isZer(num2_))perror("[!] Division by zero cannot be done\n"),exit(1);
    if(__u_greater(num2_,num1_)){
        cpy(num2_,&res->div);
        return;
    }
    if(__is_simple_num(num1_)&&__is_simple_num(num2_)&&!isZer(num1_)){
        divIntTuple r;
        __simple_int_div(num1_, num2_, &r);
        __uint_to_BG(r.mod, &res->mod);
        __uint_to_BG(r.div, &res->div);
    }
    else if(__isLong(num2_)){
        divLongTuple s;
        __udiv_BN_Long(num1_,toULong(num2_),&s);
      //  printf("Mod : %ld\n",s.mod);
        //printf("Div : %s\n",s.div);
    }
    else if(isZer(num1_)){
        __cpy_BN(num1_,&res->mod);
        return;
    }
    else{
        perror("[!] Ops sorry division can't be done\n");
        exit(9);
    }


}
static void __unsigned_division(bn num1_,bn num2_,divBNTuple* res){
    __unsigned_int_division(num1_,num2_,res);
    res->mod.intlen = cal_Len(res->mod);
    res->div.intlen = cal_Len(res->div);
}




static void __addition(bn num1_, bn num2_, bn*res){
    /*
     * Addition must be performed
     *
     */
    if(num1_.signNum == num2_.signNum){
        __unsigned_addition(num1_,num2_,res);
        __setSignBn(res,num1_.signNum);
        res->intlen = cal_Len(*res);
        return;
    }

    /*
     * Substraction must be done
     *
     */

    if(__u_equal(num1_,num2_)){
        fillZ(res);
    }
    else if (__u_greater(num1_,num2_)){
        __unsigned_substration(num1_,num2_,res);
        __setSignBn(res,num1_.signNum);
    }else
    {
        __unsigned_substration(num2_,num1_,res);
        __setSignBn(res,num2_.signNum);

    }
    res->intlen = cal_Len(*res);
    return;

}
static void __substration(bn num1_,bn num2_,bn*res){
    //Addition
   if(num1_.signNum != num2_.signNum){
       __unsigned_addition(num1_,num2_,res);
       __setSignBn(res,num1_.signNum);
       res->intlen = cal_Len(*res);
       return;

   }
   //substraction

    if(__u_equal(num1_,num2_)){
        fillZ(res);
    }
    else if (__u_greater(num1_,num2_)){
        __unsigned_substration(num1_,num2_,res);
        __setSignBn(res,num1_.signNum);
    }else
    {
        __unsigned_substration(num2_,num1_,res);
        __setSignBn(res,!num2_.signNum);

    }

    res->intlen = cal_Len(*res);
    return;
}
static void __multiplication(bn num1_,bn num2_,bn*res){
    __unsigned_multiplication(num1_,num2_,res);
    res->signNum = (num2_.signNum != num1_.signNum);
}
static void __division(bn num1_,bn num2_,divBNTuple* res){
    if(num1_.signNum == num2_.signNum){
        __unsigned_division(num1_,num2_,res);
        res->div.signNum =false;
        res->mod.signNum = false;

        return;
    }
    else{


    }

}




void times10(bn*num_){
        __mul_by_10(num_);
}
void div10(bn*num_){
    __div_by_10(num_);
}
void timeP10(bn*num_,int sh){
    __mul_by_pow10(num_,sh);
}
void divP10(bn*num_,int sh){
    __div_by_pow10(num_,sh);
}


void uiadd(bn num1_,bn num2_,bn* res){
    __unsigned_int_addition(num1_,num2_,res);
}
void uisub(bn num1_,bn num2_,bn* res){
    __unsigned_int_substration(num1_,num2_,res);
}
void uimul(bn num1_,bn num2_,bn* res){
    __unsigned_int_multiplication(num1_,num2_,res);
}


/***
 * @interface : uadd
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details This function is considered as main interface for
 *          performing @unsigned addition on BigNumbers.
 *
 */

void uadd(bn _num1,bn _num2,bn* res){
    __unsigned_addition(_num1,_num2,res);
}

/**
 * @interface usub
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details   this function is considered as main interface for
 *            performing @unsigned substraction on BigNumbers
 */
void usub(bn _num1,bn _num2,bn* res){
    __unsigned_substration(_num1,_num2,res);
}

/**
 * @interface umul
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details  this function is considered as the main interface
 *           for performing @unsigned multiplication on BigNumbers
 */
void umul(bn num1_,bn num2_,bn*res){
    __unsigned_multiplication(num1_,num2_,res);
}
void udiv(bn num1_,bn num2_,divBNTuple* res){

    __unsigned_division(num1_,num2_,res);

}//TODO==================================!


/**
 * @interface add
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details  this function is considered as the main interface
 *           for performing @signed addition on BigNumbers
 */
void add(bn num1_,bn num2_,bn*res){
    __addition(num1_,num2_,res);
}

/**
 * @interface sub
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details  this function is considered as the main interface
 *           for performing @signed substraction on BigNumbers
 */
void sub(bn num1_,bn num2_,bn* res){
    __substration(num1_,num2_,res);
}

/**
 * @interface mul
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @param bn* : a pointer to bn that result will be stored in
 * @return void
 * @details  this function is considered as the main interface
 *           for performing @signed multiplication on BigNumbers
 */
void mul(bn num1_,bn num2_,bn* res){
    __multiplication(num1_,num2_,res);
}

void div_(bn num1_,bn num2_,divBNTuple* res){}//TODO==============================!

/**
 * @interface compare
 * @param bn  : first number of type bn
 * @param bn  : second number of type bn
 * @return int
 * @details  this function is considered as the main interface
 *           for performing @signed comparaison between two BigNumbers
 *           if @return = 0 then two numbers are equal
 *           if @return = 1 then first number is greater than second
 *           if @return = -1 then second number is greater than first
 */
int compareBN(bn num1_,bn num2_){

    if(__is_greater(num1_,num2_)) return 1;
    if(__is_equal(num1_,num2_)) return 0;
    if(__is_lower(num1_,num2_)) return -1;
}

/**
 * @interface toInt
 * @param bn : first number of bn type
 * @return int
 * @details -this function is considered as main interface for performing
 *            conversion of BigNumbers to a @signed_integer
 *          -if the int type cannot hold the result then function will print an error
 *             and exit with 1 status
 */
int toInt(bn num_){
    return  __to_int(num_);
}

/**
 * @interface toUInt
 * @param bn : first number of bn type
 * @return unsigned int
 * @details -this function is considered as main interface for performing
 *            conversion of BigNumbers to a @unsigned_integer
 *          -if the int type cannot hold the result then function will print an error
 *             and exit with 1 status
 */
unsigned int toUInt(bn num_){
    return __to_u_int(num_);
}

/**
 * @interface toLong
 * @param bn : first number of bn type
 * @return long
 * @details -this function is considered as main interface for performing
 *            conversion of BigNumbers to a @signed_Longinteger
 *          -if the int type cannot hold the result then function will print an error
 *             and exit with 1 status
 */
long toLong(bn num_){
    return __to_long(num_);
}

/**
 * @interface toULONG
 * @param bn : first number of bn type
 * @return unsigned long
 * @details -this function is considered as main interface for performing
 *            conversion of BigNumbers to a @unsigned_Longinteger
 *          -if the int type cannot hold the result then function will print an error
 *             and exit with 1 status
 */
unsigned long toULong(bn num_){
    return __to_u_long(num_);
}




/*
 * Temp functions
 */



void cpy(bn num,bn*dest){
    __cpy_BN(num,dest);
}

void simpleMul(bn a,bn b,bn*res){
    __simple_int_mul(a,b,res);
}

void toBN(bn *num,const int num1){
    __int_to_BG(num1,num);
}
void split(bn num){
    struct splittedNum res;
    __split_BN(num,&res);
    print_BN(res.first);
    printf("Len1 : %u\n",res.first.intlen);
    print_BN(res.second);
    printf("Len2 : %u\n",res.second.intlen);
    printf("%u\n",res.shift);
}
void floatP(bn num){
    bn n;
    __float_part_to_bn(num,&n);
    print_BN(n);
}