/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdio.h>
#include <string.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_header.h"
#include "../../testing/cspec_output_unit.h"
#include "../../testing/cspec_output_verbose.h"

DESCRIBE(strcmp, "int strcmp ( const char * str1, const char * str2 )")

	IT( "returns 0 only when strings are equal" )
		SHOULD_EQUAL( strcmp("hello", "hello"), 0)
		SHOULD_NOT_BE_NULL( strcmp("hello", "world") )
	END_IT

	IT( "returns a negative integer when str1 is less than str2"  )
		SHOULD_BE_TRUE( strcmp("hello", "world") < 0 )
		SHOULD_BE_TRUE( strcmp("0123", "1321431") < 0 )
	END_IT

	IT( "returns a positive integer if str1 is greater than str2"  )
		SHOULD_BE_TRUE( strcmp("yellow", "world") > 0 )
		SHOULD_BE_TRUE( strcmp("9", "789") > 0 )
	END_IT

END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(strcmp), CSpec_NewOutputHeader());
	
	return 0;
}

