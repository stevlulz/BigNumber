//
// Created by stevlulz on 31/03/18.
//

#include "helper.h"
static void _one_mult (unsigned char *num, int size, int digit, unsigned char * result){
    int carry, value;
    unsigned char *nptr, *rptr;

    if (digit == 0)
        memset (result, 0, size);
    else
    {
        if (digit == 1)
            memcpy (result, num, size);
        else
        {
            /* Initialize */
            nptr = (unsigned char *) (num+size-1);
            rptr = (unsigned char *) (result+size-1);
            carry = 0;

            while (size-- > 0)
            {
                value = *nptr-- * digit + carry;
                *rptr-- = value % 10;
                carry = value / 10;
            }

            if (carry != 0) *rptr = carry;
        }
    }
}

bc_num bc_copy_num (bc_num num){
    num->n_refs++;
    return num;
}
bc_num bc_new_num (int length,int scale){
    bc_num temp;

    temp = (bc_num) malloc (sizeof(bc_struct));
    if (temp == NULL) perror("Cannot allocate memory");
    temp->n_sign = false;
    temp->n_len = length;
    temp->n_scale = scale;
    temp->n_refs = 1;
    temp->n_ptr = (char *) malloc (length+scale);
    if (temp->n_ptr == NULL)perror("Cannot allocate memory");
    temp->n_value = temp->n_ptr;
    memset (temp->n_ptr, 0, length+scale);
    return temp;
}
void bc_str2num (bc_num *num, const char *str,int scale){
    int digits, strscale;
    char *ptr, *nptr;
    char zero_int;


    /* Check for valid number and count digits. */
    ptr = str;
    digits = 0;
    strscale = 0;
    zero_int = false;
    if ( (*ptr == '+') || (*ptr == '-'))  ptr++;  /* Sign */
    while (*ptr == '0') ptr++;			/* Skip leading zeros. */
    while (isdigit((int)*ptr)) ptr++, digits++;	/* digits */
    if (*ptr == '.') ptr++;			/* decimal point */
    while (isdigit((int)*ptr)) ptr++, strscale++;	/* digits */

    /* Adjust numbers and allocate storage and initialize fields. */
    strscale = MIN(strscale, scale);
    if (digits == 0)
    {
        zero_int = true;
        digits = 1;
    }
    *num = bc_new_num (digits, strscale);

    /* Build the whole number. */
    ptr = str;
    if (*ptr == '-')
    {
        (*num)->n_sign = true;
        ptr++;
    }
    else
    {
        (*num)->n_sign = false;
        if (*ptr == '+') ptr++;
    }
    while (*ptr == '0') ptr++;			/* Skip leading zeros. */
    nptr = (*num)->n_value;
    if (zero_int)
    {
        *nptr++ = 0;
        digits = 0;
    }
    for (;digits > 0; digits--)
        *nptr++ = CH_VAL(*ptr++);


    /* Build the fractional part. */
    if (strscale > 0)
    {
        ptr++;  /* skip the decimal point! */
        for (;strscale > 0; strscale--)
            *nptr++ = CH_VAL(*ptr++);
    }
}
int bcdivide (bc_num n1,bc_num n2,bc_num* quot,int scale){
    bc_num qval;
    unsigned char *num1, *num2;
    unsigned char *ptr1, *ptr2, *n2ptr, *qptr;
    int  scale1, val;
    unsigned int  len1, len2, scale2, qdigits, extra, count;
    unsigned int  qdig, qguess, borrow, carry;
    unsigned char *mval;
    char zero;
    unsigned int  norm;

    /* Test for divide by zero. */


    /* Test for divide by 1.  If it is we must truncate. */
    if (n2->n_scale == 0)
    {
        if (n2->n_len == 1 && *n2->n_value == 1)
        {
            qval = bc_new_num (n1->n_len, scale);
            qval->n_sign = (n1->n_sign == n2->n_sign ? false : true);
            memset (&qval->n_value[n1->n_len],0,scale);
            memcpy (qval->n_value, n1->n_value,
                    n1->n_len + MIN(n1->n_scale,scale));
            //bc_free_num (quot);
            *quot = qval;
        }
    }

    /* Set up the divide.  Move the decimal point on n1 by n2's scale.
       Remember, zeros on the end of num2 are wasted effort for dividing. */
    scale2 = n2->n_scale;
    n2ptr = (unsigned char *) n2->n_value+n2->n_len+scale2-1;//LastDigit of N2
    while ((scale2 > 0) && (*n2ptr-- == 0)) scale2--;//LastDigit Of N2 skipping Zeros

    len1 = n1->n_len + scale2;
    scale1 = n1->n_scale - scale2;
    if (scale1 < scale)
        extra = scale - scale1;
    else
        extra = 0;
    num1 = (unsigned char *) malloc (n1->n_len+n1->n_scale+extra+2);
    if (num1 == NULL) perror("cannot allocate memory");
    memset (num1, 0, n1->n_len+n1->n_scale+extra+2);
    memcpy (num1+1, n1->n_value, n1->n_len+n1->n_scale);

    len2 = n2->n_len + scale2;
    num2 = (unsigned char *) malloc (len2+1);
    if (num2 == NULL) perror("cannot allocate memory");
    memcpy (num2, n2->n_value, len2);
    *(num2+len2) = 0;
    n2ptr = num2;
    while (*n2ptr == 0)
    {
        n2ptr++;
        len2--;
    }

    /* Calculate the number of quotient digits. */
    if (len2 > len1+scale)
    {
        qdigits = scale+1;
        zero = true;
    }
    else
    {
        zero = false;
        if (len2>len1)
            qdigits = scale+1;  	/* One for the zero integer part. */
        else
            qdigits = len1-len2+scale+1;
    }

    /* Allocate and zero the storage for the quotient. */
    qval = bc_new_num (qdigits-scale,scale);
    memset (qval->n_value, 0, qdigits);

    /* Allocate storage for the temporary storage mval. */
    mval = (unsigned char *) malloc (len2+1);
    if (mval == NULL) perror("cannot allocate memory");

    /* Now for the full divide algorithm. */
    if (!zero)
    {
        /* Normalize */
        norm =  10 / ((int)*n2ptr + 1);
        if (norm != 1)
        {
            _one_mult (num1, len1+scale1+extra+1, norm, num1);
            _one_mult (n2ptr, len2, norm, n2ptr);
        }

        /* Initialize divide loop. */
        qdig = 0;
        if (len2 > len1)
            qptr = (unsigned char *) qval->n_value+len2-len1;
        else
            qptr = (unsigned char *) qval->n_value;

        /* Loop */
        while (qdig <= len1+scale-len2)
        {
            /* Calculate the quotient digit guess. */
            if (*n2ptr == num1[qdig])
                qguess = 9;
            else
                qguess = (num1[qdig]*10 + num1[qdig+1]) / *n2ptr;

            /* Test qguess. */
            if (n2ptr[1]*qguess >
                (num1[qdig]*10 + num1[qdig+1] - *n2ptr*qguess)*10
                + num1[qdig+2])
            {
                qguess--;
                /* And again. */
                if (n2ptr[1]*qguess >
                    (num1[qdig]*10 + num1[qdig+1] - *n2ptr*qguess)*10
                    + num1[qdig+2])
                    qguess--;
            }

            /* Multiply and subtract. */
            borrow = 0;
            if (qguess != 0)
            {
                *mval = 0;
                _one_mult (n2ptr, len2, qguess, mval+1);
                ptr1 = (unsigned char *) num1+qdig+len2;
                ptr2 = (unsigned char *) mval+len2;
                for (count = 0; count < len2+1; count++)
                {
                    val = (int) *ptr1 - (int) *ptr2-- - borrow;
                    if (val < 0)
                    {
                        val += 10;
                        borrow = 1;
                    }
                    else
                        borrow = 0;
                    *ptr1-- = val;
                }
            }

            /* Test for negative result. */
            if (borrow == 1)
            {
                qguess--;
                ptr1 = (unsigned char *) num1+qdig+len2;
                ptr2 = (unsigned char *) n2ptr+len2-1;
                carry = 0;
                for (count = 0; count < len2; count++)
                {
                    val = (int) *ptr1 + (int) *ptr2-- + carry;
                    if (val > 9)
                    {
                        val -= 10;
                        carry = 1;
                    }
                    else
                        carry = 0;
                    *ptr1-- = val;
                }
                if (carry == 1) *ptr1 = (*ptr1 + 1) % 10;
            }

            /* We now know the quotient digit. */
            *qptr++ =  qguess;
            qdig++;
        }
    }

    /* Clean up and return the number. */
    qval->n_sign = ( n1->n_sign == n2->n_sign ? false : true );
    /*
    if (bc_is_zero (qval)) qval->n_sign = PLUS;
    _bc_rm_leading_zeros (qval);
    bc_free_num (quot);
     */
    *quot = qval;

    /* Clean up temporary storage. */
    free (mval);
    free (num1);
    free (num2);

    return 0;	/* Everything is OK. */
}
char *num2str (bc_num num){
    char *str, *sptr;
    char *nptr;
    int  index, signch;

    /* Allocate the string memory. */
    signch = ( num->n_sign == false ? 0 : 1 );  /* Number of sign chars. */
    if (num->n_scale > 0)
        str = (char *) malloc (num->n_len + num->n_scale + 2 + signch);
    else
        str = (char *) malloc (num->n_len + 1 + signch);
    if (str == NULL) perror("Cannot alocate it");

    /* The negative sign if needed. */
    sptr = str;
    if (signch) *sptr++ = '-';

    /* Load the whole number. */
    nptr = num->n_value;
    for (index=num->n_len; index>0; index--)
        *sptr++ = BCD_CHAR(*nptr++);

    /* Now the fraction. */
    if (num->n_scale > 0)
    {
        *sptr++ = '.';
        for (index=0; index<num->n_scale; index++)
            *sptr++ = BCD_CHAR(*nptr++);
    }

    /* Terminate the string and return it! */
    *sptr = '\0';
    return (str);
}