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
		expression* expr1 = newExpressionInt(5);
		expression* expr2 = newExpressionLaz(expr1);
		SHOULD_EQUAL(freeExprNR(expr2), 0)
		SHOULD_EQUAL(freeExpr(expr1), 0)
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
		tap_obj* obj = newObject(TYPE_OBJ, newProperty("test", newTypelist(TYPE_FUN), PROP_PRIVACY_PUBLIC, PROP_RANGE_GLOBAL, newExpressionFlo(10.0)));
		SHOULD_EQUAL(freeObj(obj), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeProp, "bool freeProp (property* prop)")
	IT("Frees the given property and its content")
		property* prop = newProperty("prop", newTypelist(TYPE_INT), PROP_PRIVACY_PUBLIC, PROP_RANGE_GLOBAL, newExpressionInt(10));
		SHOULD_EQUAL(freeProp(prop), 0)
	END_IT
END_DESCRIBE

DESCRIBE(freeFun, "bool freeFun (tap_fun* fun)")
	IT("Frees the given tap function and its content")
		/*argument* args[2];
		args[0] = newArgument(newString(strDup("a")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		args[1] = newArgument(newString(strDup("b")), newTypelistWithNext(TYPE_INT, newTypelist(TYPE_FLO)), NULL);
		tap_fun* fun = newTapFunction(args, 2, 3, newExpression*/
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
	/*CSpec_Run(DESCRIPTION(freeArg), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeTypelist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeCompTyp), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeStringlist), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeTypedefs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeExprstack), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freePrimFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeEnv), CSpec_NewOutputUnit());*/
	
	return 0;
}

