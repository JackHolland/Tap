/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   strings.h
    @brief  The header file for strings.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef STRINGS_H
#define STRINGS_H

#include "structs.h"

#define CASE_UPPER 0
#define CASE_LOWER 1
#define CASE_SENTENCE 2
#define CASE_TITLE 3

int indexOf(char*, char, uint);
char* substr(char*, uint, uint);
uint safeEdit(char*, uint, char, uint);
string* changeCase(string*, int);
string* copyString(string*);
char* strDup(char*);
int isNumber(char);
int stringSize(int);

#endif
