/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   string.h
    @brief  The header file for arrays.c
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef ARRAYS_H
#define ARRAYS_H

#include "structs.h"

int arrayUsedSize(array*);
array* resizeArray(array*, int, int);

#endif
