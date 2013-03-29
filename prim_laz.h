/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_laz.h
    @brief  The header file for prim_laz.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_LAZ_H
#define PRIM_LAZ_H

#include "structs.h"

void prim_lEval(expression*[], int, exprvals*, datatype*);
void prim_lFunction(expression*[], int, exprvals*, datatype*);
void prim_lAnd(expression*[], int, exprvals*, datatype*);
void prim_lOr(expression*[], int, exprvals*, datatype*);
void prim_lXor(expression*[], int, exprvals*, datatype*);
void prim_lIf(expression*[], int, exprvals*, datatype*);
void prim_lLaz(expression*[], int, exprvals*, datatype*);
void prim_lTyp(expression*[], int, exprvals*, datatype*);

#endif
