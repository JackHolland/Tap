/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   memory.h
    @brief  The header file for memory.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef MEMORY_H
#define MEMORY_H

#include "structs.h"

void* allocate(size_t);
bool freeExpr(expression*);
bool freeExprNR(expression*);
bool freeStr(string*);
bool freeArr(array*);
bool freeLaz(tap_laz*);

#endif
