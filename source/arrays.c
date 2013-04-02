/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "arrays.h"
#include "constructors.h"

/*! Resizes the given array using the given start and end indices
    @param arr      the array to resize
    @param start    the new starting index
    @param end      the new ending index
    @return         the new array (or the existing one if no memory needed to be allocated)
*/
array* resizeArray (array* arr, int start, int end) {
    if (start < 0 || end > arr->size) {
        array* newarr = newArray(end - start);
        start = 0;
        int i;
        for (i = arr->start; i < arr->end; ++i) {
            newarr->content[start++] = arr->content[i];
        }
        free(arr);
        return newarr;
    } else {
        if (start != arr->start) {
            arr->start = start;
        }
        if (end != arr->end) {
            arr->end = end;
        }
        return arr;
    }
}
