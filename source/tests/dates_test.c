/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors_test.c
    @brief  Tests for dates.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../dates.h"
#include "../engine.h"

DESCRIBE(printDate, "char* printDate(date, char*)")
	char* result;
	date dat = 672016980;
	
	IT("Prints the given date in the given format")
		result = printDate(dat, "%M/%D/%Y");
		SHOULD_EQUAL(strcmp(result, "04/18/1991"), 0)
		free(result);
		result = printDate(dat, "%e ; %n/%d/%y");
		SHOULD_EQUAL(strcmp(result, "107 ; Apr/18/91"), 0)
		free(result);
		result = printDate(dat, "%o (%O) in %N");
		SHOULD_EQUAL(strcmp(result, "Thu (Thursday) in April"), 0)
		free(result);
	END_IT
	
	IT("Prints the given time in the given format")
		result = printDate(dat, "%H:%u:%S %P");
		SHOULD_EQUAL(strcmp(result, "11:23:00 PM"), 0)
		free(result);
		result = printDate(dat, "%I%Uh");
		SHOULD_EQUAL(strcmp(result, "2323h"), 0)
		free(result);
	END_IT
	
	IT("Prints the given date and time in the given format")
		result = printDate(dat, "%m/%D/%Y %H:%U:%S %P");
		SHOULD_EQUAL(strcmp(result, "4/18/1991 11:23:00 PM"), 0)
		free(result);
	END_IT
END_DESCRIBE

DESCRIBE(yearOfDate, "void yearOfDate (date dat, yearandleap* yal)")
	IT("Calculates the year and number of leap years in the given date")
		date dat = 1139979600;
		yearandleap yal;
		yearOfDate(dat, &yal);
		SHOULD_EQUAL(yal.year, 2006)
		SHOULD_EQUAL(yal.leapyears, 9)
	END_IT
END_DESCRIBE

DESCRIBE(monthOfDate, "void monthOfDate (date dat, monthandday* mad)")
	IT("Calculates the month, day of the month, and day of the year of the given date")
		initializeGlobals();
		date dat = 1729008345;
		monthandday mad;
		monthOfDate(dat, &mad);
		SHOULD_EQUAL(mad.month, 10)
		SHOULD_EQUAL(mad.day, 15)
		SHOULD_EQUAL(mad.dayofyear, 288)
		dat = 34658909;
		monthOfDate(dat, &mad);
		SHOULD_EQUAL(mad.month, 2)
		SHOULD_EQUAL(mad.day, 6)
		SHOULD_EQUAL(mad.dayofyear, 37)
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(leapYear, "int leapYear (long year)")
	IT("Determines whether or not the given year is a leap year")
		SHOULD_EQUAL(leapYear(1970), 0)
		SHOULD_EQUAL(leapYear(2000), 1)
		SHOULD_EQUAL(leapYear(2200), 0)
		SHOULD_EQUAL(leapYear(2012), 1)
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(printDate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(yearOfDate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(monthOfDate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(leapYear), CSpec_NewOutputUnit());
	
	return 0;
}

