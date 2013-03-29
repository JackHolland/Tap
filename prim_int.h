/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_int.h
    @brief  The header file for prim_int.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_INT_H
#define PRIM_INT_H

#include "structs.h"

void prim_iError(expression*[], int, exprvals*, datatype*);
void prim_iAdd(expression*[], int, exprvals*, datatype*);
void prim_iSub(expression*[], int, exprvals*, datatype*);
void prim_iMul(expression*[], int, exprvals*, datatype*);
void prim_iDiv(expression*[], int, exprvals*, datatype*);
void prim_iMod(expression*[], int, exprvals*, datatype*);
void prim_iSqrt(expression*[], int, exprvals*, datatype*);
void prim_iPow(expression*[], int, exprvals*, datatype*);
void prim_iLog(expression*[], int, exprvals*, datatype*);
void prim_iAbs(expression*[], int, exprvals*, datatype*);
void prim_iMax(expression*[], int, exprvals*, datatype*);
void prim_iMin(expression*[], int, exprvals*, datatype*);
void prim_iRound(expression*[], int, exprvals*, datatype*);
void prim_iCeil(expression*[], int, exprvals*, datatype*);
void prim_iFloor(expression*[], int, exprvals*, datatype*);
void prim_iSin(expression*[], int, exprvals*, datatype*);
void prim_iCos(expression*[], int, exprvals*, datatype*);
void prim_iTan(expression*[], int, exprvals*, datatype*);
void prim_iAsin(expression*[], int, exprvals*, datatype*);
void prim_iAcos(expression*[], int, exprvals*, datatype*);
void prim_iAtan(expression*[], int, exprvals*, datatype*);
void prim_iAtan2(expression*[], int, exprvals*, datatype*);
void prim_iSinh(expression*[], int, exprvals*, datatype*);
void prim_iCosh(expression*[], int, exprvals*, datatype*);
void prim_iTanh(expression*[], int, exprvals*, datatype*);
void prim_iAsinh(expression*[], int, exprvals*, datatype*);
void prim_iAcosh(expression*[], int, exprvals*, datatype*);
void prim_iAtanh(expression*[], int, exprvals*, datatype*);
void prim_iRadians(expression*[], int, exprvals*, datatype*);
void prim_iDegrees(expression*[], int, exprvals*, datatype*);
void prim_iBnot(expression*[], int, exprvals*, datatype*);
void prim_iBand(expression*[], int, exprvals*, datatype*);
void prim_iBor(expression*[], int, exprvals*, datatype*);
void prim_iBxor(expression*[], int, exprvals*, datatype*);
void prim_iLshift(expression*[], int, exprvals*, datatype*);
void prim_iRashift(expression*[], int, exprvals*, datatype*);
void prim_iRlshift(expression*[], int, exprvals*, datatype*);
void prim_iLnot(expression*[], int, exprvals*, datatype*);
void prim_iLand(expression*[], int, exprvals*, datatype*);
void prim_iLor(expression*[], int, exprvals*, datatype*);
void prim_iLxor(expression*[], int, exprvals*, datatype*);
void prim_iLess(expression*[], int, exprvals*, datatype*);
void prim_iLequal(expression*[], int, exprvals*, datatype*);
void prim_iEqual(expression*[], int, exprvals*, datatype*);
void prim_iNequal(expression*[], int, exprvals*, datatype*);
void prim_iMequal(expression*[], int, exprvals*, datatype*);
void prim_iMore(expression*[], int, exprvals*, datatype*);
void prim_iIf(expression*[], int, exprvals*, datatype*);
void prim_iRand(expression*[], int, exprvals*, datatype*);
void prim_iSrand(expression*[], int, exprvals*, datatype*);
void prim_iFascii(expression*[], int, exprvals*, datatype*);
void prim_iBoo(expression*[], int, exprvals*, datatype*);
void prim_iAscii(expression*[], int, exprvals*, datatype*);
void prim_iInt(expression*[], int, exprvals*, datatype*);
void prim_iFlo(expression*[], int, exprvals*, datatype*);
void prim_iStr(expression*[], int, exprvals*, datatype*);
void prim_iArr(expression*[], int, exprvals*, datatype*);
void prim_iTyp(expression*[], int, exprvals*, datatype*);

#endif
