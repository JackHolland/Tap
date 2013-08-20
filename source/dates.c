/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   dates.c
    @brief  All of the date functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdio.h>
#include <time.h>

#include "dates.h"
#include "constants.h"
#include "memory.h"

extern int monthdays[];

/*! Prints the given date according to the given format
    @param dat      the date (UNIX timestamp) to print
    @param format   the format (e.g. "%M/%D/%Y %H:%U:%S %P") to print the date in
    @return         the resulting string
*/
char* printDate (date dat, char* format) {
    tm* timestruct = gmtime(&dat); // convert the UNIX timestamp to a timestruct
    timestruct->tm_mon += 1;
    timestruct->tm_year += 1900;
    int strsize = 0; // the size of the string to return
    int pos = 0; // the current position of the formatting string being inspected
    while (format[pos] != '\0') { // while there are more characters
        if (format[pos] == '%') { // if the current format character is a percent sign (i.e. control character)
            switch (format[pos + 1]) { // inspect the next formatting character
                case 'd': // day of the month without '0' padding
                    if (timestruct->tm_mday < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'D': // day of the month with '0' padding
                case 'F': // week of the year with '0' padding
                case 'H': // hour in non-military time with '0' padding
                case 'I': // hour in military time with '0' padding
                case 'M': // month with '0' padding
                case 'p': // am or pm (lowercase)
                case 'P': // AM or PM (uppercase)
                case 'S': // second with '0' padding
                case 'U': // minute with '0' padding
                case 'y': // abbreviated year
                    strsize += 2;
                    break;
                case 'e': // day of the year without '0' padding
                    if (timestruct->tm_yday < 10) {
                        strsize += 1;
                    } else if (timestruct->tm_yday < 100) {
                        strsize += 2;
                    } else {
                        strsize += 3;
                    }
                    break;
                case 'E': // day of the year with '0' padding
                    strsize += 3;
                    break;
                case 'f': // week of the year without '0' padding
                    if (timestruct->tm_yday / 7 < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'h': // hour in non-military time without '0' padding
                    if (timestruct->tm_hour < 10 || (timestruct->tm_hour > 12 && timestruct->tm_hour < 22)) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'i': // hour in military time without '0' padding
                    if (timestruct->tm_hour < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'm': // month without '0' padding
                    if (timestruct->tm_mon < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'n': // abbreviated month name
                case 'o': // abbreviated day of the week name
                    strsize += 3;
                    break;
                case 'N': // full month name
                    switch (timestruct->tm_mon) {
                        case 0: // January
                        case 9: // October
                            strsize += 7;
                            break;
                        case 1: // February
                        case 10: // November
                        case 11: // December
                            strsize += 8;
                            break;
                        case 2: // March
                        case 3: // April
                            strsize += 5;
                            break;
                        case 4: // May
                            strsize += 3;
                            break;
                        case 5: // June
                        case 6: // July
                            strsize += 4;
                            break;
                        case 7: // August
                            strsize += 6;
                            break;
                        case 8: // September
                            strsize += 9;
                            break;
                    }
                    break;
                case 'O': // full day of the week name
                    switch (timestruct->tm_wday) {
                        case 0: // Sunday
                        case 1: // Monday
                        case 5: // Friday
                            strsize += 6;
                            break;
                        case 2: // Tuesday
                            strsize += 7;
                            break;
                        case 3: // Wednesday
                            strsize += 9;
                            break;
                        case 4: // Thursday
                        case 6: // Saturday
                            strsize += 8;
                            break;
                    }
                    break;
                case 's': // second without '0' padding
                    if (timestruct->tm_sec < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'u': // minute without '0' padding
                    if (timestruct->tm_min < 10) {
                        strsize += 1;
                    } else {
                        strsize += 2;
                    }
                    break;
                case 'w': // day of the week
                case '%': // percent sign
                    strsize += 1;
                    break;
                case 'Y': // full year
                    strsize += 4;
                    break;
                default: // unrecognized formatting character
                    return NULL;
            }
            pos += 2; // advance to the next formatting character
        } else {
            ++strsize; // increment the size of the string
            ++pos; // advance to the next formatting character
        }
    }
    char* str = allocate(strsize + 1); // allocate the spaced needed for the string
    char* strpos = str; // the current position of the string to add to
    pos = 0; // start at the beginning of the formatting string again
    while (format[pos] != '\0') { // while there are more characters
        if (format[pos] == '%') { // if the current format character is a percent sign (i.e. control character)
            switch (format[pos + 1]) { // inspect the next formatting character
                case 'd': // day of the month without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_mday);
                    break;
                case 'D': // day of the month with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_mday);
                    break;
                case 'e': // day of the year without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_yday);
                    break;
                case 'E': // day of the year with '0' padding
                    strpos += sprintf(strpos, "%03d", timestruct->tm_yday);
                    break;
                case 'f': // week of the year without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_yday / 7);
                    break;
                case 'F': // week of the year with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_yday / 7);
                    break;
                case 'h': // hour in non-military time without '0' padding
                    if (timestruct->tm_hour > 12) {
                        strpos += sprintf(strpos, "%d", timestruct->tm_hour - 12);
                    } else {
                        strpos += sprintf(strpos, "%d", timestruct->tm_hour);
                    }
                    break;
                case 'H': // hour in non-military time with '0' padding
                    if (timestruct->tm_hour > 12) {
                        strpos += sprintf(strpos, "%02d", timestruct->tm_hour - 12);
                    } else {
                        strpos += sprintf(strpos, "%02d", timestruct->tm_hour);
                    }
                    break;
                case 'i': // hour in military time without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_hour);
                    break;
                case 'I': // hour in military time with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_hour);
                    break;
                case 'm': // month without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_mon);
                    break;
                case 'M': // month with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_mon);
                    break;
                case 'n': // abbreviated month name
                    switch (timestruct->tm_mon) {
                        case 0:
                            strpos += sprintf(strpos, "Jan");
                            break;
                        case 1:
                            strpos += sprintf(strpos, "Feb");
                            break;
                        case 2:
                            strpos += sprintf(strpos, "Mar");
                            break;
                        case 3:
                            strpos += sprintf(strpos, "Apr");
                            break;
                        case 4:
                            strpos += sprintf(strpos, "May");
                            break;
                        case 5:
                            strpos += sprintf(strpos, "Jun");
                            break;
                        case 6:
                            strpos += sprintf(strpos, "Jul");
                            break;
                        case 7:
                            strpos += sprintf(strpos, "Aug");
                            break;
                        case 8:
                            strpos += sprintf(strpos, "Sep");
                            break;
                        case 9:
                            strpos += sprintf(strpos, "Oct");
                            break;
                        case 10:
                            strpos += sprintf(strpos, "Nov");
                            break;
                        case 11:
                            strpos += sprintf(strpos, "Dec");
                            break;
                    }
                    break;
                case 'N': // full month name
                    switch (timestruct->tm_mon) {
                        case 0:
                            strpos += sprintf(strpos, "January");
                            break;
                        case 1:
                            strpos += sprintf(strpos, "February");
                            break;
                        case 2:
                            strpos += sprintf(strpos, "March");
                            break;
                        case 3:
                            strpos += sprintf(strpos, "April");
                            break;
                        case 4:
                            strpos += sprintf(strpos, "May");
                            break;
                        case 5:
                            strpos += sprintf(strpos, "June");
                            break;
                        case 6:
                            strpos += sprintf(strpos, "July");
                            break;
                        case 7:
                            strpos += sprintf(strpos, "August");
                            break;
                        case 8:
                            strpos += sprintf(strpos, "September");
                            break;
                        case 9:
                            strpos += sprintf(strpos, "October");
                            break;
                        case 10:
                            strpos += sprintf(strpos, "November");
                            break;
                        case 11:
                            strpos += sprintf(strpos, "December");
                            break;
                    }
                    break;
                case 'o': // abbreviated day of the week name
                    switch (timestruct->tm_wday) {
                        case 0:
                            strpos += sprintf(strpos, "Sun");
                            break;
                        case 1:
                            strpos += sprintf(strpos, "Mon");
                            break;
                        case 2:
                            strpos += sprintf(strpos, "Tue");
                            break;
                        case 3:
                            strpos += sprintf(strpos, "Wed");
                            break;
                        case 4:
                            strpos += sprintf(strpos, "Thu");
                            break;
                        case 5:
                            strpos += sprintf(strpos, "Fri");
                            break;
                        case 6:
                            strpos += sprintf(strpos, "Sat");
                            break;
                    }
                    break;
                case 'O': // full day of the week name
                    switch (timestruct->tm_wday) {
                        case 0:
                            strpos += sprintf(strpos, "Sunday");
                            break;
                        case 1:
                            strpos += sprintf(strpos, "Monday");
                            break;
                        case 2:
                            strpos += sprintf(strpos, "Tuesday");
                            break;
                        case 3:
                            strpos += sprintf(strpos, "Wednesday");
                            break;
                        case 4:
                            strpos += sprintf(strpos, "Thursday");
                            break;
                        case 5:
                            strpos += sprintf(strpos, "Friday");
                            break;
                        case 6:
                            strpos += sprintf(strpos, "Saturday");
                            break;
                    }
                    break;
                case 'p': // am or pm (lowercase)
                    if (timestruct->tm_hour < 12) {
                        strpos += sprintf(strpos, "am");
                    } else {
                        strpos += sprintf(strpos, "pm");
                    }
                    break;
                case 'P': // AM or PM (uppercase)
                    if (timestruct->tm_hour < 12) {
                        strpos += sprintf(strpos, "AM");
                    } else {
                        strpos += sprintf(strpos, "PM");
                    }
                    break;
                case 's': // second without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_sec);
                    break;
                case 'S': // second with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_sec);
                    break;
                case 'u': // minute without '0' padding
                    strpos += sprintf(strpos, "%d", timestruct->tm_min);
                    break;
                case 'U': // minute with '0' padding
                    strpos += sprintf(strpos, "%02d", timestruct->tm_min);
                    break;
                case 'w': // day of the week
                    strpos += sprintf(strpos, "%d", timestruct->tm_wday);
                    break;
                case 'y': // abbreviated year
                    strpos += sprintf(strpos, "%d", timestruct->tm_year % 1000);
                    break;
                case 'Y': // full year
                    strpos += sprintf(strpos, "%d", timestruct->tm_year);
                    break;
                case '%': // percent sign
                    strpos += sprintf(strpos, "%%");
                    break;
            }
            pos += 2; // advance to the next formatting character
        } else {
            strpos[0] = format[pos++]; // add the format string character to the string and advance to the next character
            ++strpos;
        }
    }
    str[strsize] = '\0'; // null terminate the string and return it
    return str;
}

/*! Returns the year and the number of leap years of the given UNIX timestamp in the given yearandleap struct
    @param dat      the date (UNIX timestamp) to calculate with
    @param yal      the struct to store the results in
    @return         nothing
*/
void yearOfDate (date dat, yearandleap* yal) {
    long days = dat / SEC_IN_DAY;
    yal->year = days / DAYS_IN_YEAR + YEAR_ZERO;
    yal->leapyears = yal->year / 4 - yal->year / 100 + yal->year / 400;
    if (yal->year >= YEAR_ZERO) {
        yal->leapyears -= YEAR_ZERO_LEAP_DAYS;
    } else {
        yal->leapyears = YEAR_ZERO_LEAP_DAYS - yal->leapyears;
    }
    yal->year -= (yal->leapyears / DAYS_IN_YEAR + (days % DAYS_IN_YEAR + 1) < yal->leapyears);
}

/*! Returns the month, day of the month, and day of year of the given UNIX timestamp in the given monthandday struct
    @param dat      the date (UNIX timestamp) to calculate with
    @param mad      the struct to store the results in
    @return         nothing
*/
void monthOfDate (date dat, monthandday* mad) {
    yearandleap yal;
    yearOfDate(dat, &yal);
    long days = dat / SEC_IN_DAY;
    mad->day = ((days % DAYS_IN_YEAR - yal.leapyears) + DAYS_IN_YEAR) % DAYS_IN_YEAR + 1;
    mad->dayofyear = mad->day;
    if (mad->day > 59 && leapYear(yal.year)) {
        mad->day -= 1;
    }
    mad->month = 0;
    while (mad->day >= monthdays[mad->month]) {
        mad->month += 1;
    }
    mad->day -= monthdays[mad->month - 1];
    mad->month += 1;
}

/*! Returns whether or not the given year is a leap year
    @param year     the year to check
    @return         1 if the given year is a leap year or 0 if it isn't
*/
int leapYear (long year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

