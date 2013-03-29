/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_fun.h
    @brief  The header file for prim_fun.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_FUN_H
#define PRIM_FUN_H

#include "structs.h"

void prim_uArgs(expression*[], int, exprvals*, datatype*);
void prim_uMinargs(expression*[], int, exprvals*, datatype*);
void prim_uMaxargs(expression*[], int, exprvals*, datatype*);
void prim_uStr(expression*[], int, exprvals*, datatype*);
void prim_uFun(expression*[], int, exprvals*, datatype*);
void prim_uTyp(expression*[], int, exprvals*, datatype*);

#endif
