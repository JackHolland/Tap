/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   prim_typ.c
    @brief  All of the primitive functions for types used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "prim_typ.h"
#include "constants.h"
#include "engine.h"
#include "constructors.h"
#include "externs.h"
#include "types.h"
#include "strings.h"

extern environment* environments[];
extern uint cenvironment;

/*! Creates a new object of the given type and properties or nil if the type if something fails (typ, [laz])->obj/nil
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_tNew (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    property* properties = NULL;
    property* curprop;
    datatype tid = (datatype)(args[0]->ev.intval);
    type* typ = NULL;
    int cenv = cenvironment;
    while (cenv >= 0) {
        typedefs* td = environments[cenv]->types;
        while (td != NULL) {
            if (tid == td->type->id) {
                typ = td->type;
                cenv = -1;
                break;
            }
        }
        --cenv;
    }
    if (typ != NULL) {
        if (numargs == 2) {
            expression* prop = args[1]->ev.lazval->expval;
            while (prop != NULL) {
                if (prop->type == TYPE_EXP && prop->ev.expval->type == TYPE_STR && prop->ev.expval->next != NULL) {
                    char* propname = prop->ev.expval->ev.strval->content;
                    expression* propval = copyExpression(evaluateArgument(prop->ev.expval->next));
                    property* newprop = NULL;
                    property* proplist = typ->properties;
                    while (proplist != NULL) {
                        if (proplist->range == PROP_RANGE_LOCAL && strcmp(propname, proplist->name) == 0) {
                            if (propTypeValid(proplist, propval)) {
                                newprop = copyProperty(proplist);
                                freeExpr(newprop->value);
                                newprop->value = propval;
                                break;
                            } else {
                                addError(newErrorlist(ERR_INVALID_PROP_TYP, newString(printExpression(prop)), 1, 1));
                                *returntype = TYPE_NIL;
                                return;
                            }
                        }
                        proplist = proplist->next;
                    }
                    if (newprop == NULL) {
                        newprop = newProperty(propname, NULL, PROP_PRIVACY_PRIVATE, PROP_RANGE_LOCAL, propval);
                    }
                    if (properties == NULL) {
                        properties = newprop;
                        curprop = properties;
                    } else {
                        curprop->next = newprop;
                        curprop = curprop->next;
                    }
                } else {
                    addError(newErrorlist(ERR_INVALID_NUM_ARGS, newString(printExpression(prop)), 1, 1));
                    *returntype = TYPE_NIL;
                    return;
                }
                prop = prop->next;
            }
        }
        int valid = 0;
        if (propReqsValid(typ->required, properties)) {
            valid = 1;
            typelist* tl = typ->inherits;
            while (valid && tl != NULL) {
                type* ptyp = NULL;
                uint cenv = cenvironment;
                while (cenv >= 0) {
                    typedefs* td = environments[cenv]->types;
                    while (td != NULL) {
                        if (tl->type == td->type->id) {
                            ptyp = td->type;
                            cenv = -1;
                            break;
                        }
                    }
                    if (ptyp != NULL) {
                        if (!propReqsValid(ptyp->required, properties)) {
                            valid = 0;
                        }
                        break;
                    }
                    --cenv;
                }
            }
        }
        if (valid) {
            object* obj = newObject(args[0]->ev.intval, properties);
            *returntype = TYPE_OBJ;
            returnval->objval = obj;
        } else {
            *returntype = TYPE_NIL;
        }
    } else {
        addError(newErrorlist(ERR_UNDEFINED_TYP, newString(printExpression(args[0])), 1, 1));
        *returntype = TYPE_NIL;
    }
}

/*! Converts the given type to its integer ID (typ)->int
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_tInt (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_INT;
    returnval->intval = args[0]->ev.intval;
}

/*! Converts the given type to its string description (typ)->str
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_tStr (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_STR;
    returnval->strval = newString(printType(args[0]->ev.intval));
}

/*! Returns type type (typ)->typ
    @param args         the list of arguments
    @param numargs      the number of arguments
    @param returnval    the value the function returns after it ends
    @param returntype   the type of value the function returns after it ends
    @return             nothing
*/
void prim_tTyp (expression* args[], int numargs, exprvals* returnval, datatype* returntype) {
    *returntype = TYPE_TYP;
    returnval->intval = TYPE_TYP;
}
