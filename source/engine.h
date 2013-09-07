/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   engine.h
    @brief  The header file for engine.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "structs.h"
#include "dep_structs.h"

expression* parse(char*);
expression* parsePiece(char*, expression**, linenum, uint, int, int);
uint parseWhitespace(char*, uint, uint);
uint parseExprForSign(char*, uint, uint, expression*);
uint parseStringLiteral(char*, uint, uint);
void storeChildExpression(expression*, expression*);
expression* evaluate(expression*);
expression* evaluateExp(expression*);
expression* evaluateLaz(expression*);
expression* evaluateInt(expression*);
expression* evaluateFlo(expression*);
expression* evaluateFun(expression*);
int numArgs(expression*);
void fillArgs(expression*, expression*[], int);
int freeArgs(expression*[], int);
tap_fun_search findFunction(expression*, expression*[], int);
expression* callFun(tap_fun_search, expression*, expression*[], int);
expression* callPrimFun(tap_prim_fun*, expression*[], int);
int validFunCall(tap_fun*, expression*, expression*[], int);
expression* callTapFun(tap_fun*, expression*[], int);
expression* evaluateArr(expression*);
expression* evaluateDat(expression*);
expression* evaluateObj(expression*);
expression* evaluateTyp(expression*);
expression* evaluateCompType(expression*);
expression* evaluateArgument(expression*);
expression* expressionsToArray(expression*);
char* printExpression(expression*);
char* printErrors();
void addError(errorlist*);
char* errorCodeString(uint);
int errorCodeStringSize(uint);
void addToEnvironment(char*, expression*);
void setEnvironment();
void resetEnvironment();
expression* getVarValue(char*);
expression* getExprValue(expression*);
expression* storeExprValue(expression*, char*, int, int, expression**, int);
char* printType(datatype);
int printTypeSize(datatype);
datatype typeFromString(char*);
char* printArg(argument*);
void initializeGlobals();
void freeGlobals();

#endif

