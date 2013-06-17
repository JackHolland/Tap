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

int main () {
	CSpec_Run(DESCRIPTION(newExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionOfType), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionInt), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionFlo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionStr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionArr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionLaz), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpressionAll), CSpec_NewOutputUnit());
	
	return 0;
}

