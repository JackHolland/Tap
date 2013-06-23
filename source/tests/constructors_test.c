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
	IT("Copies the given expression and its contents")
		expression* orig1 = newExpressionFlo(5.4);
		expression* copied1 = copyExpression(orig1);
		SHOULD_EQUAL(orig1->ev.floval, copied1->ev.floval)
		freeExpr(orig1);
		freeExpr(copied1);
		expression* orig2 = newExpressionLaz(newExpressionStr(newString(strDup("xyz"))));
		expression* copied2 = copyExpression(orig2);
		SHOULD_EQUAL(strcmp(orig2->ev.lazval->expval->ev.strval->content, copied2->ev.lazval->expval->ev.strval->content), 0)
		freeExpr(orig2);
		freeExpr(copied2);
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
		array* arr2 = copyArray(arr1);
		SHOULD_EQUAL(arr1->size, arr2->size)
		SHOULD_NOT_EQUAL(arr1->content[0], arr2->content[0])
		SHOULD_NOT_EQUAL(arr1->content[1], arr2->content[1])
		SHOULD_NOT_EQUAL(arr1->content[2], arr2->content[2])
		SHOULD_EQUAL(arr1->content[0]->ev.intval, arr2->content[0]->ev.intval)
		SHOULD_EQUAL(strcmp(arr1->content[1]->ev.strval->content, arr2->content[1]->ev.strval->content), 0)
		SHOULD_EQUAL(arr1->content[2]->ev.intval, arr2->content[2]->ev.intval)
		freeArr(arr1);
		free(arr2);
	END_IT
END_DESCRIBE

DESCRIBE(newDate, "date newDate (string* str)")
	date dat;
	
	IT("Returns the current time when given 'now'")
		dat = newDate("now");
		SHOULD_EQUAL(dat, time(NULL))
	END_IT
	
	IT("Parses date strings")
		
	END_IT
	
	IT("Parses date and time strings")
		
	END_IT
END_DESCRIBE

/*DESCRIBE(, "")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(, "")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(, "")
	IT("")
		
	END_IT
END_DESCRIBE*/

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
	CSpec_Run(DESCRIPTION(newLazyExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newString), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newArray), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyArray), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(copyArrayDeep), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newDate), CSpec_NewOutputUnit());
	/*CSpec_Run(DESCRIPTION(newObject), CSpec_NewOutputUnit());
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
	CSpec_Run(DESCRIPTION(copyTypelists), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newTypedefs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionstack), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newPrimFunction), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newEnvironment), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newStringlist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newErrorlist), CSpec_NewOutputUnit());*/
	
	return 0;
}

