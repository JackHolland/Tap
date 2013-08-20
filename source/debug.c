/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   debug.c
    @brief  Code for testing and debugging purposes
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdio.h>

#include "debug.h"
#include "constants.h"

static void printExprTree_(expression*, int);
static void printExprList_(expression*, int, int*);
static void printExprListFlags_(expression*, int, int*);
static void printExprMemory_(expression*, int);

/*! Prints the structure and content of the given expression tree in a tree format that includes only content related nodes
    @param head     the expression tree to print
    @return         nothing
*/
void printExprTree (expression* head) {
    printExprTree_(head, 0);
    printf("\n");
}

/*! Helper function for printExprTree
    @param expr     the expression tree to print
    @param level    the current tab level
    @return         nothing
*/
static void printExprTree_ (expression* expr, int level) {
    if (expr != NULL) { // if there are more expressions to print
        if (expr->type == TYPE_EXP) { // if the expression contains other expressions
            printExprTree_(expr->ev.expval, level + 1); // print whatever is inside the expression
        } else if (expr->type != TYPE_NIL) {
            int i;
            for (i = 0; i < level; i++) { // print the appropriate number of tabs for the current level
                printf("   ");
            }
            printf("[");
            switch (expr->type) { // print the expression's value according to its type
                case TYPE_INT:
                case TYPE_TYP:
                    printf("%ld", expr->ev.intval);
                    break;
                case TYPE_FLO:
                    printf("%f", expr->ev.floval);
                    break;
                case TYPE_STR:
                    printf("%s", expr->ev.strval->content);
                    break;
            }
            if (expr->flag != EFLAG_NONE) {
                printf("; %d", expr->flag);
            }
            printf("]");
            printf("\n"); // newline
        }
        printExprTree_(expr->next, level); // advance to the next expression in the list
    }
}

/*! Prints the structure and content, excluding flags, of the given expression tree in a list that includes all nodes
    @param head     the expression tree to print
    @return         nothing
*/
void printExprList (expression* head) {
    int headid = 1; // the first id (0 is reserved for the root id)
    printExprList_(head, 0, &headid);
    printf("\n");
}

/*! Helper function for printExprList
    @param expr     the expression tree to print
    @param parent   the id of the parent node
    @param id       the unique id of the node
    @return         nothing
*/
static void printExprList_ (expression* expr, int parent, int* id) {
    if (expr != NULL) { // if the expression isn't null
        if (expr->type == TYPE_INT || expr->type == TYPE_TYP) {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d, %ld], ", parent, *id, expr->type, expr->ev.intval);
        } else if (expr->type == TYPE_NIL) {
            // print the expression's parent, id, and type
            printf("[#%d->%d: %d, NIL], ", parent, *id, expr->type);
        } else if (expr->type == TYPE_STR) {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d, %s], ", parent, *id, expr->type, expr->ev.strval->content);
        } else {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d], ", parent, *id, expr->type);
        }
        if (expr->type == TYPE_EXP || expr->type == TYPE_LAZ) { // if the expression contains other expressions
            ++(*id); // increment the current id
            expression* expval;
            if (expr->type == TYPE_EXP) { // if the expression is regular
                expval = expr->ev.expval;
            } else { // if the expression is lazy
                expval = expr->ev.lazval->expval;
            }
            printExprList_(expval, (*id) - 1, id); // call this function with the child expression and this expression's id as the parent
        }
        if (expr->next != NULL) { // if there is another expression after this one
            ++(*id); // increment the current id
            printExprList_(expr->next, parent, id); // call this function with the next expression and the same parent
        }
    }
}

/*! Prints the structure and content, including flags, of the given expression tree in a list that includes all nodes
    @param head     the expression tree to print
    @return         nothing
*/
void printExprListFlags (expression* head) {
    int headid = 1; // the first id (0 is reserved for the root id)
    printExprListFlags_(head, 0, &headid);
    printf("\n");
}

/*! Helper function for printExprListFlags
    @param expr     the expression tree to print
    @param parent   the id of the parent node
    @param id       the unique id of the node
    @return         nothing
*/
static void printExprListFlags_ (expression* expr, int parent, int* id) {
    if (expr != NULL) { // if the expression isn't null
        if (expr->type == TYPE_INT || expr->type == TYPE_TYP) {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d, %ld; %d], ", parent, *id, expr->type, expr->ev.intval, expr->flag);
        } else if (expr->type == TYPE_NIL) {
            // print the expression's parent, id, and type
            printf("[#%d->%d: %d, NIL], ", parent, *id, expr->type);
        } else if (expr->type == TYPE_STR) {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d, %s; %d], ", parent, *id, expr->type, expr->ev.strval->content, expr->flag);
        } else {
            // print the expression's parent, id, type, content, and flag
            printf("[#%d->%d: %d; %d], ", parent, *id, expr->type, expr->flag);
        }
        if (expr->type == TYPE_EXP || expr->type == TYPE_LAZ) { // if the expression contains other expressions
            ++(*id); // increment the current id
            expression* expval;
            if (expr->type == TYPE_EXP) { // if the expression is regular
                expval = expr->ev.expval;
            } else { // if the expression is lazy
                expval = expr->ev.lazval->expval;
            }
            printExprListFlags_(expval, (*id) - 1, id); // call this function with the child expression and this expression's id as the parent
        }
        if (expr->next != NULL) { // if there is another expression after this one
            ++(*id); // increment the current id
            printExprListFlags_(expr->next, parent, id); // call this function with the next expression and the same parent
        }
    }
}

/*! Prints each struct allocated by the given expression tree
    @param expr     the expression tree to print
    @return         nothing
*/
void printExprMemory (expression* expr) {
    printExprMemory_(expr, 0);
    printf("\n");
}

/*! Helper function for printExprMemory
    @param expr     the expression tree to print
    @param level    the current tab level
    @return         nothing
*/
static void printExprMemory_ (expression* expr, int level) {
    if (expr != NULL) { // if the expression isn't null
        exprvals ev = expr->ev;
        int i;
        for (i = 0; i < level; i++) { // print the appropriate number of tabs for the current level
            printf("   ");
        }
        printf("expr: %p -> ", expr); // print the memory locations of the expression itself
        switch (expr->type) { // print the memory locations of every struct allocated by the expression (depending on its type)
            case TYPE_EXP:
                printf("expval: %p\n", ev.expval);
                printExprMemory_(ev.expval, level + 1); // print memory locations the child expression's structures
                break;
            case TYPE_LAZ:
                printf("lazval: %p -> expval: %p, refs: %p\n", ev.lazval, ev.lazval->expval, ev.lazval->refs);
                printExprMemory_(ev.lazval->expval, level + 1); // print memory locations the child expression's structures
                break;
            case TYPE_STR:
                printf("strval: %p -> content: %p\n", ev.strval, ev.strval->content);
                break;
            case TYPE_ARR:
                printf("arrval: %p -> content: %p\n", ev.arrval, ev.arrval->content);
                break;
            case TYPE_FUN:
                printf("funval: %p -> body: %p, args: %p\n", ev.funval, ev.funval->body, ev.funval->args);
                break;
        }
    }
}

/*! Prints each variable name and its associate value in the given environment
    @param env      the environment to print
    @return         nothing
*/
void printEnvironment (environment* env) {
    if (env != NULL) { // if the environment isn't null
        printf("--begin environment--\n");
        hashtable* table = env->variables;
        hashelement* element;
        int i;
        for (i = 0; i < table->size; ++i) { // for every potential element in the hash table
            element = table->table[i];
            while (element != NULL) { // while there are more elements at this hash index
                if (element->flag == HFLAG_PRIM) { // if the element is a primitive function then print its key and memory address
                    printf("prim: %s, %p\n", element->key, element->value);
                } else if (element->flag == HFLAG_USER) { // if the element is a user defined variable then prints its key and expression value
                    printf("user: %s, ", element->key);
                    printExprList((expression*)element->value);
                } else { // if the element is a directly stored value then print its key and numerical value
                    printf("drct: %s, %d", element->key, *((int*)element->value));
                }
                element = element->next;
            }
        }
        printf("--end environment--\n\n");
    }
}

