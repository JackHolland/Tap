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

DESCRIBE(newExpression, "expression* newExpression ()")
	
END_DESCRIBE

DESCRIBE(newExpression_t, "expression* newExpression_t (datatype type)")
	
END_DESCRIBE

DESCRIBE(newExpression_int, "expression* newExpression_int (tap_int)")
	
END_DESCRIBE

DESCRIBE(newExpression_flo, "expression* newExpression_flo (tap_flo)")
	
END_DESCRIBE

DESCRIBE(newExpression_str, "expression* newExpression_str (string*)")
	
END_DESCRIBE

DESCRIBE(newExpression_arr, "expression* newExpression_arr (array*)")
	
END_DESCRIBE

DESCRIBE(newExpression_laz, "expression* newExpression_laz (expression*)")
	
END_DESCRIBE

DESCRIBE(newExpression_all, "expression* newExpression_all (datatype type, exprvals* ev, expression* next, linenum line)")
	
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(newExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_t), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_int), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_flo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_str), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_arr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_laz), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(newExpression_all), CSpec_NewOutputUnit());
	
	return 0;
}
