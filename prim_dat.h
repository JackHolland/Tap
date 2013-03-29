/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_dat.h
    @brief  The header file for prim_dat.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef PRIM_DAT_H
#define PRIM_DAT_H

#include "structs.h"

void prim_dYear(expression*[], int, exprvals*, datatype*);
void prim_dMonth(expression*[], int, exprvals*, datatype*);
void prim_dDay(expression*[], int, exprvals*, datatype*);
void prim_dHour(expression*[], int, exprvals*, datatype*);
void prim_dMinute(expression*[], int, exprvals*, datatype*);
void prim_dSecond(expression*[], int, exprvals*, datatype*);
void prim_dWeekOfYear(expression*[], int, exprvals*, datatype*);
void prim_dWeekOfMonth(expression*[], int, exprvals*, datatype*);
void prim_dDayOfYear(expression*[], int, exprvals*, datatype*);
void prim_dDayOfWeek(expression*[], int, exprvals*, datatype*);
void prim_dLeapYear(expression*[], int, exprvals*, datatype*);
void prim_dDaysInMonth(expression*[], int, exprvals*, datatype*);
void prim_dAddYears(expression*[], int, exprvals*, datatype*);
void prim_dAddMonths(expression*[], int, exprvals*, datatype*);
void prim_dAddDays(expression*[], int, exprvals*, datatype*);
void prim_dAddHours(expression*[], int, exprvals*, datatype*);
void prim_dAddMinutes(expression*[], int, exprvals*, datatype*);
void prim_dAddSeconds(expression*[], int, exprvals*, datatype*);
void prim_dDifference(expression*[], int, exprvals*, datatype*);
void prim_dInt(expression*[], int, exprvals*, datatype*);
void prim_dStr(expression*[], int, exprvals*, datatype*);
void prim_dDat(expression*[], int, exprvals*, datatype*);
void prim_dTyp(expression*[], int, exprvals*, datatype*);

#endif
