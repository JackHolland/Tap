/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   types.h
    @brief  The header file for types.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef TYPES_H
#define TYPES_H

#include "structs.h"

int propReqsValid(stringlist*, property*);
int propTypeValid(property*, expression*);

#endif
