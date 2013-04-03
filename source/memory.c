/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   memory.c
    @brief  Contains a more sophisticated version of malloc that errors out if more memory cannot be allocated
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>

#include "memory.h"
#include "constants.h"

static void freeExpr_(expression*, int);

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
    @return         nothing
*/
inline void freeExpr (expression* expr) {
    freeExpr_(expr, 1);
}

/*! Frees from memory the given expression and all its child content (does not free the next expression)
    @param expr     the expression to free from memory
    @return         nothing
*/
inline void freeExprNR (expression* expr) {
    freeExpr_(expr, 0);
}

/*! A helper function for freeExpr and freeExprNR
    @param expr     the expression to free from memory
    @param next     whether to free the next expression as well as the given one
    @return         nothing
*/
static void freeExpr_ (expression* expr, int next) {
    if (expr != NULL) { // if the expression isn't null
        exprvals ev = expr->ev;
        expressionstack* es;
        int i;
        argument* arg;
        switch (expr->type) { // depending on the expression's type
            case TYPE_EXP:
                freeExpr(ev.expval); // recursively call this function with the expression's child expression
                break;
            case TYPE_LAZ:
                freeExpr(ev.lazval->expval); // recursively call this function with the lazy expression's child expression
                while (ev.lazval->refs != NULL) { // free the lazy expression's reference stack
                    es = ev.lazval->refs->next;
                    free(ev.lazval->refs);
                    ev.lazval->refs = es;
                }
                free(ev.lazval); // free the lazy expression content
                break;
            case TYPE_STR:
                free(ev.strval->content); // free the actual string contained in the string content
                free(ev.strval); // free the string content
                break;
            case TYPE_ARR:
                freeArray(ev.arrval);
                break;
            //case TYPE_OBJ: TODO
            case TYPE_FUN:
                freeExpr(ev.funval->body); // free the function body expression
                int numargs;
                if (ev.funval->maxargs == ARGLEN_INF) {
                    numargs = ev.funval->minargs;
                } else {
                    numargs = ev.funval->maxargs;
                }
                for (i = 0; i < numargs; ++i) { // free the function arguments
                    arg = ev.funval->args[i];
                    free(arg->name->content);
                    free(arg->name);
                    free(arg);
                }
                free(ev.funval); // free the function content
                break;
        }
        if (next) { // if the next expression should be freed
            freeExpr(expr->next); // recursively call this function with the expression's next expression
        }
        free(expr); // free the expression itself
    }
}

/*! Frees from memory the given array and all its content
    @param arr      the array to free from memory
    @return         nothing
*/
void freeArray (array* arr) {
	int i;
	for (i = arr->start; i <= arr->end; i++) {
		freeExpr(arr->content[i]);
	}
	free(arr);
}
