/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_nil.h
    @brief  The header file for prim_nil.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_NIL_H
#define PRIM_NIL_H

#include "structs.h"

void prim_nInt(expression*[], int, exprvals*, datatype*);
void prim_nStr(expression*[], int, exprvals*, datatype*);
void prim_nTyp(expression*[], int, exprvals*, datatype*);

#endif
