/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors_test.c
    @brief  Tests for constructors.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../dates.h"

DESCRIBE(printDate, "char* printDate(date, char*)")
	char* str1;
	char* str2;
	char* result;
	date dat = 671947200;
	
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
		
	END_IT
	
	IT("Prints the given date and time in the given format")
		
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(printDate), CSpec_NewOutputUnit());
	
	return 0;
}

