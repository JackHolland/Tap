/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constants.h
    @brief  All of the constants used globally in the program
    (C) 2011 Jack Holland. All rights reserved.
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define NIL 0 // nil/null/void
#define BASE 10 // the default base for functions like round, etc.
#define PI 3.1415926535897932384626433832795028841971693993751058209749 // pi (ratio of a circle's circumference to its diameter)
#define E 2.7182818284590452353602874713526624977572470936999595749669 // e (the number such that f(x) = e^x equals f'(x), f''(x), etc.)
#define CURRENT_YEAR 12 // the last two digits of the current year (find a better solution than using this?)
#define SEC_IN_MIN 60 // seconds in a minute
#define SEC_IN_HOUR 3600 // seconds in an hour
#define SEC_IN_DAY 86400L // seconds in a day
#define MIN_IN_HOUR 60 // minutes in an hour
#define HOUR_IN_DAY 24 // hours in a day
#define DAYS_IN_WEEK 7 // days in a week
#define DAYS_IN_YEAR 365 // days in a (non-leap) year
#define MON_IN_YEAR 12 // months in a year
#define YEAR_ZERO 1970 // the year UNIX timestamps consider 0
#define YEAR_ZERO_LEAP_DAYS (1970 / 4) - (1970 / 100) + (1970 / 400)

// types
#define TYPE_UNK 0 // unknown type, only used for function arguments with no specified type
#define TYPE_NIL 1 // nil/null/void
#define TYPE_EXP 2 // expression
#define TYPE_LAZ 3 // lazy expression
#define TYPE_INT 4 // integer
#define TYPE_FLO 5 // float
#define TYPE_STR 6 // string
#define TYPE_ARR 7 // array
#define TYPE_DAT 8 // date
#define TYPE_OBJ 9 // object
#define TYPE_FUN 10 // function
#define TYPE_TYP 11 // type
#define TYPE_COMP_START 12 // the first available type ID for composite (i.e. user defined) types

// expression flags
#define EFLAG_NONE -1 // the default flag
#define EFLAG_CONVBASE 0 // marks the expression as an integer that needs its base converted
#define EFLAG_VAR 0 // marks the expression as a variable
#define EFLAG_SYMB 1 // marks the expression a string symbol
#define EFLAG_ARR 0 // marks the expression as an array expression
#define EFLAG_DEL 1 // marks the expression for deletion

// hash element flags
#define HFLAG_PRIM 0 // primitive function
#define HFLAG_USER 1 // user defined variable
#define HFLAG_DIRECT 2 // direct value that doesn't need to be freed from memory

// errors
#define ERR_GENERAL 0 // general (unknown) error
#define ERR_UNCLOSED_PAREN 1 // there is at least one unclosed opening parenthesis
#define ERR_UNMATCHED_PAREN 2 // there is at least one unmatched closing parenthesis
#define ERR_UNCLOSED_STR_LIT 3 // there is an unclosed string literal
#define ERR_INVALID_ARG 4 // an argument of an expression doesn't meet the type requirements
#define ERR_INVALID_NUM_ARGS 5 // the given number of arguments doesn't meet the type requirements
#define ERR_INVALID_PROP_TYP 6 // the given property doesn't meet the type requirements
#define ERR_UNDEFINED_VAR 7 // the given variable is undefined
#define ERR_UNDEFINED_FUN 8 // the given function is undefined with the given arguments
#define ERR_UNDEFINED_TYP 9 // the given type is undefined
#define ERR_UNDEFINED_PROP 10 // the given property is undefined
#define ERR_OUT_OF_BOUNDS 11 // the index being accessed is out of the bounds of the given array

// environment defaults
#define INITIAL_VAR_COUNT 100
#define INITIAL_VAR_SIZE 100
#define INITIAL_ENV_COUNT 100
#define INITIAL_ENV_SIZE 89
#define INITIAL_ROOT_ENV_SIZE 11519

// symbols defaults
#define INITIAL_SYMBOL_COUNT 737279

// types array defaults
#define INITIAL_TYPES_SIZE 100

// indicates a potentially infinite number of arguments (for primitive functions)
#define ARGLEN_INF -1
// 'more arguments' indicator (for user functions)
#define UFUNC_MORE_ARGS "..."

// property constants
#define PROP_PRIVACY_PUBLIC 0
#define PROP_PRIVACY_PRIVATE 1
#define PROP_RANGE_GLOBAL 0
#define PROP_RANGE_LOCAL 1
#define PROP_ATTR_UNK -1

// exit codes
#define EXIT_SUCCESS 0
#define EXIT_NO_ARGS 1
#define EXIT_OUT_OF_MEMORY 2

#endif
