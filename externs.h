/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   externs.h
    @brief  All of the external variables used globally in the program
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef EXTERNS_H
#define EXTERNS_H

#include "constants.h"

environment* environments[INITIAL_ENV_COUNT];
uint cenvironment;
datatype ctypeid;
errorlist* errors;
errorlist* cerror;
int monthdays[MON_IN_YEAR];

#endif
