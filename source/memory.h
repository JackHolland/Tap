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
void freeExpr(expression*);
void freeExprNR(expression*);
void freeArray(array*);

#endif
