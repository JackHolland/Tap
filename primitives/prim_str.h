/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_str.h
    @brief  The header file for prim_str.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_STR_H
#define PRIM_STR_H

#include "../source/structs.h"

void prim_sSet(expression*[], int, exprvals*, datatype*);
void prim_sNewtype(expression*[], int, exprvals*, datatype*);
void prim_sPrint(expression*[], int, exprvals*, datatype*);
void prim_sCopy(expression*[], int, exprvals*, datatype*);
void prim_sSize(expression*[], int, exprvals*, datatype*);
void prim_sChar(expression*[], int, exprvals*, datatype*);
void prim_sSubstr(expression*[], int, exprvals*, datatype*);
void prim_sFind(expression*[], int, exprvals*, datatype*);
void prim_sFindlast(expression*[], int, exprvals*, datatype*);
void prim_sFindall(expression*[], int, exprvals*, datatype*);
void prim_sContains(expression*[], int, exprvals*, datatype*);
void prim_sConcat(expression*[], int, exprvals*, datatype*);
void prim_sReplace(expression*[], int, exprvals*, datatype*);
void prim_sReplaceat(expression*[], int, exprvals*, datatype*);
void prim_sInsert(expression*[], int, exprvals*, datatype*);
void prim_sInsertat(expression*[], int, exprvals*, datatype*);
void prim_sRemove(expression*[], int, exprvals*, datatype*);
void prim_sRemoveat(expression*[], int, exprvals*, datatype*);
void prim_sReverse(expression*[], int, exprvals*, datatype*);
void prim_sUpper(expression*[], int, exprvals*, datatype*);
void prim_sLower(expression*[], int, exprvals*, datatype*);
void prim_sSentence(expression*[], int, exprvals*, datatype*);
void prim_sTitle(expression*[], int, exprvals*, datatype*);
void prim_sLess(expression*[], int, exprvals*, datatype*);
void prim_sLequal(expression*[], int, exprvals*, datatype*);
void prim_sEqual(expression*[], int, exprvals*, datatype*);
void prim_sNequal(expression*[], int, exprvals*, datatype*);
void prim_sMequal(expression*[], int, exprvals*, datatype*);
void prim_sMore(expression*[], int, exprvals*, datatype*);
void prim_sFunction(expression*[], int, exprvals*, datatype*);
void prim_sInt(expression*[], int, exprvals*, datatype*);
void prim_sFlo(expression*[], int, exprvals*, datatype*);
void prim_sStr(expression*[], int, exprvals*, datatype*);
void prim_sArr(expression*[], int, exprvals*, datatype*);
void prim_sDat(expression*[], int, exprvals*, datatype*);
void prim_sTyp(expression*[], int, exprvals*, datatype*);

#endif
