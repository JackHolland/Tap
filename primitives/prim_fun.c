/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_fun.c
    @brief  All of the primitive functions for functions used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "prim_fun.h"
#include "../source/constants.h"
#include "../source/engine.h"
#include "../source/constructors.h"

/*! Returns the given function's arguments as an array of strings (fun)->arr
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uArgs (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    tap_fun* fun = args[0]->ev.funval;
    int fnumargs;
    if (fun->maxargs == ARGLEN_INF) {
        fnumargs = fun->minargs;
    } else {
        fnumargs = fun->maxargs;
    }
    array* result = newArray(numargs);
    expression* expr;
    int i;
    for (i = 0; i < fnumargs; ++i) {
        expr = newExpression_t(TYPE_STR);
        expr->ev.strval = newString(printArg(fun->args[i]));
        result->content[i] = expr;
    }
    *returntype = TYPE_ARR;
    returnval->arrval = result;
}

/*! Returns the given function's minimum number of arguments (fun)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uMinargs (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.funval->minargs;
}

/*! Returns the given function's maximum number of arguments (fun)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uMaxargs (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.funval->maxargs;
}

/*! Returns information about the given function in the form of a string (fun)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    /*tap_fun* fun = args[0]->ev.funval;
    uint size = 11;
    int i;
    for (i = 0; i < fun->numargs; ++i) {

    }
    char* result = allocate(size + 1);
    result[size] = '\0';
    *returntype = TYPE_STR;
    returnval->strval = newString(result);*/
}

/*! Returns a copy of the given function (fun)->fun
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uFun (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_FUN;
    tap_fun* uf = args[0]->ev.funval;
    returnval->funval = newUserfunction(NULL, uf->minargs, uf->maxargs, copyExpression(uf->body));
    memcpy(returnval->funval->args, uf->args, sizeof(uf->args));
}

/*! Returns type function (fun)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_uTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_FUN;
}
