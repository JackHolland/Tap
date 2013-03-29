/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   dates.h
    @brief  The header file for dates.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef DATES_H
#define DATES_H

#include "structs.h"

typedef struct yearandleap_ yearandleap;
typedef struct monthandday_ monthandday;

struct yearandleap_ {
    long year;
    long leapyears;
};

struct monthandday_ {
    int month;
    int day;
    int dayofyear;
};

char* printDate(date, char*);
void yearOfDate(date, yearandleap*);
void monthOfDate(date, monthandday*);
int leapYear(long);

#endif
