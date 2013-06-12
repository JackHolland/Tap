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

static array* copyArray_(array*, int);

/*! Creates a new expression struct with the default properties
    @return     the new expression struct
*/
inline expression* newExpression () {
    return newExpression_t(TYPE_NIL); // set the value to nil until a real value is given
}

/*! Creates a new expression struct with the given type
    @param type     the type of data to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_t (datatype type) {
    return newExpression_all(type, NULL, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given integer as its value
    @param value    the integer to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_int (long value) {
	exprvals ev;
	ev.intval = value;
    return newExpression_all(TYPE_INT, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given float as its value
    @param value    the float to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_flo (double value) {
	exprvals ev;
	ev.floval = value;
    return newExpression_all(TYPE_FLO, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given string as its value
    @param value    the string to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_str (string* value) {
	exprvals ev;
	ev.strval = value;
    return newExpression_all(TYPE_STR, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given array as its value
    @param value    the array to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_arr (array* value) {
	exprvals ev;
	ev.arrval = value;
    return newExpression_all(TYPE_ARR, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new lazy expression struct with the given expression as its value
    @param value    the lazy expression to be stored in the expression
    @return         the new expression struct
*/
inline expression* newExpression_laz (expression* value) {
	exprvals ev;
	ev.lazval = newLazyexpr();
	ev.lazval->expval = value;
    return newExpression_all(TYPE_LAZ, &ev, NULL, 0); // set the value to null until a real value is given and set the next expression to null
}

/*! Creates a new expression struct with the given properties
    @param type     the type of data to be stored in the expression
    @param ev       the expression values union to be stored in the expression
    @param next     the next expression
    @param line     the line which the expression is on
    @return         the new expression struct
*/
expression* newExpression_all (datatype type, exprvals* ev, expression* next, linenum line) {
    expression* expr = allocate(sizeof(expression)); // allocate the needed memory
    expr->type = type; // set the type to the given type
    if (ev != NULL) { // set the expression's value to the given exprvals
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
        expression* duplicate = newExpression_all(expr->type, NULL, copyExpression(expr->next), expr->line); // create a new expression with identical properties to the original one
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
                ev1->funval = newUserfunction(ev2->funval->args, ev2->funval->minargs, ev2->funval->maxargs, copyExpression(ev2->funval->body));
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
            lazyexpr* lazy = newLazyexpr();
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
lazyexpr* newLazyexpr () {
    lazyexpr* le = allocate(sizeof(lazyexpr)); // allocate the needed memory
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

/*! Creates a new object struct with the given type and properties list
    @param type     the object's type
    @param props    the object's list of properties
    @return         the new object struct
*/
object* newObject (datatype type, property* props) {
    object* obj = allocate(sizeof(object));
    obj->type = type;
    obj->props = props;
    return obj;
}

/*! Copies the given object with its type and properties
    @param obj      the object to copy
    @return         the new, duplicate object
*/
object* copyObject (object* obj) {
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
userfunction* newUserfunction (argument* args[], int minargs, int maxargs, expression* body) {
    int numargs;
    if (maxargs == ARGLEN_INF) {
        numargs = minargs;
    } else {
        numargs = maxargs;
    }
    userfunction* uf = allocate(sizeof(userfunction) + sizeof(argument*) * numargs); // allocate the needed memory
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
inline userfunction* copyUserfunction (userfunction* uf) {
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
        return newUserfunction(args, uf->minargs, uf->maxargs, copyExpression(uf->body));
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
primfunction* newPrimFunction (void(*address)(expression*[], int, exprvals*, datatype*), int minargs, int maxargs, typelist* types) {
    primfunction* func = allocate(sizeof(primfunction));
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
