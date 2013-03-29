/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_flo.h
    @brief  The header file for prim_flo.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_FLO_H
#define PRIM_FLO_H

#include "../source/structs.h"

void prim_fAdd(expression*[], int, exprvals*, datatype*);
void prim_fSub(expression*[], int, exprvals*, datatype*);
void prim_fMul(expression*[], int, exprvals*, datatype*);
void prim_fDiv(expression*[], int, exprvals*, datatype*);
void prim_fSqrt(expression*[], int, exprvals*, datatype*);
void prim_fPow(expression*[], int, exprvals*, datatype*);
void prim_fLog(expression*[], int, exprvals*, datatype*);
void prim_fAbs(expression*[], int, exprvals*, datatype*);
void prim_fMax(expression*[], int, exprvals*, datatype*);
void prim_fMin(expression*[], int, exprvals*, datatype*);
void prim_fRound(expression*[], int, exprvals*, datatype*);
void prim_fCeil(expression*[], int, exprvals*, datatype*);
void prim_fFloor(expression*[], int, exprvals*, datatype*);
void prim_fSin(expression*[], int, exprvals*, datatype*);
void prim_fCos(expression*[], int, exprvals*, datatype*);
void prim_fTan(expression*[], int, exprvals*, datatype*);
void prim_fAsin(expression*[], int, exprvals*, datatype*);
void prim_fAcos(expression*[], int, exprvals*, datatype*);
void prim_fAtan(expression*[], int, exprvals*, datatype*);
void prim_fAtan2(expression*[], int, exprvals*, datatype*);
void prim_fSinh(expression*[], int, exprvals*, datatype*);
void prim_fCosh(expression*[], int, exprvals*, datatype*);
void prim_fTanh(expression*[], int, exprvals*, datatype*);
void prim_fAsinh(expression*[], int, exprvals*, datatype*);
void prim_fAcosh(expression*[], int, exprvals*, datatype*);
void prim_fAtanh(expression*[], int, exprvals*, datatype*);
void prim_fRadians(expression*[], int, exprvals*, datatype*);
void prim_fDegrees(expression*[], int, exprvals*, datatype*);
void prim_fLess(expression*[], int, exprvals*, datatype*);
void prim_fLequal(expression*[], int, exprvals*, datatype*);
void prim_fEqual(expression*[], int, exprvals*, datatype*);
void prim_fNequal(expression*[], int, exprvals*, datatype*);
void prim_fMequal(expression*[], int, exprvals*, datatype*);
void prim_fMore(expression*[], int, exprvals*, datatype*);
void prim_fInt(expression*[], int, exprvals*, datatype*);
void prim_fFlo(expression*[], int, exprvals*, datatype*);
void prim_fStr(expression*[], int, exprvals*, datatype*);
void prim_fArr(expression*[], int, exprvals*, datatype*);
void prim_fTyp(expression*[], int, exprvals*, datatype*);

#endif
