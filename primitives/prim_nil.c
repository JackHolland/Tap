/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_nil.c
    @brief  All of the primitive functions for nil values used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <string.h>

#include "prim_nil.h"
#include "prim_int.h"
#include "../source/constants.h"
#include "../source/constructors.h"
#include "../source/strings.h"

/*! Converts the given nil value to its integer equivalent, 0 (nil)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_nInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = NIL;
}

/*! Converts the given nil value to its string name, 'nil' (nil)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_nStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = newString(strDup("[nil]"));
}

/*! Returns type nil (nil)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_nTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_NIL;
}
