/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  Tests for arrays.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../arrays.h"

DESCRIBE(parse, "expression* parse (char* text)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(parsePiece, "expression* parsePiece (char* text, expression** tail, linenum startline, uint startindex, int ascii, int iscont)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(parseWhitespace, "uint parseWhitespace (char* text, uint index, uint end)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(parseExprForSign, "uint parseExprForSign (char* text, uint index, uint size, expression* expr)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(parseStringLiteral, "uint parseStringLiteral (char* text, uint index, uint end)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(storeChildExpression, "void storeChildExpression (expression* parent, expression* child)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(evaluate, "expression* evaluate (expression* head)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(evaluateArgument, "expression* evaluateArgument (expression* arg)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(evaluateLazy, "expression* evaluateLazy (expression* expr)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(printExpression, "char* printExpression (expression* result)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(printErrors, "char* printErrors ()")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(addError, "void addError (errorlist* error)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(errorCodeString, "char* errorCodeString (uint code)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(errorCodeStringSize, "int errorCodeStringSize (uint code)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(addToEnvironment, "void addToEnvironment (char* key, expression* value)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(setEnvironment, "void setEnvironment ()")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(resetEnvironment, "void resetEnvironment ()")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(getVarValue, "expression* getVarValue (char* name)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(getExprValue, "expression* getExprValue (expression* expr)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(storeExprValue, "expression* storeExprValue (expression* expr, char* text, int start, int end, expression** last, int lazy)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(printType, "char* printType (datatype typ)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(printTypeSize, "int printTypeSize (datatype typ)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(typeFromString, "datatype typeFromString (char* type)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(printArg, "char* printArg (argument* arg)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(initializeGlobals, "void initializeGlobals ()")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(freeGlobals, "void freeGlobals ()")
	IT("")
		
	END_IT
END_DESCRIBE

int main () {
	CSpec_Run(DESCRIPTION(parse), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parsePiece), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseWhitespace), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseExprForSign), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseStringLiteral), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(storeChildExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateArgument), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateLazy), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(printExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(printErrors), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(addError), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(errorCodeString), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(errorCodeStringSize), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(addToEnvironment), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(setEnvironment), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(resetEnvironment), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(getVarValue), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(getExprValue), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(storeExprValue), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(printType), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(printTypeSize), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(printArg), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(initializeGlobals), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(freeGlobals), CSpec_NewOutputUnit());
	
	return 0;
}

