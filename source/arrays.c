/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "arrays.h"
#include "constructors.h"
#include "constants.h"

/*! Returns the size of the array being used (i.e. does not include unused padding from original allocation)
    @param arr      the array whose size should be calculated
    @return         the used size of the given array
*/
int arrayUsedSize (array* arr) {
	return arr->end - arr->start + 1;
}

/*! Resizes the given array using the given start and end indices
    @param arr      the array to resize
    @param start    the new starting index
    @param end      the new ending index
    @return         the new array (or the existing one if no memory needed to be allocated)
*/
array* resizeArray (array* arr, int start, int end) {
	int i;
	int arrspan = arr->end - arr->start;
	int newsize = end - start + 1;
	int index = 0;
	int arrstart = arr->start;
	int arrend = arr->end;
	int samearray;
	array* newarr;
	if (newsize > arr->size) {
		samearray = 0;
		newarr = newArray(newsize);
	} else {
		samearray = 1;
		newarr = arr;
	}
	if (start < 0) {
		for (i = start; i < 0; i++) {
			newarr->content[index++] = newExpression_t(TYPE_NIL);
		}
	} else {
		for (i = 0; i < start; i++) {
			freeExpr(arr->content[arrstart++]);
		}
	}
	if (arrspan < end) {
		int tempindex = index + 1;
		for (i = arrspan; i < end; i++) {
			newarr->content[arrspan + tempindex++] = newExpression_t(TYPE_NIL);
		}
	} else {
		for (i = end; i < arrspan; i++) {
			freeExpr(arr->content[arrend--]);
		}
	}
	if (samearray) {
		newarr->start = arrstart;
		newarr->end = arrend;
	} else {
		for (i = arrstart; i <= arrend; i++) {
			newarr->content[index++] = arr->content[i];
		}
	}
	return newarr;
}

