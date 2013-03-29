/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_arr.c
    @brief  All of the primitive functions for arrays used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "prim_arr.h"
#include "../source/constants.h"
#include "../source/engine.h"
#include "../source/constructors.h"
#include "../source/casting.h"
#include "../source/arrays.h"

/*! Returns from the given array the element at the given index (arr, int)->*
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aGet (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    array* arr = args[0]->ev.arrval;
    int index = castToInt(args[1]) + arr->start;
    if (index < arr->end) {
        expression* expr = arr->content[index];
        *returntype = expr->type;
        memcpy(&returnval, &(expr->ev), sizeof(expr->ev));
    } else {
        *returntype = TYPE_NIL;
        returnval->intval = NIL;
    }
}

/*! Sets the value in the given array the element at the given index to the given value, returning 1 on success and 0 on failure (arr, int, *)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aSet (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int index = castToInt(args[1]);
    array* arr = args[0]->ev.arrval;
    if (index < arr->end) {
        expression* value = arr->content[arr->start + index];
        value->type = args[2]->type;
        memcpy(&value->ev, &(args[2]->ev), sizeof(args[2]->ev));
    } else {
        returnval->intval = 0;
    }
    *returntype = TYPE_NIL;
}

/*! Returns the size of the given array (arr)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aSize (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.arrval->end - args[0]->ev.arrval->start;
}

/*! Sets the size of the given array, reallocating memory if needed, returning 1 on success and 0 on failure (arr, [int], [int])->int
    If two size arguments are given then the first argument sets the left bound and the second one sets the right bound
    If no size arguments are given then the array is trimmed to minimize its memory
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aResize (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    array* arr = args[0]->ev.arrval;
    int success = 1;
    int start;
    int end;
    if (numargs == 1) {
        if (arr->start > 0 || arr->end < arr->size) {
            array* newarr = newArray(arr->end - arr->start);
            int newindex = 0;
            int i;
            for (i = arr->start; i < arr->end; ++i) {
                newarr->content[newindex] = arr->content[i];
            }
            free(arr);
            args[0]->ev.arrval = newarr;
        }
    } else {
        if (numargs == 2) {
            start = arr->start;
            end = castToInt(args[1]);
        } else {
            start = castToInt(args[1]);
            end = castToInt(args[3]);
        }
        if (start > end) {
            success = 0;
        } else {
            args[0]->ev.arrval = resizeArray(arr, start, end);
        }
    }
    *returntype = TYPE_INT;
    returnval->intval = success;
}

/*! Concatenate the given list of arrays into one array, turning non-arrays into single array elements (arr...)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aConcat (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int size = args[0]->ev.arrval->end;
    int i;
    for (i = 1; i < numargs; ++i) {
        if (args[i]->type == TYPE_ARR) {
            size += args[i]->ev.arrval->end - args[i]->ev.arrval->start;
        } else {
            ++size;
        }
    }
    array* result = newArray(size);
    int index = 0;
    array* temparr;
    int j;
    for (i = 0; i < numargs; ++i) {
        temparr = castToArr(args[i]);
        for (j = temparr->start; j < temparr->end; ++j) {
            result->content[index++] = temparr->content[j];
        }
    }
    *returntype = TYPE_ARR;
    returnval->arrval = result;
}

/*! Converts the given array into a string using the given delimiter or ', ' if none is given (arr, [str])->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    array* arr = args[0]->ev.arrval;
    int arrsize = arr->end - arr->start;
    char* delimiter;
    if (numargs == 1) {
        delimiter = ", ";
    } else {
        string* str = castToStr(args[1]);
        delimiter = str->content;
        free(str);
    }
    int demsize = strlen(delimiter);
    int size = demsize * (arrsize - 1) + 3;
    string* strings[arrsize];
    int lengths[arrsize];
    int i;
    for (i = 0; i < arrsize; ++i) {
        strings[i] = castToStr(arr->content[arr->start + i]);
        lengths[i] = strings[i]->size;
        size += strings[i]->size;
    }
    char* result = allocate(size);
    result[0] = '{';
    int index = 1;
    for (i = 0; i < arrsize - 1; ++i) {
        strcpy(result + index, strings[i]->content);
        index += lengths[i];
        strcpy(result + index, delimiter);
        index += demsize;
        free(strings[i]);
    }
    if (arrsize == 0) {
        result[1] = '}';
        result[2] = '\0';
    } else {
        strcpy(result + index, strings[arrsize - 1]->content);
        result[index + 1] = '}';
        result[index + 2] = '\0';
        free(strings[arrsize - 1]);
    }
    *returntype = TYPE_STR;
    returnval->strval = newString(result);
}

/*! Returns a copy of the given array (arr)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aArr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_ARR;
    array* arr = args[0]->ev.arrval;
    returnval->arrval = newArray(arr->size);
    returnval->arrval->start = arr->start;
    returnval->arrval->end = arr->end;
    memcpy(returnval->arrval->content, arr->content, sizeof(arr->content));
}

/*! Returns type array (arr)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_aTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_ARR;
}
