/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_header.h"
#include "../../testing/cspec_output_unit.h"
#include "../../testing/cspec_output_verbose.h"

#include "../arrays.h"
#include "../constructors.h"
#include "../constants.h"
#include "../memory.h"

void freeIfDiffArrays(array*, array*);

DESCRIBE(arrayUsedSize, "int arrayUsedSize (array* arr)")
	array* arr1 = newArray(3);
	IT("returns the size of the used portion of the array")
		SHOULD_EQUAL(arrayUsedSize(arr1), 3)
	END_IT
	freeArray(arr1);
END_DESCRIBE

DESCRIBE(resizeArray, "array* resizeArray (array* arr, int start, int end)")
	array* arr1 = newArray(3);
	arr1->content[0] = newExpression_int(2);
	arr1->content[1] = newExpression_int(3);
	arr1->content[2] = newExpression_flo(4.0);
	array* arr2 = copyArrayDeep(arr1);
	
	IT("returns the same array with a smaller size when the size is reduced")
		array* result = resizeArray(arr1, 0, 1);
		SHOULD_EQUAL(result, arr1)
		SHOULD_EQUAL(arrayUsedSize(result), 2)
		SHOULD_EQUAL(result->content[0]->ev.intval, 2)
		SHOULD_EQUAL(result->content[1]->ev.intval, 3)
		freeIfDiffArrays(result, arr1);
		freeArray(arr1);
	END_IT
	
	IT("returns a bigger array with new items set to nil when the size is increased")
		array* result = resizeArray(arr2, 0, 3);
		SHOULD_EQUAL(arrayUsedSize(result), 4)
		SHOULD_EQUAL(result->content[0]->ev.intval, 2)
		SHOULD_EQUAL(result->content[1]->ev.intval, 3)
		SHOULD_EQUAL(result->content[2]->ev.floval, 4.0)
		SHOULD_EQUAL(result->content[3]->type, TYPE_NIL)
		freeIfDiffArrays(arr2, result);
		freeArray(result);
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(arrayUsedSize), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(resizeArray), CSpec_NewOutputUnit());
	
	return 0;
}

void freeIfDiffArrays (array* arr1, array* arr2) {
	if (arr1 != arr2) {
		free(arr1);
	}
}

