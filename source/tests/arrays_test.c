/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_header.h"
#include "../../testing/cspec_output_unit.h"
#include "../../testing/cspec_output_verbose.h"

#include "../arrays.h"
#include "../constructors.h"
#include "../constants.h"

DESCRIBE(resizeArray, "array* resizeArray (array* arr, int start, int end)")
	array* arr1 = newArray(3);
	arr1->content[0] = newExpression_int(2);
	arr1->content[1] = newExpression_int(3);
	arr1->content[2] = newExpression_flo(4.0);
	
	IT("returns the same array with a smaller size when the size is reduced")
		array* result = resizeArray(arr1, 0, 1);
		SHOULD_EQUAL(result, arr1)
		SHOULD_EQUAL(result->size, 2)
	END_IT
	
	IT("returns a bigger array with new items set to nil when the size is increased")
		array* result = resizeArray(arr1, 0, 3);
		SHOULD_EQUAL(result->size, 3)
		SHOULD_EQUAL(result->content[3]->type, TYPE_NIL)
	END_IT
	
	freeArray(arr1);
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(resizeArray), CSpec_NewOutputUnit());
	
	return 0;
}

