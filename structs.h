/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   structs.h
    @brief  All of the structs used globally in the program that don't have dependencies (dep_structs.h contains those)
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include "typedefs.h"

typedef union exprvals_ exprvals;
typedef struct expression_ expression;
typedef struct lazyexpr_ lazyexpr;
typedef struct string_ string;
typedef struct array_ array;
typedef struct object_ object;
typedef struct type_ type;
typedef struct typedefs_ typedefs;
typedef struct property_ property;
typedef struct userfunction_ userfunction;
typedef struct argument_ argument;
typedef struct typelist_ typelist;
typedef struct expressionstack_ expressionstack;
typedef struct primfunction_ primfunction;
typedef struct stringlist_ stringlist;
typedef struct errorlist_ errorlist;

union exprvals_ {
    expression* expval;
    lazyexpr* lazval;
    long intval;
    double floval;
    string* strval;
    array* arrval;
    date datval;
    object* objval;
    userfunction* funval;
};

struct expression_ {
    datatype type;
    exprvals ev;
    expression* next;
    linenum line;
    int flag:2;
    uint isref:1;
    uint refs;
};

struct lazyexpr_ {
    expression* expval;
    expressionstack* refs;
};

struct string_ {
    char* content;
    int size;
};

struct array_ {
    int size;
    int start;
    int end;
    expression* content[0];
};

struct object_ {
    datatype type;
    property* props;
};

struct type_ {
    datatype id;
    char* name;
    stringlist* required;
    typelist* inherits;
    property* properties;
};

struct typedefs_ {
    type* type;
    typedefs* next;
};

struct property_ {
    char* name;
    typelist* types;
    int privacy:1;
    int range:1;
    expression* value;
    property* next;
};

struct userfunction_ {
    expression* body;
    int minargs;
    int maxargs;
    argument* args[0];
};

struct argument_ {
    string* name;
    typelist* types;
    expression* initial;
};

struct typelist_ {
    datatype type;
    typelist* next;
};

struct expressionstack_ {
    expression* expr;
    expressionstack* next;
};

struct primfunction_ {
    void(*address)(expression*[], int, exprvals*, datatype*);
    int minargs;
    int maxargs;
    typelist* types;
};

struct stringlist_ {
    string* str;
    stringlist* next;
};

struct errorlist_ {
    uint code;
    string* message;
    linenum line;
    uint index;
    errorlist* next;
};

#endif
