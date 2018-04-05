//
// Created by stevlulz on 28/03/18.
//

#ifndef OP_HELPER_H
#define OP_HELPER_H
#define CH_VAL(c)     (c - '0')
#define BCD_CHAR(d)   (d + '0')
#define MAX(a,b)      ((a)>(b)?(a):(b))
#define MIN(a,b)      ((a)>(b)?(b):(a))

#include <stdbool.h>
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <ctype.h>

typedef struct bc_struct *bc_num;

typedef struct bc_struct
{
    bool  n_sign;
    int   n_len;	/* The number of digits before the decimal point. */
    int   n_scale;	/* The number of digits after the decimal point. */
    int   n_refs;     /* The number of pointers to this number. */
    bc_num n_next;	/* Linked list for available list. */
    char *n_ptr;	/* The pointer to the actual storage.
			   If NULL, n_value points to the inside of
			   another number (bc_multiply...) and should
			   not be "freed." */
    char *n_value;	/* The number. Not zero char terminated.
			   May not point to the same place as n_ptr as
			   in the case of leading zeros generated. */
} bc_struct;

bc_num bc_copy_num (bc_num );
bc_num bc_new_num (int , int );
void bc_str2num (bc_num *num, const char *,int);
int bcdivide (bc_num ,bc_num,bc_num*,int);
char *num2str (bc_num num);
#endif //OP_HELPER_H
