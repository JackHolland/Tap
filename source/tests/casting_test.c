/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  All of the array functions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

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

DESCRIBE(castToInt, "long castToInt (expression* expr)")
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

DESCRIBE(fRound, "expression* fRound (double result, int digits)")
	expression* expr;
	
	IT("rounds floats to integers")
		expr = fRound(101.3, -1);
		SHOULD_EQUAL(expr->ev.intval, 101)
		freeExpr(expr);
		expr = fRound(34.5, -1);
		SHOULD_EQUAL(expr->ev.intval, 34)
		freeExpr(expr);
		expr = fRound(35.5, -1);
		SHOULD_EQUAL(expr->ev.intval, 36)
		freeExpr(expr);
	END_IT
	
	IT("rounds floats to arbitrary precisions")
		expr = fRound(203.45, 1);
		SHOULD_EQUAL(expr->ev.floval, 203.4)
		freeExpr(expr);
		expr = fRound(-0.5689, 3);
		SHOULD_EQUAL(expr->ev.floval, -0.569)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(castToFlo, "double castToFlo (expression* expr)")
	expression* expr;
	
	IT("returns the value of float expressions")
		expr = newExpression_flo(37.2);
		SHOULD_EQUAL(castToFlo(expr), 37.2)
		freeExpr(expr);
	END_IT
	
	IT("casts integers to their floating point equivalents")
		expr = newExpression_int(-11);
		SHOULD_EQUAL(castToFlo(expr), -11.0)
		freeExpr(expr);
	END_IT
	
	IT("casts numeric strings to floats and non-numeric strings to nil")
		expr = newExpression_str(newString(strDup("82.8")));
		SHOULD_EQUAL(castToFlo(expr), 82.8)
		freeExpr(expr);
		expr = newExpression_str(newString(strDup("abc")));
		SHOULD_EQUAL(castToFlo(expr), NIL)
		freeExpr(expr);
	END_IT
	
	IT("evaluates lazy expressions and tries to cast their results to floats")
		initializeGlobals();
		expr = newExpression_laz(newExpression_flo(-78.9));
		SHOULD_EQUAL(castToFlo(expr), -78.9)
		freeExpr(expr);
		expr = newExpression_laz(newExpression_str(newString(strDup("xyz"))));
		SHOULD_EQUAL(castToFlo(expr), NIL)
		freeExpr(expr);
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(castToStr, "string* castToStr (expression* expr)")
	expression* expr;
	string* result;
	
	IT("returns the value of string expressions")
		expr = newExpression_str(newString(strDup("abcd")));
		result = castToStr(expr);
		SHOULD_EQUAL(strcmp(result->content, expr->ev.strval->content), 0)
		freeStr(result);
		freeExpr(expr);
	END_IT
	
	IT("casts integers into their string equivalents")
		expr = newExpression_int(7);
		result = castToStr(expr);
		SHOULD_EQUAL(strcmp(result->content, "7"), 0)
		freeStr(result);
		freeExpr(expr);
	END_IT
	
	IT("casts floats into their string equivalents")
		expr = newExpression_flo(22.0);
		result = castToStr(expr);
		SHOULD_EQUAL(strcmp(result->content, "22.000000"), 0)
		freeStr(result);
		freeExpr(expr);
	END_IT
	
	IT("evaluates lazy expressions and tries to cast their results to strings")
		initializeGlobals();
		expr = newExpression_laz(newExpression_int(6));
		result = castToStr(expr);
		SHOULD_EQUAL(strcmp(result->content, "6"), 0)
		freeStr(result);
		freeExpr(expr);
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(castToStrWithBase, "string* castToStrWithBase (int intval, uint base)")
	string* result;
	
	IT("casts base 10 integers into strings")
		result = castToStrWithBase(5, 10);
		SHOULD_EQUAL(strcmp(result->content, "5"), 0)
		freeStr(result);
		result = castToStrWithBase(23, 10);
		SHOULD_EQUAL(strcmp(result->content, "23"), 0)
		freeStr(result);
		result = castToStrWithBase(-189, 10);
		SHOULD_EQUAL(strcmp(result->content, "-189"), 0)
		freeStr(result);
	END_IT
	
	IT("casts integers in any base into strings")
		result = castToStrWithBase(5, 2);
		SHOULD_EQUAL(strcmp(result->content, "101"), 0)
		freeStr(result);
		result = castToStrWithBase(23, 16);
		SHOULD_EQUAL(strcmp(result->content, "17"), 0)
		freeStr(result);
		result = castToStrWithBase(-189, 5);
		SHOULD_EQUAL(strcmp(result->content, "-1224"), 0)
		freeStr(result);
	END_IT
END_DESCRIBE

DESCRIBE(alphaNumeric, "uint alphaNumeric (uint number)")
	IT("returns '0' to '9' unmodified")
		SHOULD_EQUAL(alphaNumeric('0'), '0')
		SHOULD_EQUAL(alphaNumeric('9'), '9')
	END_IT
	
	IT("returns the appropriate character for values above '9')")
		SHOULD_EQUAL(alphaNumeric('9' + 1), 'A')
		SHOULD_EQUAL(alphaNumeric('8' + 3), 'B')
	END_IT
END_DESCRIBE

DESCRIBE(castToArr, "array* castToArr (expression* expr)")
	expression* expr;
	array* result;
	
	IT("returns the value of array expressions")
		expr = newExpression_arr(newArray(3));
		result = castToArr(expr);
		SHOULD_EQUAL(result->size, 3)
		freeExpr(expr);
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(castToNum), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToInt), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToBoo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(fRound), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToFlo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToStr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToStrWithBase), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(alphaNumeric), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(castToArr), CSpec_NewOutputUnit());
	
	return 0;
}

