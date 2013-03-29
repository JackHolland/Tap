/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_dat.c
    @brief  All of the primitive functions for dates used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "prim_dat.h"
#include "../source/constants.h"
#include "../source/casting.h"
#include "../source/constructors.h"
#include "../source/dates.h"

extern int monthdays[];

/*! Returns the year of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dYear (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    yearandleap yal;
    yearOfDate(args[0]->ev.datval, &yal);
    *returntype = TYPE_INT;
    returnval->intval = yal.year;
}

/*! Returns the month of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dMonth (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    returnval->intval = mad.month;
}

/*! Returns the day of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDay (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    returnval->intval = mad.day;
}

/*! Returns the hour of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dHour (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.datval % SEC_IN_DAY / SEC_IN_HOUR;
}

/*! Returns the minute of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dMinute (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.datval % SEC_IN_HOUR / MIN_IN_HOUR;
}

/*! Returns the second of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dSecond (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.datval % SEC_IN_MIN;
}

/*! Returns the week of the year of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dWeekOfYear (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    returnval->intval = mad.dayofyear / DAYS_IN_WEEK + 1;
}

/*! Returns the week of the month of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dWeekOfMonth (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    returnval->intval = mad.day / DAYS_IN_WEEK + 1 + (((args[0]->ev.datval - mad.day * SEC_IN_DAY) / SEC_IN_DAY + 4) % DAYS_IN_WEEK != 0);
}

/*! Returns the day of the year of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDayOfYear (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    returnval->intval = mad.dayofyear;
}

/*! Returns the day of the week of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDayOfWeek (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = (args[0]->ev.datval / SEC_IN_DAY + 4) % DAYS_IN_WEEK + 1;
}

/*! Returns whether or not the given date is a leap year (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dLeapYear (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    yearandleap yal;
    yearOfDate(args[0]->ev.datval, &yal);
    *returntype = TYPE_INT;
    returnval->intval = leapYear(yal.year);
}

/*! Returns the number of days in the month of the given date (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDaysInMonth (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    yearandleap yal;
    yearOfDate(args[0]->ev.datval, &yal);
    monthandday mad;
    monthOfDate(args[0]->ev.datval, &mad);
    *returntype = TYPE_INT;
    int days;
    switch (mad.month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            days = 31;
            break;
        case 2:
            if (leapYear(yal.year)) {
                days = 29;
            } else {
                days = 28;
            }
            break;
        default:
            days = 30;
    }
    returnval->intval = days;
}

/*! Adds the given number of years to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddYears (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    date dat = args[0]->ev.datval;
    long years = castToInt(args[1]);
    yearandleap yal;
    yearOfDate(dat, &yal);
    long finalyear = years;
    if (years < 0) {
        finalyear *= -1;
    }
    long days = DAYS_IN_YEAR * finalyear;
    days += finalyear / 4 + ((yal.year % 4 + (4 - (yal.year + finalyear % 4))) >= 4);
    if (years < 0) {
        days *= -1;
    }
    *returntype = TYPE_DAT;
    returnval->datval = dat + days * SEC_IN_DAY;
}

/*! Adds the given number of months to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddMonths (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    date dat = args[0]->ev.datval;
    long months = castToInt(args[1]);
    monthandday mad;
    monthOfDate(dat, &mad);
    int monthsleft;
    if (months >= 0) {
        monthsleft = MON_IN_YEAR - mad.month;
    } else {
        monthsleft = mad.month - 1;
    }
    int restofyear;
    int remainder;
    long days = 0;
    if ((months >= 0 && months > monthsleft) || (months < 0 && months < monthsleft * -1)) {
        if (months >= 0) {
            restofyear = MON_IN_YEAR;
            days += monthdays[restofyear - 1] - monthdays[mad.month - 2];
            remainder = (months - monthsleft) % MON_IN_YEAR - 1;
            if (remainder > 1) {
                days += monthdays[remainder - 1];
            }
        } else {
            restofyear = mad.month;
            days += monthdays[MON_IN_YEAR - 1] - monthdays[MON_IN_YEAR + (months + monthsleft) % MON_IN_YEAR - 1] + monthdays[restofyear - 2];
            remainder = (months + monthsleft) * -1 - (MON_IN_YEAR - 3);
        }
    } else {
        restofyear = mad.month + months;
        remainder = 0;
        if (months >= 0) {
            if (restofyear == 1) {
                restofyear = MON_IN_YEAR;
                days += monthdays[0];
            } else {
                days += monthdays[restofyear - 2] - monthdays[mad.month - 2];
            }
        } else {
            if (restofyear == 0) {
                restofyear = mad.month > 1;
                days += monthdays[0];
            }
            days += monthdays[mad.month - 2] - monthdays[mad.month + months - 2];
        }
    }
    long years;
    if (months >= 0) {
        years = months / MON_IN_YEAR;
    } else if (months <= ((MON_IN_YEAR + mad.month - 1) * -1)) {
        years = months * -1 / MON_IN_YEAR;
    } else {
        years = 0;
    }
    long finalyear = years;
    if (months < 0) {
        finalyear *= -1;
    }
    yearandleap yal;
    yearOfDate(dat, &yal);
    days += DAYS_IN_YEAR * years;
    days += years / 4 + ((yal.year % 4 + (4 - (yal.year + finalyear % 4))) >= 4);
    days += (restofyear > 1 && leapYear(yal.year)) + (remainder > 1 && leapYear(yal.year + finalyear + 1));
    if (months < 0) {
        days *= -1;
    }
    *returntype = TYPE_DAT;
    returnval->datval = dat + days * SEC_IN_DAY;
}

/*! Adds the given number of days to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddDays (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_DAT;
    returnval->datval = args[0]->ev.datval + castToInt(args[1]) * SEC_IN_DAY;
}

/*! Adds the given number of hours to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddHours (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_DAT;
    returnval->datval = args[0]->ev.datval + castToInt(args[1]) * SEC_IN_HOUR;
}

/*! Adds the given number of minutes to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddMinutes (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_DAT;
    returnval->datval = args[0]->ev.datval + castToInt(args[1]) * SEC_IN_MIN;
}

/*! Adds the given number of seconds to the given date (dat, int)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dAddSeconds (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_DAT;
    returnval->datval = args[0]->ev.datval + castToInt(args[1]);
}

/*! Returns the difference between two dates (dat, dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDifference (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
}

/*! Converts the given date to an integer in the form of a Unix timestamp (dat)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.datval;
}

/*! Converts the given date to a string with the given format (or "%M/%D/%Y %H:%U:%S %P" if none is given) (dat, [str])->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    static char* defaultformat = "%M/%D/%Y %H:%U:%S %P";
    char* format;
    string* formatstr;
    int freestr = 0;
    if (numargs == 1) {
        format = defaultformat;
    } else {
        formatstr = castToStr(args[1]);
        if (formatstr == NIL) {
            format = defaultformat;
        } else {
            format = formatstr->content;
            freestr = args[1]->type != TYPE_STR;
        }
    }
    string* str = newString(printDate(args[0]->ev.datval, format));
    if (freestr) {
        free(formatstr->content);
        free(formatstr);
    }
    *returntype = TYPE_STR;
    returnval->strval = str;
}

/*! Returns the given date (dat)->dat
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dDat (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_DAT;
    returnval->datval = args[0]->ev.datval;
}

/*! Returns type date (dat)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_dTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_DAT;
}
