/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   constructors.c
    @brief  All of the constructors for structs and unions
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <string.h>

#include "constructors.h"
#include "constants.h"
#include "memory.h"
#include "strings.h"
#include "arrays.h"
#include "dates.h"

static array* copyArray_(array*, int);

/*! Creates a new expression struct with the default properties (i.e. a nil expression)
    @return     the new expression struct
*/
inline expression* newExpression () {
    return newExpressionOfType(TYPE_NIL); // set the value to nil until a real value is given
}

/*! Creates a new expression struct with the given type
    @param type     the type of data to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionOfType (datatype type) {
	exprvals ev;
	switch(type) {
		case TYPE_NIL:
			break;
		case TYPE_EXP:
			ev.expval = NULL;
			break;
		case TYPE_LAZ:
			ev.lazval = newLazyexpr();
			break;
		case TYPE_INT:
			ev.intval = 0;
			break;
		case TYPE_FLO:
			ev.floval = 0;
			break;
		case TYPE_STR:
			ev.strval = newString(strDup(""));
			break;
		case TYPE_ARR:
			ev.arrval = newArray(0);
			break;
		case TYPE_DAT:
			ev.datval = time(NULL);
			break;
		case TYPE_OBJ:
			// TODO
			break;
		case TYPE_FUN:
			ev.funval = newTapFunction(NULL, 0, 0, newExpressionNil());
			break;
		case TYPE_TYP:
			ev.intval = TYPE_UNK;
			break;
	}
    return newExpressionAll(type, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct of type nil
    @return         the new expression struct
*/
inline expression* newExpressionNil () {
	return newExpressionAll(TYPE_NIL, NULL, NULL, 0);
}

/*! Creates a new expression struct with the given integer as its value
    @param value    the integer to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionInt (long value) {
	exprvals ev;
	ev.intval = value;
    return newExpressionAll(TYPE_INT, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given float as its value
    @param value    the float to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionFlo (double value) {
	exprvals ev;
	ev.floval = value;
    return newExpressionAll(TYPE_FLO, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given string as its value
    @param value    the string to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionStr (string* value) {
	exprvals ev;
	ev.strval = value;
    return newExpressionAll(TYPE_STR, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given array as its value
    @param value    the array to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionArr (array* value) {
	exprvals ev;
	ev.arrval = value;
    return newExpressionAll(TYPE_ARR, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new lazy expression struct with the given expression as its value
    @param value    the lazy expression to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpressionLaz (expression* value) {
	exprvals ev;
	ev.lazval = newLazyexpr();
	ev.lazval->expval = value;
    return newExpressionAll(TYPE_LAZ, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given properties
    @param type     the type of data to be stored in the expression
    @param ev       the expression values union to be stored in the expression
    @param next     the next expression
    @param line     the line which the expression is on
    @return         the new expression struct
*/
expression* newExpressionAll (datatype type, exprvals* ev, expression* next, linenum line) {
    expression* expr = allocate(sizeof(expression)); // allocate the needed memory
    expr->type = type; // set the type to the given type
    if (ev == NULL) { // initialize the expression's value to NIL if one isn't given
    	expr->ev.intval = NIL;
    } else { // set the expression's value to the given exprvals
        memcpy(&(expr->ev), ev, sizeof(ev));
    }
    expr->next = next;
    expr->line = line;
    expr->flag = EFLAG_NONE;
    expr->refs = 0;
    return expr;
}

/*! Copies the given expression into new memory
    @param expr     the expression to copy
    @return         the new, duplicate expression
*/
expression* copyExpression (expression* expr) {
    if (expr == NULL) { // if the expression is null then just return null
        return NULL;
    } else {
        expression* duplicate = newExpressionAll(expr->type, NULL, copyExpression(expr->next), expr->line); // create a new expression with identical properties to the original one
        exprvals* ev1 = &(duplicate->ev);
        exprvals* ev2 = &(expr->ev);
        int i;
        switch (expr->type) { // some data type require their inner contents to be copied
            case TYPE_STR:
                ev1->strval = copyString(ev2->strval); // copy the string's content
                break;
            case TYPE_ARR:
                ev1->arrval = newArray(ev2->arrval->size);
                for (i = 0; i < ev1->arrval->size; ++i) { // copy each of the array's elements
                    ev1->arrval->content[i] = copyExpression(ev2->arrval->content[i]);
                }
                break;
            case TYPE_FUN:
                // copy the function's properties, arguments, and body
                ev1->funval = newTapFunction(ev2->funval->args, ev2->funval->minargs, ev2->funval->maxargs, copyExpression(ev2->funval->body));
                break;
            default: // if the original expression value is a primitive then copy it to the new expression value
                ev1->intval = ev2->intval;
                break;
        }
        duplicate->flag = expr->flag;
        duplicate->isref = expr->isref;
        duplicate->refs = expr->refs;
        if (expr->type == TYPE_EXP) { // copy the child expressions if the expression is a container expression
            ev1->expval = copyExpression(ev2->expval);
        } else if (expr->type == TYPE_LAZ) { // copy the lazy expression content if the expression is lazy
            tap_laz* lazy = newLazyexpr();
            lazy->expval = copyExpression(ev2->lazval->expval);
            expressionstack* es2 = ev2->lazval->refs;
            if (es2 == NULL) {
                lazy->refs = NULL;
            } else {
                lazy->refs = allocate(sizeof(expressionstack));
                expressionstack* es1 = lazy->refs;
                while (es2 != NULL) {
                    es1->expr = es2->expr;
                    if (es2->next != NULL) {
                        es1->next = allocate(sizeof(expressionstack));
                        es1 = es1->next;
                    }
                    es2 = es2->next;
                }
            }
            ev1->lazval = lazy;
        }
        return duplicate;
    }
}

/*! Creates a new lazy expression struct
    @param size     the number of elements that initially can be stored in the array
    @return         the new array struct
*/
tap_laz* newLazyexpr () {
    tap_laz* le = allocate(sizeof(tap_laz)); // allocate the needed memory
    le->expval = NULL;
    le->refs = NULL;
    return le;
}

/*! Creates a new string struct with the string
    @param string   the string to store in the struct
    @return         the new string struct, which contains the string and its size
*/
string* newString (char* content) {
    string* str = allocate(sizeof(string)); // allocate the needed memory
    str->content = content;
    str->size = strlen(content); // store the length of the string
    return str;
}

/*! Creates a new array struct with the given size
    @param size     the number of elements that initially can be stored in the array
    @return         the new array struct
*/
array* newArray (int size) {
    array* arr = allocate(sizeof(array) + sizeof(expression*) * size); // allocate the needed memory
    arr->size = size; // set the expression's size to the given size
    arr->start = 0; // set the start and end points to cover the entire array
    arr->end = size - 1;
    return arr;
}

/*! Copies the given array but uses the same element pointers
    @param arr      the array to copy
    @return         the new, duplicate array
*/
inline array* copyArray (array* arr) {
	return copyArray_(arr, 0);
}

/*! Copies the given array and its elements
    @param arr      the array to copy
    @return         the new, duplicate array
*/
inline array* copyArrayDeep (array* arr) {
	return copyArray_(arr, 1);
}

/*! A helper function for copyArray and copyArrayDeep
    @param arr      the array to copy
    @param deep     whether or not to free the expressions in the array
    @return         the new, duplicate array
*/
static array* copyArray_ (array* arr, int deep) {
	array* newarr = newArray(arrayUsedSize(arr));
	int i;
	int j;
	for (i = arr->start, j = 0; i <= arr->end; i++, j++) {
		if (deep) {
			newarr->content[j] = copyExpression(arr->content[i]);
		} else {
			newarr->content[j] = arr->content[i];
		}
	}
	return newarr;
}

/*! Creates a new date by parsing the given string
    @param str      the string to parse into a date
    @return         the new date or NIL if the date parsing failed
*/
date newDate (string* str) {
	char* sdate = str->content;
    if (strcmp(sdate, "now") == 0) {
        return time(NULL);
    } else {
    	date dat;
        int size = str->size;
        while (size > 0 && sdate[size - 1] == ' ') {
            --size;
        }
        int pos = 0;
        while (size > pos && sdate[pos] == ' ') {
            ++pos;
        }
        int valid = 1;
        tm timestruct;
        memset(&timestruct, 0, sizeof(timestruct));
        if (size > pos + 2 && isNumber(sdate[pos])) {
            if (isNumber(sdate[pos + 1]) && sdate[pos + 2] == '/') {
                timestruct.tm_mon = (sdate[pos] - '0') * 10 + (sdate[pos + 1] - '0') - 1;
                pos += 3;
            } else if (sdate[pos + 1] == '/') {
                timestruct.tm_mon = (sdate[pos] - '0') - 1;
                pos += 2;
            } else {
                valid = 0;
            }
            if (size > pos && isNumber(sdate[pos])) {
                if (isNumber(sdate[pos + 1]) && (size == pos + 2 || (size > pos + 2 && (sdate[pos + 2] == '/' || sdate[pos + 2] == ' ')))) {
                    timestruct.tm_mday = (sdate[pos] - '0') * 10 + (sdate[pos + 1] - '0') - 1;
                    pos += 3;
                } else if (size == pos + 1 || (size > pos + 1 && sdate[pos + 1] == '/')) {
                    timestruct.tm_mday = (sdate[pos] - '0') - 1;
                    pos += 2;
                } else {
                    valid = 0;
                }
                int yearchars = 0;
                while (size > pos + yearchars && isNumber(sdate[pos + yearchars])) {
                    ++yearchars;
                }
                if (yearchars == 2 || yearchars == 4) {
                    int i;
                    static int powers[4] = {1, 10, 100, 1000};
                    for (i = 0; i < yearchars; ++i) {
                        timestruct.tm_year += (sdate[pos + yearchars - i - 1] - '0') * powers[i];
                    }
                    if (yearchars == 2) {
                        if (timestruct.tm_year > CURRENT_YEAR) {
                            timestruct.tm_year -= 70;
                        } else {
                            timestruct.tm_year += 30;
                        }
                    } else {
                        timestruct.tm_year -= YEAR_ZERO;
                    }
                    pos += yearchars;
                    i = 0;
                    while (size > pos + i && sdate[pos + i] == ' ') {
                        ++i;
                    }
                    if (i > 0) {
                        pos += i;
                        if (size > pos && isNumber(sdate[pos])) {
                            if (size > pos + 2 && isNumber(sdate[pos + 1]) && sdate[pos + 2] == ':') {
                                timestruct.tm_hour = (sdate[pos] - '0') * 10 + (sdate[pos + 1] - '0');
                                pos += 3;
                            } else if (size > pos + 1 && sdate[pos + 1] == ':') {
                                timestruct.tm_hour = sdate[pos] - '0';
                                pos += 2;
                            } else {
                                valid = 0;
                            }
                            if (size > pos && isNumber(sdate[pos])) {
                                if (isNumber(sdate[pos + 1]) && (size == pos + 2 || (size > pos + 2 && (sdate[pos + 2] == ':' || sdate[pos + 2] == ' ')))) {
                                    timestruct.tm_min = (sdate[pos] - '0') * 10 + (sdate[pos + 1] - '0');
                                    pos += 3;
                                } else if (size == pos + 1 || (size > pos + 1 && sdate[pos + 1] == ':')) {
                                    timestruct.tm_min = sdate[pos] - '0';
                                    pos += 2;
                                } else {
                                    valid = 0;
                                }
                                if (size > pos && isNumber(sdate[pos])) {
                                    if (size > pos + 1 && isNumber(sdate[pos + 1])) {
                                        timestruct.tm_sec = (sdate[pos] - '0') * 10 + (sdate[pos + 1] - '0');
                                        pos += 2;
                                    } else {
                                        timestruct.tm_sec = sdate[pos] - '0';
                                        pos += 1;
                                    }
                                    while (pos < size && sdate[pos] == ' ') {
                                        ++pos;
                                    }
                                }
                                if (size > pos + 1) {
                                    if ((sdate[pos] == 'p' || sdate[pos] == 'P') && (sdate[pos + 1] == 'm' || sdate[pos + 1] == 'M')) {
                                        timestruct.tm_hour += 12;
                                    } else if (!((sdate[pos] == 'a' || sdate[pos] == 'A') && (sdate[pos + 1] == 'm' || sdate[pos + 1] == 'M'))) {
                                        valid = 0;
                                    }
                                    if (size != pos + 2) {
                                        valid = 0;
                                    }
                                } else {
                                    valid = size == pos;
                                }
                            } else {
                                valid = size == pos;
                            }
                        } else {
                            valid = size == pos;
                        }
                    }
                } else {
                    valid = yearchars == 0;
                }
            } else {
                valid = size == pos;
            }
        } else {
            valid = size == pos;
        }
        if (valid) {
            switch (timestruct.tm_mon) {
                case 0:
                case 2:
                case 4:
                case 6:
                case 7:
                case 9:
                case 11:
                    if (timestruct.tm_mday > 31) {
                        valid = 0;
                    }
                    break;
                case 1:
                    if (leapYear(timestruct.tm_year) && timestruct.tm_mday > 29) {
                        valid = 0;
                    } else if (timestruct.tm_mday > 28) {
                        valid = 0;
                    }
                    break;
                case 3:
                case 5:
                case 8:
                case 10:
                    if (timestruct.tm_mday > 30) {
                        valid = 0;
                    }
                    break;
                default:
                    valid = 0;
            }
            if (timestruct.tm_hour > HOUR_IN_DAY - 1 || timestruct.tm_min > MIN_IN_HOUR || timestruct.tm_sec > SEC_IN_MIN) {
                valid = 0;
            }
        }
        if (valid) {
            dat = timestruct.tm_sec + (timestruct.tm_min * SEC_IN_MIN) + (timestruct.tm_hour * SEC_IN_HOUR) + (timestruct.tm_mday * SEC_IN_DAY);
            int year = timestruct.tm_year + YEAR_ZERO;
            int month = timestruct.tm_mon - 1;
            int monthdays = 0;
            while (month >= 0) {
                switch (month) {
                    case 0:
                    case 2:
                    case 4:
                    case 6:
                    case 7:
                    case 9:
                    case 11:
                        monthdays += 31;
                        break;
                    case 1:
                        if (leapYear(year)) {
                            monthdays += 29;
                        } else {
                            monthdays += 28;
                        }
                        break;
                    case 3:
                    case 5:
                    case 8:
                    case 10:
                        monthdays += 30;
                        break;
                }
                --month;
            }
            dat += (monthdays + (timestruct.tm_year * DAYS_IN_YEAR) + ((timestruct.tm_year + 2) / 4) - ((timestruct.tm_year + 30)  / 100) + ((timestruct.tm_year + 330) / 400)) * SEC_IN_DAY;
        } else {
            return NULL;
        }
        return dat;
    }
}

/*! Creates a new object struct with the given type and properties list
    @param type     the object's type
    @param props    the object's list of properties
    @return         the new object struct
*/
tap_obj* newObject (datatype type, property* props) {
    tap_obj* obj = allocate(sizeof(tap_obj));
    obj->type = type;
    obj->props = props;
    return obj;
}

/*! Copies the given object with its type and properties
    @param obj      the object to copy
    @return         the new, duplicate object
*/
tap_obj* copyObject (tap_obj* obj) {
    property* props = copyProperty(obj->props);
    property* cprop = props->next;
    while (cprop != NULL) {
        cprop = copyProperty(cprop);
        cprop = cprop->next;
    }
    return newObject(obj->type, props);
}

/*! Creates a new type with the given inherited types, required types, and properties
    @param id           the assigned ID of the type
    @param name         the name of the type
    @param required     the list of required property constraints
    @param inherits     the list of inherited type constraints
    @param properties   the list of properties (i.e. variables, functions, etc.) associated with the type
*/
type* newType (datatype id, char* name, stringlist* required, typelist* inherits, property* properties) {
    type* typ = allocate(sizeof(type));
    typ->id = id;
    typ->name = name;
    typ->required = required;
    typ->inherits = inherits;
    typ->properties = properties;
    return typ;
}

/*! Creates a new property struct with the given name, type restrictions, privacy, and range
    @param name     the name used to identify the property
    @param types    the type restrictions on the property (use TYP_UNK for no restrictions)
    @param privacy  the privacy (public/private) of the property
    @param range    the range (static/dynamic) of the property
    @param value    the value to initially give the property
    @return         the new property struct
*/
property* newProperty (char* name, typelist* types, int privacy, int range, expression* value) {
    property* prop = allocate(sizeof(property));
    prop->name = strDup(name);
    prop->types = types;
    prop->privacy = privacy;
    prop->range = range;
    prop->value = copyExpression(value);
    prop->next = NULL;
    return prop;
}

/*! Copies the given property with its name, type restrictions, privacy, and range
    @param prop     the property to copy
    @return         the new, duplicate property
*/
property* copyProperty (property* prop) {
    typelist* ats = copyTypelist(prop->types);
    typelist* cat = ats->next;
    while (cat != NULL) {
        cat = copyTypelist(cat);
        cat = cat->next;
    }
    return newProperty(strDup(prop->name), ats, prop->privacy, prop->range, prop->value);
}

/*! Creates a new user function struct with the given arguments, more arguments flag, and body
    @param args         the array of arguments the function accepts
    @param minargs      the minimum number of arguments the new function accepts
    @param maxargs      the maximum number of arguments the new function accepts (may be infinite)
    @param body         the list of expressions the function performs when called
    @return             the new user function struct
*/
tap_fun* newTapFunction (argument* args[], int minargs, int maxargs, expression* body) {
    int numargs;
    if (maxargs == ARGLEN_INF) {
        numargs = minargs;
    } else {
        numargs = maxargs;
    }
    tap_fun* uf = allocate(sizeof(tap_fun) + sizeof(argument*) * numargs); // allocate the needed memory
    uf->body = body; // set the function's body to the given body
    uf->minargs = minargs;
    uf->maxargs = maxargs;
    int i;
    for (i = 0; i < numargs; ++i) { // copy each of the given arguments to the user function's argument array
        uf->args[i] = args[i];
    }
    return uf;
}

/*! Copies the given user function with the given body, minimum and maximum arguments, and arguments list
    @param uf   the user function to copy
    @return     the new, duplicate user function
*/
inline tap_fun* copyTapFunction (tap_fun* uf) {
    if (uf == NULL) {
        return NULL;
    } else {
        int numargs;
        if (uf->maxargs == ARGLEN_INF) {
            numargs = uf->minargs;
        } else {
            numargs = uf->maxargs;
        }
        argument** args = allocate(sizeof(argument*) * numargs);
        int i;
        for (i = 0; i < numargs; ++i) {
            args[i] = copyArgument(uf->args[i]);
        }
        return newTapFunction(args, uf->minargs, uf->maxargs, copyExpression(uf->body));
    }
}

/*! Creates a new argument struct with the given name, type, and initial value
    @param name     the array of arguments the function accepts
    @param type     a flag indicating whether or not the function can accept more arguments after its listed ones
    @param initial  the list of expressions the function performs when called
    @return         the new user function struct
*/
inline argument* newArgument (string* name, typelist* types, expression* initial) {
    argument* arg = allocate(sizeof(argument)); // allocate the needed memory
    arg->name = name; // set the argument's name to the given name
    arg->types = types; // set the argument's type to the given type, which may be TYPE_UNK, or 'unknown type'
    arg->initial = initial; // set the argument's intiial expression value to the given value, which may be null
    return arg;
}

/*! Copies the given argument
    @param arg     the argument to copy
    @return         the new, duplicate argument
*/
inline argument* copyArgument (argument* arg) {
    if (arg == NULL) {
        return NULL;
    } else {
        return newArgument(copyString(arg->name), copyTypelists(arg->types), copyExpression(arg->initial));
    }
}

/*! Creates a new argument type list struct with the given type as the first type of the list
    @param name     the array of arguments the function accepts
    @param type     a flag indicating whether or not the function can accept more arguments after its listed ones
    @param initial  the list of expressions the function performs when called
    @return         the new user function struct
*/
inline typelist* newTypelist (datatype type) {
    typelist* at = allocate(sizeof(typelist));
    at->type = type;
    at->next = NULL;
    return at;
}

/*! Creates a new argument type list struct with the given type as the first type of the list and the given typelist as the next argument
    @param name     the array of arguments the function accepts
    @param type     a flag indicating whether or not the function can accept more arguments after its listed ones
    @param initial  the list of expressions the function performs when called
    @return         the new user function struct
*/
inline typelist* newTypelist_n (datatype type, typelist* next) {
    typelist* at = allocate(sizeof(typelist));
    at->type = type;
    at->next = next;
    return at;
}

/*! Copies the given argument type with its type and next reference
    @param at   the argument type to copy
    @return     the new, duplicate argument type
*/
inline typelist* copyTypelist (typelist* at) {
    if (at == NULL) {
        return NULL;
    } else {
        return newTypelist_n(at->type, at->next);
    }
}

/*! Copies the given argument types
    @param at   the argument types to copy
    @return     the new, duplicate argument types
*/
inline typelist* copyTypelists (typelist* at) {
    if (at == NULL) {
        return NULL;
    } else {
        return newTypelist_n(at->type, copyTypelists(at->next));
    }
}

/*! Creates a new type definitions structure, which contains a type and the next type in the list
    @param type     the type the type definition references
    @return         the new type definitions structure
*/
inline typedefs* newTypedefs (type* typ) {
    typedefs* td = allocate(sizeof(typedefs));
    td->type = typ;
    td->next = NULL;
    return td;
}

/*! Creates a new expression stack pointing to the given expression stack
    @param current  the expression stack to be pointed to
    @return         the new expression stack
*/
inline expressionstack* newExpressionstack (expressionstack* current) {
    expressionstack* es = allocate(sizeof(expressionstack));
    es->expr = NULL;
    es->next = current;
    return es;
}

/*! Creates a function structure, which contains a reference to the primitive function and its return type
    @return     the new function structure
*/
tap_prim_fun* newPrimFunction (void(*address)(expression*[], int, exprvals*, datatype*), int minargs, int maxargs, typelist* types) {
    tap_prim_fun* func = allocate(sizeof(tap_prim_fun));
    func->address = address;
    func->minargs = minargs;
    func->maxargs = maxargs;
    func->types = types;
    return func;
}

/*! Creates an environment structure, which contains a hash table of functions and the parent environment's index
    @return     the new environment structure
*/
inline environment* newEnvironment (hashtable* variables, int parent) {
    environment* env = allocate(sizeof(environment));
    env->variables = variables;
    env->types = NULL;
    env->numvars = 0;
    env->parent = parent;
    return env;
}

/*! Creates a string list structure
    @param str      the string to be stored
    @param next     the next string in the list
    @return         the new string list structure
*/
stringlist* newStringlist (string* str, stringlist* next) {
    stringlist* sl = allocate(sizeof(stringlist));
    sl->str = copyString(str);
    sl->next = next;
    return sl;
}

/*! Creates an error list structure, which contains a list of errors
    @param code     the error code to categorize this new error
    @param message  the message specific to this error instance
    @param line     the line at which the error occurs
    @param index    the index in the line at which the error occurs
    @return         the new error list structure
*/
errorlist* newErrorlist (uint code, string* message, linenum line, uint index) {
    errorlist* sl = allocate(sizeof(errorlist));
    sl->code = code;
    sl->message = message;
    sl->line = line;
    sl->index = index;
    sl->next = NULL;
    return sl;
}
