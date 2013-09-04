/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   arrays_test.c
    @brief  Tests for arrays.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "../../testing/cspec.h"
#include "../../testing/cspec_output_unit.h"

#include "../engine.h"
#include "../constants.h"
#include "../constructors.h"
#include "../strings.h"

extern errorlist* cerror;

DESCRIBE(parse, "expression* parse (char* text)")
	expression* result;
	expression* child1;
	expression* child2;
	expression* child3;
	
	IT("Parses simple expressions")
		result = parse(NULL);
		SHOULD_EQUAL(result, NULL)
		result = parse("");
		SHOULD_EQUAL(result->type, TYPE_NIL)
		SHOULD_EQUAL(result->next, NULL)
		freeExpr(result);
		result = parse("(5)");
		SHOULD_EQUAL(result->type, TYPE_EXP)
		SHOULD_EQUAL(result->next, NULL)
		child1 = result->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 5)
		SHOULD_EQUAL(child1->next, NULL)
		freeExpr(result);
		result = parse("(+ 1 2.0)");
		SHOULD_EQUAL(result->type, TYPE_EXP)
		SHOULD_EQUAL(result->next, NULL)
		child1 = result->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child1->ev.strval->content, "+"), 0)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 1)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_FLO)
		SHOULD_EQUAL(child1->ev.floval, 2.0)
		SHOULD_EQUAL(child1->next, NULL)
		freeExpr(result);
	END_IT
	
	IT("Parses nested expressions")
		result = parse("(* (+ 4 -1) 2 (- 3.4))");
		SHOULD_EQUAL(result->type, TYPE_EXP)
		SHOULD_EQUAL(result->next, NULL)
		child1 = result->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child1->ev.strval->content, "*"), 0)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_EXP)
		child2 = child1->ev.expval;
		SHOULD_EQUAL(child2->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child2->ev.strval->content, "+"), 0)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_INT)
		SHOULD_EQUAL(child2->ev.intval, 4)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_INT)
		SHOULD_EQUAL(child2->ev.intval, -1)
		SHOULD_EQUAL(child2->next, NULL)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 2)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_EXP)
		SHOULD_EQUAL(child1->next, NULL)
		child1 = child1->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child1->ev.strval->content, "-"), 0)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_FLO)
		SHOULD_EQUAL(child1->ev.floval, 3.4)
		SHOULD_EQUAL(child1->next, NULL)
		freeExpr(result);
	END_IT
	
	IT("Parses lazy expressions")
		result = parse("(&& [1] [(|| 1 0)])");
		SHOULD_EQUAL(result->type, TYPE_EXP)
		SHOULD_EQUAL(result->next, NULL)
		child1 = result->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child1->ev.strval->content, "&&"), 0)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_LAZ)
		child2 = child1->ev.lazval->expval;
		SHOULD_EQUAL(child2->type, TYPE_INT)
		SHOULD_EQUAL(child2->ev.intval, 1)
		SHOULD_EQUAL(child2->next, NULL)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_LAZ)
		child1 = child1->ev.lazval->expval;
		SHOULD_EQUAL(child1->type, TYPE_EXP)
		SHOULD_EQUAL(child1->next, NULL)
		child1 = child1->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child1->ev.strval->content, "||"), 0)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 1)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 0)
		SHOULD_EQUAL(child2->next, NULL)
		freeExpr(result);
	END_IT
	
	IT("Parses anonymous functions")
		result = parse("((function [x] [(* x 2)]) 4)");
		SHOULD_EQUAL(result->type, TYPE_EXP)
		SHOULD_EQUAL(result->next, NULL)
		child1 = result->ev.expval;
		SHOULD_EQUAL(child1->type, TYPE_EXP)
		child2 = child1->ev.expval;
		SHOULD_EQUAL(child2->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child2->ev.strval->content, "function"), 0)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_LAZ)
		child3 = child2->ev.lazval->expval;
		SHOULD_EQUAL(child3->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child3->ev.strval->content, "x"), 0)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_LAZ)
		child2 = child2->ev.lazval->expval;
		SHOULD_EQUAL(child2->type, TYPE_EXP)
		SHOULD_EQUAL(child2->next, NULL)
		child2 = child2->ev.expval;
		SHOULD_EQUAL(child2->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child2->ev.strval->content, "*"), 0)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(child2->ev.strval->content, "x"), 0)
		child2 = child2->next;
		SHOULD_EQUAL(child2->type, TYPE_INT)
		SHOULD_EQUAL(child2->ev.intval, 2)
		SHOULD_EQUAL(child2->next, NULL)
		child1 = child1->next;
		SHOULD_EQUAL(child1->type, TYPE_INT)
		SHOULD_EQUAL(child1->ev.intval, 4)
		SHOULD_EQUAL(child1->next, NULL)
		freeExpr(result);
	END_IT
	
	IT("Produces errors when given malformed expressions")
		initializeGlobals();
		result = parse("(");
		SHOULD_EQUAL(result->type, TYPE_NIL)
		SHOULD_EQUAL(cerror->code, ERR_UNCLOSED_PAREN)
		freeExpr(result);
		result = parse(")");
		SHOULD_EQUAL(result->type, TYPE_NIL)
		SHOULD_EQUAL(cerror->code, ERR_UNMATCHED_PAREN)
		freeExpr(result);
		result = parse("()");
		SHOULD_EQUAL(result->type, TYPE_NIL)
		SHOULD_EQUAL(cerror->code, ERR_INVALID_NUM_ARGS)
		freeExpr(result);
		result = parse("(\"x )");
		SHOULD_EQUAL(result->type, TYPE_NIL)
		SHOULD_EQUAL(cerror->code, ERR_UNCLOSED_STR_LIT)
		freeExpr(result);
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(parsePiece, "expression* parsePiece (char* text, expression** tail, linenum startline, uint startindex, int ascii, int iscont)")
	IT("Parses a parentheses-enclosed string")
		expression* tail = NULL;
		expression* result = parsePiece(strDup("+ 1 2"), &tail, 1, 1, '(', 0);
		SHOULD_EQUAL(result->type, TYPE_STR)
		SHOULD_EQUAL(strcmp(result->ev.strval->content, "+"), 0)
		expression* next = result->next;
		SHOULD_EQUAL(next->type, TYPE_INT)
		SHOULD_EQUAL(next->ev.intval, 1)
		next = next->next;
		SHOULD_EQUAL(next->type, TYPE_INT)
		SHOULD_EQUAL(next->ev.intval, 2)
		SHOULD_EQUAL(next->next, NULL)
		SHOULD_EQUAL(next, tail);
		freeExpr(result);
	END_IT
END_DESCRIBE

DESCRIBE(parseWhitespace, "uint parseWhitespace (char* text, uint index, uint end)")
	IT("Advances the given index past any whitespace found")
		uint index = parseWhitespace("hi", 0, 2);
		SHOULD_EQUAL(index, 0)
		index = parseWhitespace(" hi", 0, 3);
		SHOULD_EQUAL(index, 1)
		index = parseWhitespace("  xyz ", 1, 1);
		SHOULD_EQUAL(index, 1)
		index = parseWhitespace("        ", 2, 4);
		SHOULD_EQUAL(index, 4)
	END_IT
END_DESCRIBE

DESCRIBE(parseExprForSign, "uint parseExprForSign (char* text, uint index, uint size, expression* expr)")
	IT("Determines if the given text represents a signed number and marks the given expression as an integer")
		expression* expr = newExpressionNil();
		uint index = parseExprForSign("+5", 0, 2, expr);
		SHOULD_EQUAL(index, 1)
		SHOULD_EQUAL(expr->type, TYPE_INT)
		expr->type = TYPE_NIL;
		index = parseExprForSign(" - 2", 1, 4, expr);
		SHOULD_EQUAL(index, 1)
		SHOULD_EQUAL(expr->type, TYPE_NIL)
		freeExpr(expr);
	END_IT
END_DESCRIBE

DESCRIBE(parseStringLiteral, "uint parseStringLiteral (char* text, uint index, uint end)")
	IT("Skips past text until '\"' is found")
		uint index = parseStringLiteral("Jack\"", 0, 5);
		SHOULD_EQUAL(index, 4)
		index = parseStringLiteral("Jack\"kcaJ", 1, 9);
		SHOULD_EQUAL(index, 4)
		index = parseStringLiteral("JackkcaJ", 3, 8);
		SHOULD_EQUAL(index, 8)
	END_IT
END_DESCRIBE

DESCRIBE(storeChildExpression, "void storeChildExpression (expression* parent, expression* child)")
	IT("Stores the child expression in the parent expression")
		expression* parent = newExpressionOfType(TYPE_EXP);
		expression* child = newExpressionInt(0);
		storeChildExpression(parent, child);
		SHOULD_EQUAL(parent->ev.expval, child)
		freeExpr(parent);
		parent = newExpressionLaz(NULL);
		child = newExpressionFlo(1.0);
		storeChildExpression(parent, child);
		SHOULD_EQUAL(parent->ev.lazval->expval, child)
		freeExpr(parent);
	END_IT
END_DESCRIBE

DESCRIBE(evaluate, "expression* evaluate (expression* head)")
	IT("")
		
	END_IT
END_DESCRIBE

DESCRIBE(evaluateExp, "expression* evaluateExp (expression* expr)")
	IT("Evaluates the given container expression")
		initializeGlobals();
		expression* head = newExpressionOfType(TYPE_EXP);
		expression* child1 = newExpressionStr(newString(strDup("+")));
		head->ev.expval = child1;
		child1->flag = EFLAG_VAR;
		child1->next = newExpressionInt(1);
		child1 = child1->next;
		child1->next = newExpressionInt(1);
		expression* result = evaluateExp(head);
		SHOULD_EQUAL(result->type, TYPE_INT)
		SHOULD_EQUAL(result->ev.intval, 2)
		freeExpr(head);
		freeExpr(result);
		head = newExpressionOfType(TYPE_EXP);
		child1 = newExpressionStr(newString(strDup("*")));
		head->ev.expval = child1;
		child1->flag = EFLAG_VAR;
		child1->next = newExpressionOfType(TYPE_EXP);
		child1 = child1->next;
		expression* child2 = newExpressionStr(newString(strDup("+")));
		child1->ev.expval = child2;
		child2->flag = EFLAG_VAR;
		child2->next = newExpressionFlo(2.2);
		child2 = child2->next;
		child2->next = newExpressionFlo(2.8);
		child1->next = newExpressionInt(2);
		result = evaluateExp(head);
		SHOULD_EQUAL(result->type, TYPE_FLO)
		SHOULD_EQUAL(result->ev.floval, 10)
		freeExpr(head);
		freeExpr(result);
		freeGlobals();
	END_IT
END_DESCRIBE

DESCRIBE(evaluateLaz, "expression* evaluateArgument (expression* arg)")
	expression* expr1;
	expression* expr2;
	expression* expr3;
	expression* result;
	
	IT("Evaluates the given lazy expression")
		initializeGlobals();
		expr1 = newExpressionStr(newString(strDup("+")));
		expr1->flag = EFLAG_VAR;
		expr1->next = newExpressionInt(10);
		expr2 = expr1->next;
		expr2->next = newExpressionInt(1);
		expr1 = newExpressionLaz(expr1);
		result = evaluateLaz(expr1);
		SHOULD_EQUAL(result->type, TYPE_INT)
		SHOULD_EQUAL(result->ev.intval, 11)
		freeExpr(expr1);
		freeExpr(result);
		expr1 = newExpressionOfType(TYPE_EXP);
		expr2 = newExpressionStr(newString(strDup("*")));
		expr1->ev.expval = expr2;
		expr2->flag = EFLAG_VAR;
		expr2->next = newExpressionFlo(2);
		expr2 = expr2->next;
		expr2->next = newExpressionFlo(2.5);
		expr2 = newExpressionOfType(TYPE_EXP);
		expr1->next = expr2;
		expr3 = newExpressionStr(newString(strDup("/")));
		expr2->ev.expval = expr3;
		expr3->flag = EFLAG_VAR;
		expr3->next = newExpressionFlo(2);
		expr3 = expr3->next;
		expr3->next = newExpressionFlo(2.5);
		expr1 = newExpressionLaz(expr1);
		result = evaluateLaz(expr1);
		SHOULD_EQUAL(result->type, TYPE_FLO)
		SHOULD_EQUAL(result->ev.floval, 0.8)
		freeExpr(expr1);
		freeExpr(result);
		freeGlobals();
	END_IT
	
	IT("Evaluates the given non-lazy expression")
		expr1 = newExpressionInt(4);
		result = evaluateLaz(expr1);
		SHOULD_EQUAL(result->type, TYPE_INT)
		SHOULD_EQUAL(result->ev.intval, 4)
		freeExpr(expr1);
		freeExpr(result);
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
	/*CSpec_Run(DESCRIPTION(parse), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parsePiece), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseWhitespace), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseExprForSign), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(parseStringLiteral), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(storeChildExpression), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluate), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateExp), CSpec_NewOutputUnit());*/
	CSpec_Run(DESCRIPTION(evaluateLaz), CSpec_NewOutputUnit());
	/*CSpec_Run(DESCRIPTION(evaluateInt), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateFlo), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(numArgs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(fillArgs), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(findFunction), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(callPrimFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(validFunCall), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(callFun), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateArr), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateDat), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateObj), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateTyp), CSpec_NewOutputUnit());
	CSpec_Run(DESCRIPTION(evaluateCompType), CSpec_NewOutputUnit());
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
	CSpec_Run(DESCRIPTION(freeGlobals), CSpec_NewOutputUnit());*/
	
	return 0;
}

