/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_arr.h
    @brief  The header file for prim_arr.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_ARR_H
#define PRIM_ARR_H

#include "structs.h"

void prim_aGet(expression*[], int, exprvals*, datatype*);
void prim_aSet(expression*[], int, exprvals*, datatype*);
void prim_aSize(expression*[], int, exprvals*, datatype*);
void prim_aResize(expression*[], int, exprvals*, datatype*);
void prim_aConcat(expression*[], int, exprvals*, datatype*);
/*void prim_aRemove(expression*[], int, exprvals*, datatype*);
void prim_aReverse(expression*[], int, exprvals*, datatype*);
void prim_aSort(expression*[], int, exprvals*, datatype*);
void prim_aMap(expression*[], int, exprvals*, datatype*);
void prim_aFilter(expression*[], int, exprvals*, datatype*);
void prim_aAccum(expression*[], int, exprvals*, datatype*);*/
void prim_aStr(expression*[], int, exprvals*, datatype*);
void prim_aArr(expression*[], int, exprvals*, datatype*);
void prim_aTyp(expression*[], int, exprvals*, datatype*);

#endif
