/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   memory.h
    @brief  The header file for memory.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef MEMORY_H
#define MEMORY_H

#include "structs.h"

struct environment_;
typedef struct environment_ environment;

void* allocate(size_t);
bool freeExpr(expression*);
bool freeExprNR(expression*);
bool freeLaz(tap_laz*);
bool freeStr(string*);
bool freeArr(array*);
bool freeObj(tap_obj*);
bool freeProp(property*);
bool freeFun(tap_fun*);
bool freeArg(argument*);
bool freeTypelist(typelist*);
bool freeCompTyp(type*);
bool freeTypedefs(typedefs*);
bool freeExprstack(exprstack*);
bool freePrimFun(tap_prim_fun*);
bool freeEnv(environment*);
bool freeStringlist(stringlist*);
bool freeErrorlist(errorlist*);

#endif
