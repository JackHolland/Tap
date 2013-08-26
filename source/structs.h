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
typedef struct tap_laz_ tap_laz;
typedef struct string_ string;
typedef struct array_ array;
typedef struct tap_obj_ tap_obj;
typedef struct type_ type;
typedef struct typedefs_ typedefs;
typedef struct property_ property;
typedef struct tap_fun_ tap_fun;
typedef struct argument_ argument;
typedef struct typelist_ typelist;
typedef struct exprstack_ exprstack;
typedef struct tap_prim_fun_ tap_prim_fun;
typedef struct stringlist_ stringlist;
typedef struct errorlist_ errorlist;
typedef union tap_fun_con_ tap_fun_con;
typedef struct tap_fun_search_ tap_fun_search;

typedef long tap_int;
typedef double tap_flo;
typedef string tap_str;
typedef array tap_arr;
typedef time_t tap_dat;

union exprvals_ {
    expression* expval;
    tap_laz* lazval;
    tap_int intval;
    tap_flo floval;
    tap_str* strval;
    tap_arr* arrval;
    tap_dat datval;
    tap_obj* objval;
    tap_fun* funval;
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

struct tap_laz_ {
    expression* expval;
    exprstack* refs;
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

struct tap_obj_ {
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

struct tap_fun_ {
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

struct exprstack_ {
    expression* expr;
    exprstack* next;
};

struct tap_prim_fun_ {
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

union tap_fun_con_ {
	tap_fun* fun;
	tap_prim_fun* prim_fun;
};

struct tap_fun_search_ {
	int found:1;
	int prim:1;
	tap_fun_con funs;
};

#endif

