/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   casting.h
    @brief  The header file for casting.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef CASTING_H
#define CASTING_H

#include "structs.h"

expression* castToNum(string*, int);
long castToInt(expression*);
expression* castToBoo(expression*);
expression* fRound(double, int);
double castToFlo(expression*);
string* castToStr(expression*);
string* castToStrWithBase(int, uint);
uint alphaNumeric(uint);
array* castToArr(expression*);

#endif
