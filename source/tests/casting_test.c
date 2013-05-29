/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../casting.h"
#include "../constants.h"
#include "../constructors.h"
#include "../engine.h"
#include "../strings.h"
#include "../memory.h"

DESCRIBE(castToNum, "expression* castToNum (string* string, int base)")
	string* str1 = newString(strDup("365"));
	string* str2 = newString(strDup("87.2"));
	string* str3 = newString(strDup("1211"));
	string* str4 = newString(strDup("58a"));
	
	IT("casts base 10 numeric strings into numbers")
		expression* num1 = castToNum(str1, 10);
		SHOULD_EQUAL(num1->type, TYPE_FLO)
		SHOULD_EQUAL(num1->ev.floval, 365)
		freeExpr(num1);
		expression* num2 = castToNum(str2, 10);
		SHOULD_EQUAL(num2->type, TYPE_FLO)
		SHOULD_EQUAL(num2->ev.floval, 87.2)
		freeExpr(num2);
	END_IT
	
	IT("casts numeric strings of any base into numbers")
		expression* num1 = castToNum(str3, 8);
		SHOULD_EQUAL(num1->type, TYPE_FLO)
		SHOULD_EQUAL(num1->ev.floval, 649)
		freeExpr(num1);
		expression* num2 = castToNum(str3, 3);
		SHOULD_EQUAL(num2->type, TYPE_FLO)
		SHOULD_EQUAL(num2->ev.floval, 49)
		freeExpr(num2);
		expression* num3 = castToNum(str3, 16);
		SHOULD_EQUAL(num3->type, TYPE_FLO)
		SHOULD_EQUAL(num3->ev.floval, 4625)
		freeExpr(num3);
	END_IT
	
	IT("casts non-numeric strings into nil expressions")
		expression* num1 = castToNum(str4, 10);
		SHOULD_EQUAL(num1->type, TYPE_NIL)
		freeExpr(num1);
	END_IT
	
	freeStr(str1);
	freeStr(str2);
	freeStr(str3);
	freeStr(str4);
END_DESCRIBE

DESCRIBE(castToInt, "int castToInt (expression* expr)")
	expression* expr;
	
	IT("returns the value of integer expressions")
		expr = newExpression_int(14);
		SHOULD_EQUAL(castToInt(expr), 14)
		freeExpr(expr);
	END_IT
	
	IT("rounds floats to their nearest integer equivalents")
		expr = newExpression_flo(10.4);
		SHOULD_EQUAL(castToInt(expr), 10)
		freeExpr(expr);
		expr = newExpression_flo(-23.8);
		SHOULD_EQUAL(castToInt(expr), -24)
		freeExpr(expr);
	END_IT
	
	IT("casts integral strings to integers and non-integral strings to nil")
		expr = newExpression_str(newString(strDup("45")));
		SHOULD_EQUAL(castToInt(expr), 45)
		freeExpr(expr);
		expr = newExpression_str(newString(strDup("abc")));
		SHOULD_EQUAL(castToInt(expr), NIL)
		freeExpr(expr);
	END_IT
	
	IT("evaluates lazy expressions and tries to cast their results to integers")
		initializeGlobals();
		expr = newExpression_laz(newExpression_int(3));
		SHOULD_EQUAL(castToInt(expr), 3)
		freeExpr(expr);
		expr = newExpression_laz(newExpression_str(newString(strDup("xyz"))));
		SHOULD_EQUAL(castToInt(expr), NIL)
		freeExpr(expr);
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(castToBoo, "expression* castToBoo (expression* expr)")
	expression* expr;
	expression* result;
	
	IT("casts integers to booleans")
		expr = newExpression_int(8);
		result = castToBoo(expr);
		SHOULD_EQUAL(result->ev.intval, 1)
		freeExpr(result);
		freeExpr(expr);
		expr = newExpression_int(0);
		result = castToBoo(expr);
		SHOULD_EQUAL(result->ev.intval, 0)
		freeExpr(result);
		freeExpr(expr);
	END_IT
	
	IT("tries to cast strings to booleans")
		expr = newExpression_str(newString(strDup("1")));
		result = castToBoo(expr);
		SHOULD_EQUAL(result->ev.intval, 1)
		freeExpr(result);
		freeExpr(expr);
	END_IT
	
	IT("tries to cast lazy expressions to booleans")
		initializeGlobals();
		expr = newExpression_laz(newExpression_str(newString(strDup("-10"))));
		result = castToBoo(expr);
		SHOULD_EQUAL(result->ev.intval, 1)
		freeExpr(result);
		freeExpr(expr);
		freeGlobals();
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(castToNum), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToInt), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToBoo), CSpec_NewOutputUnit());
	
	return 0;
}

