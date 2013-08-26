/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors_test.c
    @brief  Tests for constructors.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../constructors.h"
#include "../constants.h"
#include "../strings.h"
#include "../../primitives/prim_int.h"

DESCRIBE(newExpression, "expression* newExpression ()")
	IT("Creates a nil expression")
		expression* expr = newExpression();
		SHOULD_EQUAL(expr->type, TYPE_NIL)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionOfType, "expression* newExpressionOfType (datatype type)")
	IT("Creates an expression with the given type and initializes it to the default value for that type")
		expression* expr1 = newExpressionOfType(TYPE_INT);
		SHOULD_EQUAL(expr1->ev.intval, 0)
		freeExpr(expr1);
		expression* expr2 = newExpressionOfType(TYPE_ARR);
		SHOULD_EQUAL(expr2->ev.arrval->size, 0)
		freeExpr(expr2);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionInt, "expression* newExpressionInt (tap_int)")
	IT("Creates a new integer expression")
		expression* expr = newExpressionInt(-100);
		SHOULD_EQUAL(expr->ev.intval, -100)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionFlo, "expression* newExpressionFlo (tap_flo)")
	IT("Creates a new floating point expression")
		expression* expr = newExpressionFlo(956.2);
		SHOULD_EQUAL(expr->ev.floval, 956.2)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionStr, "expression* newExpressionStr (string*)")
	IT("Creates a new string expression")
		expression* expr = newExpressionStr(newString(strDup("hi")));
		SHOULD_EQUAL(strcmp(expr->ev.strval->content, "hi"), 0)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionArr, "expression* newExpressionArr (array*)")
	IT("Creates a new array expression")
		expression* expr = newExpressionArr(newArray(5));
		SHOULD_EQUAL(expr->ev.arrval->size, 5)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionLaz, "expression* newExpressionLaz (expression*)")
	IT("Creates a new lazy expression")
		expression* expr = newExpressionLaz(newExpressionNil());
		SHOULD_EQUAL(expr->ev.lazval->expval->type, TYPE_NIL)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionTyp, "expression* newExpressionTyp (datatype value)")
	IT("Creates a new type expression")
		expression* expr = newExpressionTyp(TYPE_INT);
		SHOULD_EQUAL(expr->ev.intval, TYPE_INT)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionAll, "expression* newExpressionAll (datatype type, exprvals* ev, expression* next, linenum line)")
	expression* expr;
	
	IT("Creates an expression with the given expression value")
		exprvals ev;
		ev.intval = 10;
		expr = newExpressionAll(TYPE_INT, &ev, NULL, 0);
		SHOULD_EQUAL(expr->ev.intval, 10)
		freeExpr(expr);
	END_IT
	
	IT("Creates an expression with a null expression value")
		expr = newExpressionAll(TYPE_FLO, NULL, NULL, 0);
		SHOULD_EQUAL(expr->ev.floval, 0.0)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(copyExpression, "expression* copyExpression (expression* expr)")
	IT("Copies the given list of expressions and their contents")
		expression* orig1 = newExpressionFlo(5.4);
		orig1->next = newExpressionFlo(4.5);
		expression* copied1 = copyExpression(orig1);
		SHOULD_EQUAL(orig1->ev.floval, copied1->ev.floval)
		SHOULD_EQUAL(orig1->next->ev.floval, copied1->next->ev.floval)
		freeExpr(orig1);
		freeExpr(copied1);
		expression* orig2 = newExpressionLaz(newExpressionStr(newString(strDup("xyz"))));
		expression* copied2 = copyExpression(orig2);
		SHOULD_EQUAL(strcmp(orig2->ev.lazval->expval->ev.strval->content, copied2->ev.lazval->expval->ev.strval->content), 0)
		freeExpr(orig2);
		freeExpr(copied2);
	END_IT
END_DESCRIBE

DESCRIBE(copyExpressionNR, "expression* copyExpressionNR (expression* expr)")
	IT("Copies the given expression and its contents")
		expression* orig = newExpressionFlo(5.4);
		orig->next = newExpressionFlo(4.5);
		expression* copied = copyExpressionNR(orig);
		SHOULD_EQUAL(orig->ev.floval, copied->ev.floval)
		SHOULD_EQUAL(copied->next, NULL)
		freeExpr(orig);
		freeExpr(copied);
	END_IT
END_DESCRIBE

DESCRIBE(newLazyExpression, "tap_laz* newLazyExpression ()")
	IT("Creates a new lazy expression with no expression content or references")
		tap_laz* laz = newLazyExpression();
		SHOULD_EQUAL(laz->expval, NULL)
		SHOULD_EQUAL(laz->refs, NULL)
		freeLaz(laz);
	END_IT
END_DESCRIBE

DESCRIBE(newString, "string* newString (char* content)")
	IT("Creates a new string expression with the correct content and size")
		string* str = newString(strDup("testing"));
		SHOULD_EQUAL(strcmp(str->content, "testing"), 0)
		SHOULD_EQUAL(str->size, 7)
		freeStr(str);
	END_IT
END_DESCRIBE

DESCRIBE(newArray, "array* newArray (int size)")
	IT("Creates a new array expression with the correct size")
		array* arr = newArray(5);
		SHOULD_EQUAL(arr->size, 5)
		freeArr(arr);
	END_IT
END_DESCRIBE

DESCRIBE(copyArray, "array* copyArray (array* arr)")
	IT("Copies the given array, referencing the contents")
		array* arr1 = newArray(3);
		arr1->content[0] = newExpressionInt(1);
		arr1->content[1] = newExpressionStr(newString(strDup("2")));
		arr1->content[2] = newExpressionTyp(TYPE_LAZ);
		array* arr2 = copyArray(arr1);
		SHOULD_EQUAL(arr1->size, arr2->size)
		SHOULD_EQUAL(arr1->content[0], arr2->content[0])
		SHOULD_EQUAL(arr1->content[1], arr2->content[1])
		SHOULD_EQUAL(arr1->content[2], arr2->content[2])
		freeArr(arr1);
		free(arr2);
	END_IT
END_DESCRIBE

DESCRIBE(copyArrayDeep, "array* copyArrayDeep (array* arr)")
	IT("Copies the given array, copying the contents")
		array* arr1 = newArray(3);
		arr1->content[0] = newExpressionInt(1);
		arr1->content[1] = newExpressionStr(newString(strDup("2")));
		arr1->content[2] = newExpressionTyp(TYPE_LAZ);
		array* arr2 = copyArrayDeep(arr1);
		SHOULD_EQUAL(arr1->size, arr2->size)
		SHOULD_NOT_EQUAL(arr1->content[0], arr2->content[0])
		SHOULD_NOT_EQUAL(arr1->content[1], arr2->content[1])
		SHOULD_NOT_EQUAL(arr1->content[2], arr2->content[2])
		SHOULD_EQUAL(arr1->content[0]->ev.intval, arr2->content[0]->ev.intval)
		SHOULD_EQUAL(strcmp(arr1->content[1]->ev.strval->content, arr2->content[1]->ev.strval->content), 0)
		SHOULD_EQUAL(arr1->content[2]->ev.intval, arr2->content[2]->ev.intval)
		freeArr(arr1);
		freeArr(arr2);
	END_IT
END_DESCRIBE

DESCRIBE(newDate, "date newDate (string* str)")
	string* str1;
	string* str2;
	string* str3;
	
	IT("Returns the current time when given 'now'")
		str1 = newString(strDup("now"));
		SHOULD_EQUAL(newDate(str1), time(NULL))
		freeStr(str1);
	END_IT
	
	IT("Parses date strings")
		str1 = newString(strDup("10/3/1999"));
		SHOULD_EQUAL(newDate(str1), 938908800)
		freeStr(str1);
		str2 = newString(strDup(" 5/1/2004"));
		SHOULD_EQUAL(newDate(str2), 1083369600)
		freeStr(str2);
		str3 = newString(strDup("12/13/14 "));
		SHOULD_EQUAL(newDate(str3), -1737331200)
		freeStr(str3);
	END_IT
	
	IT("Parses date and time strings")
		str1 = newString(strDup("3/24/1988 5:42am"));
		SHOULD_EQUAL(newDate(str1), 575185320)
		freeStr(str1);
		str2 = newString(strDup("10/11/2000   15:12:34"));
		SHOULD_EQUAL(newDate(str2), 971277154)
		freeStr(str2);
	END_IT
END_DESCRIBE

DESCRIBE(newObject, "tap_obj* newObject (datatype type, property* props)")
	IT("Creates a new object with the given type and properties")
		expression* expr = newExpressionInt(5);
		tap_obj* obj = newObject(TYPE_OBJ, newProperty("x", newTypelist(TYPE_INT), PROP_PRIVACY_PUBLIC, PROP_RANGE_LOCAL, expr));
		SHOULD_EQUAL(obj->type, TYPE_OBJ)
		SHOULD_EQUAL(strcmp(obj->props->name, "x"), 0)
		freeExpr(expr);
		freeObj(obj);
	END_IT
END_DESCRIBE

DESCRIBE(copyObject, "tap_obj* copyObject (tap_obj* obj)")
	IT("Copies the given object, copying its type and properties")
		expression* expr = newExpressionFlo(-9.8);
		tap_obj* obj1 = newObject(TYPE_OBJ, newProperty("y", newTypelist(TYPE_FLO), PROP_PRIVACY_PRIVATE, PROP_RANGE_GLOBAL, expr));
		tap_obj* obj2 = copyObject(obj1);
		SHOULD_NOT_EQUAL(obj1, obj2)
		SHOULD_EQUAL(obj1->type, obj2->type)
		SHOULD_NOT_EQUAL(obj1->props, obj2->props)
		SHOULD_NOT_EQUAL(obj1->props->name, obj2->props->name)
		SHOULD_EQUAL(strcmp(obj1->props->name, obj2->props->name), 0)
		SHOULD_EQUAL(obj1->props->value->ev.intval, obj2->props->value->ev.intval)
		freeExpr(expr);
		freeObj(obj1);
		freeObj(obj2);
	END_IT
END_DESCRIBE

DESCRIBE(newType, "type* newType (datatype id, char* name, stringlist* required, typelist* inherits, property* properties)")
	IT("Creates a new composite type")
		expression* expr = newExpressionNil();
		string* str = newString(strDup("fun1"));
		type* comptyp = newType(TYPE_COMP_START, "comp", newStringlist(str, NULL), newTypelist(TYPE_OBJ), newProperty("prop1", newTypelist(TYPE_NIL), PROP_PRIVACY_PRIVATE, PROP_RANGE_LOCAL, expr));
		SHOULD_EQUAL(strcmp(comptyp->name, "comp"), 0)
		SHOULD_EQUAL(strcmp(comptyp->required->str->content, "fun1"), 0)
		SHOULD_EQUAL(comptyp->inherits->type, TYPE_OBJ)
		SHOULD_EQUAL(comptyp->properties->value->type, TYPE_NIL)
		freeExpr(expr);
		freeStr(str);
		freeCompTyp(comptyp);
	END_IT
END_DESCRIBE

DESCRIBE(newProperty, "property* newProperty (char* name, typelist* types, int privacy, int range, expression* value)")
	IT("Creates a new property")
		expression* expr = newExpressionStr(newString(strDup("hi")));
		property* prop = newProperty("sample-prop", newTypelist(TYPE_STR), PROP_PRIVACY_PRIVATE, PROP_RANGE_GLOBAL, expr);
		SHOULD_EQUAL(strcmp(prop->name, "sample-prop"), 0)
		SHOULD_EQUAL(prop->privacy & PROP_PRIVACY_PRIVATE, PROP_PRIVACY_PRIVATE)
		SHOULD_EQUAL(prop->range & PROP_RANGE_GLOBAL, PROP_RANGE_GLOBAL) 
		SHOULD_EQUAL(strcmp(prop->value->ev.strval->content, "hi"), 0)
		freeExpr(expr);
		freeProp(prop);
	END_IT
END_DESCRIBE

DESCRIBE(copyProperty, "property* copyProperty (property* prop)")
	IT("Copies the given property and its content")
		expression* expr = newExpressionInt(101);
		property* prop1 = newProperty("prop1", newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), PROP_PRIVACY_PUBLIC, PROP_RANGE_GLOBAL, expr);
		property* prop2 = copyProperty(prop1);
		SHOULD_NOT_EQUAL(prop1, prop2)
		SHOULD_EQUAL(strcmp(prop1->name, prop2->name), 0)
		SHOULD_NOT_EQUAL(prop1->types, prop2->types)
		SHOULD_EQUAL(prop1->types->type, prop2->types->type)
		SHOULD_EQUAL(prop1->types->next->type, prop2->types->next->type)
		SHOULD_EQUAL(prop1->privacy, prop2->privacy)
		SHOULD_EQUAL(prop1->range, prop2->range)
		SHOULD_NOT_EQUAL(prop1->value, prop2->value)
		SHOULD_EQUAL(prop1->value->ev.intval, prop2->value->ev.intval)
		freeExpr(expr);
		freeProp(prop1);
		freeProp(prop2);
	END_IT
END_DESCRIBE

DESCRIBE(newTapFunction, "tap_fun* newTapFunction (argument* args[], int minargs, int maxargs, expression* body)")
	IT("Creates a new tap function")
		argument* args[2];
		args[0] = newArgument(newString(strDup("x")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		args[1] = newArgument(newString(strDup("y")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		expression* body = newExpressionStr(newString(strDup("+")));
		body->next = newExpressionStr(newString(strDup("x")));
		body->next->next = newExpressionStr(newString(strDup("y")));
		body = newExpressionLaz(body);
		tap_fun* fun = newTapFunction(args, 2, 2, body);
		SHOULD_EQUAL(fun->args[0], args[0])
		SHOULD_EQUAL(fun->args[1], args[1])
		SHOULD_EQUAL(fun->minargs, 2)
		SHOULD_EQUAL(fun->maxargs, 2)
		SHOULD_EQUAL(fun->body, body)
		freeFun(fun);
	END_IT
END_DESCRIBE

DESCRIBE(newArgument, "argument* newArgument (string* name, typelist* types, expression* initial)")
	IT("Creates a new argument")
		expression* expr = newExpressionNil();
		argument* arg = newArgument(newString(strDup("arg")), newTypelist(TYPE_NIL), expr);
		SHOULD_EQUAL(strcmp(arg->name->content, "arg"), 0)
		SHOULD_EQUAL(arg->types->type, TYPE_NIL)
		SHOULD_EQUAL(arg->initial, expr)
		freeArg(arg);
	END_IT
END_DESCRIBE

DESCRIBE(copyArgument, "argument* copyArgument (argument* arg)")
	IT("Copies the given argument and its content")
		expression* expr = newExpressionInt(1);
		argument* arg1 = newArgument(newString(strDup("name")), newTypelist(TYPE_INT), expr);
		argument* arg2 = copyArgument(arg1);
		SHOULD_NOT_EQUAL(arg1, arg2)
		SHOULD_EQUAL(strcmp(arg1->name->content, arg2->name->content), 0)
		SHOULD_NOT_EQUAL(arg1->types, arg2->types)
		SHOULD_EQUAL(arg1->types->type, arg2->types->type)
		SHOULD_NOT_EQUAL(arg1->initial, arg2->initial)
		SHOULD_EQUAL(arg1->initial->ev.intval, arg2->initial->ev.intval)
		freeArg(arg1);
		freeArg(arg2);
	END_IT
END_DESCRIBE

DESCRIBE(newTypelist, "typelist* newTypelist (datatype type)")
	IT("Creates a new list of types of length 1")
		typelist* tl = newTypelist(TYPE_INT);
		SHOULD_EQUAL(tl->type, TYPE_INT)
		freeTypelist(tl);
	END_IT
END_DESCRIBE

DESCRIBE(newTypelistWithNext, "typelist* newTypelistWithNext (datatype type, typelist* next)")
	IT("Creates a new list of types")
		typelist* tl = newTypelistWithNext(TYPE_INT, newTypelistWithNext(TYPE_FLO, newTypelist(TYPE_ARR)));
		SHOULD_EQUAL(tl->type, TYPE_INT)
		SHOULD_EQUAL(tl->next->type, TYPE_FLO)
		SHOULD_EQUAL(tl->next->next->type, TYPE_ARR)
		freeTypelist(tl);
	END_IT
END_DESCRIBE

DESCRIBE(copyTypelist, "typelist* copyTypelist (typelist* tl)")
	IT("Copies the first item in the given list of types and its content")
		typelist* tl1 = newTypelist(TYPE_NIL);
		typelist* tl2 = newTypelistWithNext(TYPE_INT, tl1);
		typelist* tl3 = copyTypelist(tl2);
		SHOULD_NOT_EQUAL(tl2, tl3)
		SHOULD_EQUAL(tl3->type, TYPE_INT)
		SHOULD_EQUAL(tl3->next, NULL)
		freeTypelist(tl2);
		freeTypelist(tl3);
	END_IT
END_DESCRIBE

DESCRIBE(copyTypelistDeep, "typelist* copyTypelistDeep (typelist* tl)")
	IT("Copies the given list of types and their contents")
		typelist* tl1 = newTypelistWithNext(TYPE_STR, newTypelist(TYPE_OBJ));
		typelist* tl2 = copyTypelistDeep(tl1);
		SHOULD_NOT_EQUAL(tl1, tl2)
		SHOULD_EQUAL(tl1->type, tl2->type)
		SHOULD_EQUAL(tl1->next->type, tl2->next->type)
		freeTypelist(tl1);
		freeTypelist(tl2);
	END_IT
END_DESCRIBE

DESCRIBE(newTypedefs, "typedefs* newTypedefs (type* typ)")
	IT("Creates a new list of composite type definitions")
		string* str = newString(strDup("bloog"));
		expression* expr = newExpressionInt(8);
		type* typ = newType(TYPE_COMP_START, "blah", newStringlist(str, NULL), newTypelist(TYPE_OBJ), newProperty("prop1", newTypelist(TYPE_INT), PROP_PRIVACY_PRIVATE, PROP_RANGE_LOCAL, expr));
		typedefs* td = newTypedefs(typ);
		SHOULD_EQUAL(td->type, typ)
		freeStr(str);
		freeExpr(expr);
		freeTypedefs(td);
	END_IT
END_DESCRIBE

DESCRIBE(newExprstack, "exprstack* newExprstack (exprstack* current)")
	IT("Creates a new stack of expressions, adding to the given stack")
		expression* expr1 = newExpressionInt(1);
		expression* expr2 = newExpressionInt(2);
		exprstack* es1 = newExprstack(NULL);
		es1->expr = expr1;
		exprstack* es2 = newExprstack(es1);
		es2->expr = expr2;
		SHOULD_EQUAL(es1->expr, expr1)
		SHOULD_EQUAL(es1->next, NULL)
		SHOULD_EQUAL(es2->expr, expr2)
		SHOULD_EQUAL(es2->next, es1)
		freeExprstack(es2);
	END_IT
END_DESCRIBE

DESCRIBE(newPrimFunction, "tap_prim_fun* newPrimFunction (void(*address)(expression*[], int, exprvals*, datatype*), int minargs, int maxargs, typelist* types)")
	IT("Creates a new primitive tap function")
		tap_prim_fun* fun = newPrimFunction(prim_iAdd, 1, ARGLEN_INF, newTypelist(TYPE_INT));
		SHOULD_EQUAL(fun->address, prim_iAdd)
		SHOULD_EQUAL(fun->minargs, 1)
		SHOULD_EQUAL(fun->maxargs, ARGLEN_INF)
		SHOULD_EQUAL(fun->types->type, TYPE_INT)
		freePrimFun(fun);
	END_IT
END_DESCRIBE

DESCRIBE(newEnvironment, "environment* newEnvironment (hashtable* variables, int parent)")
	IT("Creates a new environment")
		environment* env = newEnvironment(newHashtable(100), 0);
		SHOULD_NOT_EQUAL(env->variables, NULL)
		insertPrimHash(env->variables, "+", newPrimFunction(&prim_iAdd, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
		SHOULD_EQUAL(env->types, NULL)
		freeEnv(env);
	END_IT
END_DESCRIBE

DESCRIBE(newStringlist, "newStringlist (string* str, stringlist* next)")
	IT("Creates a new list of strings")
		string* str1 = newString(strDup("a"));
		string* str2 = newString(strDup("b"));
		string* str3 = newString(strDup("c"));
		stringlist* sl = newStringlist(str1, newStringlist(str2, newStringlist(str3, NULL)));
		SHOULD_EQUAL(strcmp(sl->str->content, "a"), 0)
		SHOULD_EQUAL(strcmp(sl->next->str->content, "b"), 0)
		SHOULD_EQUAL(strcmp(sl->next->next->str->content, "c"), 0)
		freeStr(str1);
		freeStr(str2);
		freeStr(str3);
		freeStringlist(sl);
	END_IT
END_DESCRIBE

DESCRIBE(newErrorlist, "newErrorlist (uint code, string* message, linenum line, uint index)")
	IT("Creates a new list of errors")
		string* message = newString(strDup("An error occurred"));
		errorlist* el = newErrorlist(ERR_GENERAL, message, 1, 1);
		SHOULD_EQUAL(el->code, ERR_GENERAL)
		SHOULD_EQUAL(strcmp(el->message->content, "An error occurred"), 0)
		SHOULD_EQUAL(el->line, 1)
		SHOULD_EQUAL(el->index, 1)
		freeStr(message);
		freeErrorlist(el);
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(newExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionOfType), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionInt), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionFlo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionStr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionArr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionLaz), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionTyp), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionAll), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyExpressionNR), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newLazyExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newString), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newArray), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyArray), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyArrayDeep), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newDate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newObject), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyObject), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newType), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newProperty), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyProperty), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newTapFunction), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newArgument), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyArgument), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newTypelist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newTypelistWithNext), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyTypelist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyTypelistDeep), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newTypedefs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExprstack), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newPrimFunction), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newEnvironment), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newStringlist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newErrorlist), CSpec_NewOutputUnit());
	
	return 0;
}

