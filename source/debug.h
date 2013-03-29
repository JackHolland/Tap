/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   debug.h
    @brief  The header file for debug.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef DEBUG_H
#define DEBUG_H

#include "structs.h"
#include "dep_structs.h"

void printExprTree(expression*);
void printExprList(expression*);
void printExprListFlags(expression*);
void printExprMemory(expression*);
void printEnvironment(environment*);

#endif
