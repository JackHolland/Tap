/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   memory.c
    @brief  Contains a more sophisticated version of malloc that errors out if more memory cannot be allocated
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "memory.h"
#include "constants.h"
#include "constructors.h"

static bool freeExpr_(expression*, bool);

/*! Attempts to allocate the amount of memory specified by the given size and errors out if the memory could not be allocated
    @param size     how much memory to allocate (in bytes)
    @return         the location of the newly allocated memory
*/
inline void* allocate (size_t size) {
	void* location = malloc(size); // attempt to allocate the amount of memory specified by the given size
	if (location == NULL) { // if the memory could not be allocated
		exit(EXIT_OUT_OF_MEMORY); // exit with the code representing an out of memory error
	} else { // if the memory was successfully allocated
		return location; // return the location of the newly allocated memory
	}
}

/*! Frees from memory the given expression and all its associated content
    @param expr     the expression to free from memory
    @return         0
*/
inline bool freeExpr (expression* expr) {
	return freeExpr_(expr, 1);
}

/*! Frees from memory the given expression and all its child content (does not free the next expression)
    @param expr     the expression to free from memory
    @return         0
*/
inline bool freeExprNR (expression* expr) {
	return freeExpr_(expr, 0);
}

/*! A helper function for freeExpr and freeExprNR
    @param expr     the expression to free from memory
    @param next     whether to free the next expression as well as the given one
    @return         0
*/
static bool freeExpr_ (expression* expr, bool next) {
    if (expr != NULL) { // if the expression isn't null
        exprvals ev = expr->ev;
        switch (expr->type) { // depending on the expression's type
            case TYPE_EXP:
                freeExpr(ev.expval); // recursively call this function with the expression's child expression
                break;
            case TYPE_LAZ:
                freeLaz(ev.lazval);
                break;
            case TYPE_STR:
                freeStr(ev.strval);
                break;
            case TYPE_ARR:
                freeArr(ev.arrval);
                break;
            case TYPE_OBJ:
            	freeObj(ev.objval);
            case TYPE_FUN:
                freeFun(ev.funval);
                break;
        }
        if (next) { // if the next expression should be freed
            freeExpr(expr->next); // recursively call this function with the expression's next expression
        }
        free(expr); // free the expression itself
    }
    
    return 0;
}

/*! Frees from memory the given array and its content
    @param le       the lazy expression to free from memory
    @return         0
*/
bool freeLaz (tap_laz* laz) {
	freeExpr(laz->expval);
	exprstack* es1 = laz->refs;
	exprstack* es2;
	while (es1 != NULL) {
		es2 = es1->next;
		freeExpr(es1->expr);
		es1 = es2;
	}
	free(laz);
	
	return 0;
}

/*! Frees from memory the given string and its content
	@param str		the string to free from memory
	@return			0
*/
bool freeStr (string* str) {
	free(str->content);
	free(str);
	
	return 0;
}

/*! Frees from memory the given array and its content
    @param arr      the array to free from memory
    @return         0
*/
bool freeArr (array* arr) {
	int i;
	for (i = arr->start; i <= arr->end; i++) {
		freeExpr(arr->content[i]);
	}
	free(arr);
	
	return 0;
}

/*! Frees from memory the given object and its content
    @param obj      the object to free from memory
    @return         0
*/
bool freeObj (tap_obj* obj) {
	property* prop1 = obj->props;
	property* prop2;
	while (prop1 != NULL) {
		prop2 = prop1->next;
		freeProp(prop1);
		prop1 = prop2;
	}
	free(obj);
	
	return 0;
}

/*! Frees from memory the given property and its content
    @param prop     the property to free from memory
    @return         0
*/
bool freeProp (property* prop) {
	free(prop->name);
	freeTypelist(prop->types);
	freeExpr(prop->value);
	if (prop->next != NULL) {
		freeProp(prop->next);
	}
	free(prop);
	
	return 0;
}

/*! Frees from memory the given function and its content
    @param fun      the function to free from memory
    @return         0
*/
bool freeFun (tap_fun* fun) {
	freeExpr(fun->body);
    int numargs;
    if (fun->maxargs == ARGLEN_INF) {
        numargs = fun->minargs;
    } else {
        numargs = fun->maxargs;
    }
    int i;
    for (i = 0; i < numargs; ++i) {
        freeArg(fun->args[i]);
    }
    free(fun);
    
    return 0;
}

/*! Frees from memory the given argument
	@param arg		the argument to free from memory
	@return			0
*/
bool freeArg (argument* arg) {
	freeStr(arg->name);
	freeTypelist(arg->types);
	freeExpr(arg->initial);
	free(arg);
	
	return 0;
}

/*! Frees from memory the given list of types
	@param tl		the list of types to free from memory
	@return			0
*/
bool freeTypelist (typelist* tl) {
	typelist* next_tl;
	while (tl != NULL) {
		next_tl = tl->next;
		free(tl);
		tl = next_tl;
	}
	
	return 0;
}

/*! Frees from memory the given composite type
	@param typ		the type to free from memory
	@return			0
*/
bool freeCompTyp (type* typ) {
	free(typ->name);
	freeStringlist(typ->required);
	freeTypelist(typ->inherits);
	freeProp(typ->properties);
	free(typ);
	
	return 0;
}

/*! Frees from memory the given list of composite type definitions
	@param td		the list of type definitions
	@return			0
*/
bool freeTypedefs (typedefs* td) {
	typedefs* next_td;
	while (td != NULL) {
		next_td = td->next;
		freeCompTyp(td->type);
		free(td);
		td = next_td;
	}
	
	return 0;
}

/*! Frees from memory the given stack of expressions
	@param es		the stack of expressions
	@return			0
*/
bool freeExprstack (exprstack* es) {
	exprstack* next_es;
	while (es != NULL) {
		next_es = es->next;
		freeExpr(es->expr);
		free(es);
		es = next_es;
	}
	
	return 0;
}

/*! Frees from memory the given tap primitive function
	@param fun		the primitive function
	@return			0
*/
bool freePrimFun (tap_prim_fun* fun) {
	freeTypelist(fun->types);
	free(fun);
	
	return 0;
}

/*! Frees from memory the given environment
	@param env		the environment
	@return			0
*/
bool freeEnv (environment* env) {
	deleteHash(env->variables);
	freeTypedefs(env->types);
	free(env);
	
	return 0;
}

/*! Frees from memory the given list of strings
	@param sl		the list of strings to free from memory
	@return			0
*/
bool freeStringlist (stringlist* sl) {
	stringlist* next_sl;
	while (sl != NULL) {
		next_sl = sl->next;
		freeStr(sl->str);
		free(sl);
		sl = next_sl;
	}
	
	return 0;
}

/*! Frees from memory the given list of errors
	@param sl		the list of strings to free from memory
	@return			0
*/
bool freeErrorlist (errorlist* el) {
	errorlist* next_el;
	while (el != NULL) {
		next_el = el->next;
		free(el);
		el = next_el;
	}
	
	return 0;
}

