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
#include "../constructors.h"
#include "../strings.h"

DESCRIBE(allocate, "void* allocate (size_t size)")
	IT("returns the address of the newly allocated memory")
		void* memory = allocate(8);
		SHOULD_NOT_EQUAL(memory, 0)
		free(memory);
	END_IT
END_DESCRIBE

DESCRIBE(freeExpr, "bool freeExpr (expression* expr)")
	IT("frees the given expression and all its children and dependent siblings")
		expression* expr = newExpressionLaz(newExpressionInt(5));
		SHOULD_EQUAL(freeExpr(expr), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeExprNR, "bool freeExprNR (expression* expr)")
	IT("frees the given expression but its children or dependent siblings")
		expression* expr1 = newExpressionInt(5);
		expression* expr2 = newExpressionLaz(expr1);
		SHOULD_EQUAL(freeExprNR(expr2), 0)
		SHOULD_EQUAL(freeExpr(expr1), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeStr, "bool freeStr (string* str)")
	IT("frees the given string and its content")
		string* str = newString(strDup("abcd"));
		SHOULD_EQUAL(freeStr(str), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeArr, "bool freeArr (array* arr)")
	IT("frees the given array and its content")
		array* arr = newArray(4);
		SHOULD_EQUAL(freeArr(arr), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeLaz, "bool freeLaz (tap_laz* laz)")
	IT("frees the given lazy expression and its content")
		tap_laz* laz = newLazyExpression();
		laz->expval = newExpressionInt(3);
		SHOULD_EQUAL(freeLaz(laz), 0)
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(allocate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExpr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExprNR), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeStr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeArr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeLaz), CSpec_NewOutputUnit());
	
	return 0;
}

