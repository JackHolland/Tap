/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   dep_structs.h
    @brief  The structs used globally in the program that require other files (these are separated because of dependency issues)
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef DEP_STRUCTS_H
#define DEP_STRUCTS_H

#include "hashtable.h"

typedef struct environment_ environment;

struct environment_ {
    hashtable* variables;
    typedefs* types;
    int numvars;
    int parent;
};

#endif
