/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_obj.c
    @brief  All of the primitive functions for objects used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include "prim_obj.h"
#include "../source/constants.h"
#include "../source/constructors.h"

/*! Returns a copy of the given object (obj)->obj
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_oObj (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_OBJ;
    returnval->objval = copyObject(args[0]->ev.objval);
}

/*! Returns the type object (obj)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_oTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_OBJ;
}
