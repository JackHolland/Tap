/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_laz.c
    @brief  All of the primitive function for lazy expressions used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "prim_laz.h"
#include "prim_int.h"
#include "../source/constants.h"
#include "../source/engine.h"
#include "../source/constructors.h"
#include "../source/strings.h"

/*! Evaluates the given lazy expression and returns the result (laz)->*
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lEval (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    expression* result = evaluateLazy(copyExpression(args[0]));
    *returntype = result->type;
    returnval->intval = result->ev.intval;
    freeExpr(result);
}

/*! Creates and returns a function using the given arguments and body (laz, exp)->fun
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lFunction (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    int minargs = 0;
    int maxargs = 0;
    expression* tempexpr1 = args[0]->ev.lazval->expval;
    if (tempexpr1 == NULL) {
        // error, no valid arguments were given
    }
    expression* tempexpr2 = tempexpr1;
    while (tempexpr2 != NULL) {
        ++maxargs;
        tempexpr2 = tempexpr2->next;
    }
    argument* fargs[maxargs];
    typelist* types = NULL;
    int i;
    for (i = 0; i < maxargs; ++i) {
        if (tempexpr1->type == TYPE_EXP || tempexpr1->type == TYPE_LAZ) {
            if (tempexpr1->type == TYPE_EXP) {
                if (minargs == maxargs) {
                    // error, optional arguments must come last
                } else {
                    ++minargs;
                    tempexpr2 = tempexpr1->ev.expval;
                }
            } else {
                ++minargs;
                tempexpr2 = tempexpr1->ev.lazval->expval;
            }
            if (tempexpr2->type == TYPE_STR) {
                types = newTypelist(typeFromString(tempexpr2->ev.strval->content));
            } else if (tempexpr2->type == TYPE_EXP && tempexpr2->flag == EFLAG_ARR) {
                expression* typearr = tempexpr2->ev.expval;
                if (typearr == NULL) {
                    types = newTypelist(TYPE_UNK);
                } else {
                    typelist* tempat = types;
                    while (typearr != NULL) {
                        if (typearr->type == TYPE_STR) {
                            typelist* newat = newTypelist(typeFromString(typearr->ev.strval->content));
                            if (tempat == NULL) {
                                types = newat;
                                tempat = types;
                            } else {
                                tempat->next = newat;
                                tempat = tempat->next;
                            }
                            typearr = typearr->next;
                        } else {
                            // error, the type must be a string
                        }
                    }
                }
            } else {
                // error, the type must be either a string or an array of strings
            }
            if (tempexpr2->next == NULL) {
                if (types == NULL) {
                    fargs[i] = newArgument(copyString(tempexpr2->ev.strval), newTypelist(TYPE_UNK), NULL);
                } else {
                    // error, name a variable after a type
                }
            } else if (tempexpr2->next->next == NULL) {
                if (types == NULL) {
                    fargs[i] = newArgument(copyString(tempexpr2->ev.strval), newTypelist(TYPE_UNK), tempexpr2->next);
                } else if (tempexpr2->next->type == TYPE_STR) {
                    fargs[i] = newArgument(copyString(tempexpr2->next->ev.strval), types, NULL);
                } else {
                    // error, the expression after the type must be a string
                }
            } else {
                // error, there cannot be more than two expressions in an argument definition
            }
        } else if (tempexpr1->type == TYPE_STR) {
            if (strcmp(tempexpr1->ev.strval->content, UFUNC_MORE_ARGS) == 0) {
                maxargs = ARGLEN_INF;
            } else {
                fargs[i] = newArgument(copyString(tempexpr1->ev.strval), newTypelist(TYPE_UNK), NULL);
            }
        } else {
            // error, the argument expression must be a string
        }
        tempexpr1 = tempexpr1->next;
    }
    *returntype = TYPE_FUN;
    returnval->funval = newUserfunction(fargs, minargs, maxargs, copyExpression(args[1]));
}

/*! Ands each given evaluated lazy expression as a boolean with the next evaluated lazy expression as a boolean (laz...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lAnd (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_iLand(args, numargs, returnval, returntype);
}

/*! Ors each bit of each given evaluated lazy expression with each bit of the next evaluated lazy expression (laz...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lOr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_iLor(args, numargs, returnval, returntype);
}

/*! Exclusive-ors each bit of each given evaluated lazy expression with each bit of the next evaluated lazy expression (laz...)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lXor (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_iLxor(args, numargs, returnval, returntype);
}

/*! Returns the second argument if the first is true, the fourth argument if the second is true, etc. (laz...)->*
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lIf (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    prim_iIf(args, numargs, returnval, returntype);
}

/*! Returns a copy of the given lazy expression (laz)->laz
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lLaz (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_LAZ;
    returnval->lazval = newLazyexpr();
    tap_laz* le = args[0]->ev.lazval;
    returnval->lazval->expval = copyExpression(le->expval);
    expressionstack* oldref = le->refs;
    expressionstack** newref = &(returnval->lazval->refs);
    while (oldref != NULL) {
        memcpy(*newref, oldref, sizeof(oldref));
        (*newref)->next = oldref->next;
        newref = &(*newref)->next;
    }
}

/*! Returns type lazy expression (laz)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_lTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_LAZ;
}
