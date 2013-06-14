/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors.h
    @brief  The header file for constructors.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef CONSTRUCTORS_H
#define CONSTRUCTORS_H

#include "structs.h"
#include "dep_structs.h"

expression* newExpression();
expression* newExpression_t(datatype);
expression* newExpression_int(tap_int);
expression* newExpression_flo(tap_flo);
expression* newExpression_str(string*);
expression* newExpression_arr(array*);
expression* newExpression_laz(expression*);
expression* newExpression_all(datatype, exprvals*, expression*, linenum);
expression* copyExpression(expression*);
tap_laz* newLazyexpr();
string* newString(char*);
array* newArray(int);
array* copyArray(array*);
array* copyArrayDeep(array*);
tap_obj* newObject(datatype, property*);
tap_obj* copyObject(tap_obj*);
type* newType(datatype, char*, stringlist*, typelist*, property*);
property* newProperty(char*, typelist*, int, int, expression*);
property* copyProperty(property*);
tap_fun* newUserfunction(argument*[], int, int, expression*);
tap_fun* copyUserfunction(tap_fun*);
argument* newArgument(string*, typelist*, expression*);
argument* copyArgument(argument*);
typelist* newTypelist(datatype);
typelist* newTypelist_n(datatype, typelist*);
typelist* copyTypelist(typelist*);
typelist* copyTypelists(typelist*);
typedefs* newTypedefs(type*);
expressionstack* newExpressionstack(expressionstack*);
tap_prim_fun* newPrimFunction(void(*address)(expression*[], int, exprvals*, datatype*), int, int, typelist*);
environment* newEnvironment(hashtable*, int);
stringlist* newStringlist(string*, stringlist*);
errorlist* newErrorlist(uint, string*, linenum, uint);

#endif
