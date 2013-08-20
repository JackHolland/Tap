/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   memory_test.c
    @brief  Tests for memory.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../memory.h"
#include "../constants.h"
#include "../constructors.h"
#include "../strings.h"
#include "../../primitives/prim_int.h"

DESCRIBE(allocate, "void* allocate (size_t size)")
	IT("Returns the address of the newly allocated memory")
		void* memory = allocate(8);
		SHOULD_NOT_EQUAL(memory, 0)
		free(memory);
	END_IT
END_DESCRIBE

DESCRIBE(freeExpr, "bool freeExpr (expression* expr)")
	IT("Frees the given expression and all its children and dependent siblings")
		expression* expr = newExpressionLaz(newExpressionInt(5));
		SHOULD_EQUAL(freeExpr(expr), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeExprNR, "bool freeExprNR (expression* expr)")
	IT("Frees the given expression but its children or dependent siblings")
		expression* expr1 = newExpressionLaz(newExpressionFlo(5.5));
		expression* expr2 = newExpressionInt(5);
		expr1->next = expr2;
		SHOULD_EQUAL(freeExprNR(expr1), 0)
		SHOULD_EQUAL(freeExpr(expr2), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeLaz, "bool freeLaz (tap_laz* laz)")
	IT("Frees the given lazy expression and its content")
		tap_laz* laz = newLazyExpression();
		laz->expval = newExpressionInt(3);
		SHOULD_EQUAL(freeLaz(laz), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeStr, "bool freeStr (string* str)")
	IT("Frees the given string and its content")
		string* str = newString(strDup("abcd"));
		SHOULD_EQUAL(freeStr(str), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeArr, "bool freeArr (array* arr)")
	IT("Frees the given array and its content")
		array* arr = newArray(4);
		SHOULD_EQUAL(freeArr(arr), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeObj, "bool freeObj (tap_obj* obj)")
	IT("Frees the given object and its content")
		expression* expr = newExpressionFlo(10.0);
		tap_obj* obj = newObject(TYPE_OBJ, newProperty("test", newTypelist(TYPE_FUN), PROP_PRIVACY_PUBLIC, PROP_RANGE_GLOBAL, expr));
		SHOULD_EQUAL(freeObj(obj), 0)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(freeProp, "bool freeProp (property* prop)")
	IT("Frees the given property and its content")
		expression* expr = newExpressionInt(10);
		property* prop = newProperty("prop", newTypelist(TYPE_INT), PROP_PRIVACY_PUBLIC, PROP_RANGE_GLOBAL, expr);
		SHOULD_EQUAL(freeProp(prop), 0)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(freeFun, "bool freeFun (tap_fun* fun)")
	IT("Frees the given tap function and its content")
		argument* args[2];
		args[0] = newArgument(newString(strDup("a")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		args[1] = newArgument(newString(strDup("b")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		expression* body = newExpressionStr(newString(strDup("*")));
		body->next = newExpressionStr(newString(strDup("a")));
		body->next->next = newExpressionStr(newString(strDup("b")));
		body = newExpressionLaz(body);
		tap_fun* fun = newTapFunction(args, 2, 2, body);
		SHOULD_EQUAL(freeFun(fun), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeArg, "bool freeArg (argument* arg)")
	IT("Frees the given argument and its content")
		argument* arg1 = newArgument(newString(strDup("var")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_LAZ)), NULL);
		argument* arg2 = newArgument(newString(strDup("initialized")), newTypelist(TYPE_STR), newExpressionStr(newString(strDup("hi"))));
		SHOULD_EQUAL(freeArg(arg1), 0)
		SHOULD_EQUAL(freeArg(arg2), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeTypelist, "bool freeTypelist(typelist*)")
	IT("Frees the given list of types and their content")
		typelist* tl = newTypelistWithNext(TYPE_NIL, newTypelist(TYPE_INT));
		SHOULD_EQUAL(freeTypelist(tl), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeCompTyp, "bool freeCompTyp(type*)")
	IT("Frees the given composite type and its content")
		string* str = newString(strDup("required-fun"));
		expression* expr = newExpressionInt(0);
		type* comptyp = newType(TYPE_COMP_START, "comp", newStringlist(str, NULL), newTypelist(TYPE_OBJ), newProperty("prop", newTypelist(TYPE_NIL), PROP_PRIVACY_PRIVATE, PROP_RANGE_LOCAL, expr));
		SHOULD_EQUAL(freeCompTyp(comptyp), 0)
		freeStr(str);
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(freeTypedefs, "bool freeTypedefs(typedefs*)")
	IT("Frees the given list of type definitions and its content")
		string* str1 = newString(strDup("req1"));
		string* str2 = newString(strDup("req2"));
		expression* expr1 = newExpressionStr(newString(strDup("test1")));
		expression* expr2 = newExpressionStr(newString(strDup("test2")));
		type* typ1 = newType(TYPE_COMP_START, "type1", newStringlist(str1, NULL), newTypelist(TYPE_OBJ), newProperty("prop1", newTypelist(TYPE_STR), PROP_PRIVACY_PRIVATE, PROP_RANGE_GLOBAL, expr1));
		type* typ2 = newType(TYPE_COMP_START, "type2", newStringlist(str2, NULL), newTypelist(TYPE_OBJ), newProperty("prop2", newTypelist(TYPE_STR), PROP_PRIVACY_PUBLIC, PROP_RANGE_LOCAL, expr2));
		typedefs* td1 = newTypedefs(typ1);
		td1->next = newTypedefs(typ2);
		SHOULD_EQUAL(freeTypedefs(td1), 0)
		freeStr(str1);
		freeStr(str2);
		freeExpr(expr1);
		freeExpr(expr2);
	END_IT
END_DESCRIBE

DESCRIBE(freeExprstack, "bool freeExprstack(exprstack*)")
	IT("Frees the given stack of expressions and its content")
		exprstack* es1 = newExprstack(NULL);
		es1->expr = newExpressionNil();
		exprstack* es2 = newExprstack(es1);
		es2->expr = newExpressionStr(newString(strDup("expr")));
		SHOULD_EQUAL(freeExprstack(es2), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freePrimFun, "bool freePrimFun(tap_prim_fun*)")
	IT("Frees the given primitive function and its content")
		tap_prim_fun* fun = newPrimFunction(prim_iAdd, 1, ARGLEN_INF, newTypelist(TYPE_INT));
		SHOULD_EQUAL(freePrimFun(fun), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeEnv, "bool freeEnv(environment*)")
	IT("Frees the given environment and its content")
		environment* env = newEnvironment(newHashtable(1000), 0);
		insertPrimHash(env->variables, "+", newPrimFunction(&prim_iAdd, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
		SHOULD_EQUAL(freeEnv(env), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeStringlist, "bool freeStringlist(stringlist*)")
	IT("Frees the given list of strings and its content")
		string* str1 = newString(strDup("a"));
		string* str2 = newString(strDup("b"));
		stringlist* sl = newStringlist(str1, newStringlist(str2, NULL));
		SHOULD_EQUAL(freeStringlist(sl), 0)
		freeStr(str1);
		freeStr(str2);
	END_IT
END_DESCRIBE

DESCRIBE(freeErrorlist, "bool freeErrorlist(errorlist*)")
	IT("Frees the given list of errors and its content")
		string* message1 = newString(strDup("ERROR"));
		string* message2 = newString(strDup("ERROR AGAIN"));
		errorlist* el = newErrorlist(ERR_GENERAL, message1, 2, 10);
		el->next = newErrorlist(ERR_UNCLOSED_PAREN, message2, 3, 0);
		SHOULD_EQUAL(freeErrorlist(el), 0);
		freeStr(message1);
		freeStr(message2);
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(allocate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExpr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExprNR), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeLaz), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeStr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeArr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeObj), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeProp), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeArg), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeTypelist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeCompTyp), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeTypedefs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExprstack), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freePrimFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeEnv), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeStringlist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeErrorlist), CSpec_NewOutputUnit());
	
	return 0;
}

