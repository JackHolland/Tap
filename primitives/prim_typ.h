/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_typ.h
    @brief  The header file for prim_typ.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_TYP_H
#define PRIM_TYP_H

#include "structs.h"

void prim_tNew(expression*[], int, exprvals*, datatype*);
void prim_tInt(expression*[], int, exprvals*, datatype*);
void prim_tStr(expression*[], int, exprvals*, datatype*);
void prim_tTyp(expression*[], int, exprvals*, datatype*);

#endif
