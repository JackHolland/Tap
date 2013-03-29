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
expression* evaluate (expression*);
expression* evaluateArgument (expression*);
expression* evaluateLazy(expression*);
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
