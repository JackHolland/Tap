/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors_test.c
    @brief  Tests for constructors.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../constructors.h"
#include "../constants.h"

DESCRIBE(newExpression, "expression* newExpression ()")
	IT("Creates a nil expression")
		expression* expr = newExpression();
		SHOULD_EQUAL(expr->type, TYPE_NIL)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionOfType, "expression* newExpressionOfType (datatype type)")
	IT("Creates an expression with the given type")
		expression* expr1 = newExpressionOfType(TYPE_INT);
		SHOULD_EQUAL(expr1->ev.intval, 0)
		expression* expr2 = newExpressionOfType(TYPE_ARR);
		SHOULD_EQUAL(expr2->ev.arrval->size, 0)
		freeExpr(expr1);
		freeExpr(expr2);
	END_IT
END_DESCRIBE

DESCRIBE(newExpressionInt, "expression* newExpressionInt (tap_int)")
	
END_DESCRIBE

DESCRIBE(newExpressionFlo, "expression* newExpressionFlo (tap_flo)")
	
END_DESCRIBE

DESCRIBE(newExpressionStr, "expression* newExpressionStr (string*)")
	
END_DESCRIBE

DESCRIBE(newExpressionArr, "expression* newExpressionArr (array*)")
	
END_DESCRIBE

DESCRIBE(newExpressionLaz, "expression* newExpressionLaz (expression*)")
	
END_DESCRIBE

DESCRIBE(newExpressionAll, "expression* newExpressionAll (datatype type, exprvals* ev, expression* next, linenum line)")
	
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

