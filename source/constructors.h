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
expression* newExpressionOfType(datatype);
expression* newExpressionNil();
expression* newExpressionInt(tap_int);
expression* newExpressionFlo(tap_flo);
expression* newExpressionStr(string*);
expression* newExpressionArr(array*);
expression* newExpressionLaz(expression*);
expression* newExpressionTyp(datatype);
expression* newExpressionAll(datatype, exprvals*, expression*, linenum);
expression* copyExpression(expression*);
tap_laz* newLazyExpression();
string* newString(char*);
array* newArray(int);
array* copyArray(array*);
array* copyArrayDeep(array*);
date newDate(string*);
tap_obj* newObject(datatype, property*);
tap_obj* copyObject(tap_obj*);
type* newType(datatype, char*, stringlist*, typelist*, property*);
property* newProperty(char*, typelist*, int, int, expression*);
property* copyProperty(property*);
tap_fun* newTapFunction(argument*[], int, int, expression*);
tap_fun* copyTapFunction(tap_fun*);
argument* newArgument(string*, typelist*, expression*);
argument* copyArgument(argument*);
typelist* newTypelist(datatype);
typelist* newTypelistWithNext(datatype, typelist*);
typelist* copyTypelist(typelist*);
typelist* copyTypelistDeep(typelist*);
typedefs* newTypedefs(type*);
exprstack* newExprstack(exprstack*);
tap_prim_fun* newPrimFunction(void(*address)(expression*[], int, exprvals*, datatype*), int, int, typelist*);
environment* newEnvironment(hashtable*, int);
stringlist* newStringlist(string*, stringlist*);
errorlist* newErrorlist(uint, string*, linenum, uint);

#endif
