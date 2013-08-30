/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   engine.c
    @brief  The interpreter engine
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "engine.h"
#include "constants.h"
#include "externs.h"
#include "structs.h"
#include "typedefs.h"
#include "constructors.h"
#include "memory.h"
#include "strings.h"
#include "dates.h"
#include "../primitives/prim_nil.h"
#include "../primitives/prim_exp.h"
#include "../primitives/prim_laz.h"
#include "../primitives/prim_int.h"
#include "../primitives/prim_flo.h"
#include "../primitives/prim_str.h"
#include "../primitives/prim_arr.h"
#include "../primitives/prim_dat.h"
#include "../primitives/prim_obj.h"
#include "../primitives/prim_fun.h"
#include "../primitives/prim_typ.h"
#include "debug.h"

extern environment* environments[];
extern uint cenvironment;
extern expression* cfunction;
extern expression* cobject;
extern datatype ctypeid;
extern errorlist* errors;
extern errorlist* cerror;

static expression* parse_(char*, uint, linenum);

/*! Parses the given string and returns a list containing parsed expressions
    @param text         the text to be parsed
    @return             the head of the list of expressions
*/
expression* parse (char* text) {
	if (text == NULL) {
		return NULL;
	}
    return parse_(text, 0, 1); // start parsing at the beginning
}

/*! Parses the given string starting from the given index and given line and returns a list containing parsed expressions
    @param text         the text to be parsed
    @param startindex   the index to start at
    @return             the head of the list of expressions
    @todo               errors
*/
static expression* parse_ (char* text, uint startindex, linenum startline) {
    linenum endline = startline; // the last line parsed so far
    expression* head = newExpressionOfType(TYPE_EXP); // list of expressions
    head->line = startline;
    expression* tail = NULL; // the last expression parsed
    expression* tempexpr = NULL;
    expression** current = (expression**)&head; // the current expression
    exprstack* rootes = newExprstack(NULL); // the root expression in the stack
    rootes->expr = *current;
    exprstack* es = rootes; // the current expression in the stack
    exprstack* tempes; // a temporary storage when deleting items on the stack
    uint lastdelim = startindex; // last delimeter in the stack
    int currentdelim = -1; // the ascii value of the most recent delimeter
    uint size = strlen(text); // the size of the given text
    int situation = 0; // a flag used to determine how the next parsed expression should be structured
    int emptyparse; // a flag used to handle empty parses (i.e. two opening delimeters in a row)
    int iscont; // a flag used to determine whether or not an expression piece is a continuation of an existing expression
    uint i;
    for (i = startindex; i < size; i++) {
        int ascii = text[i]; // ASCII value of the current character
        switch (ascii) {
            case '(':
            case '[':
            case '{':
                emptyparse = i - lastdelim == 0 && currentdelim != -1 && !situation; // determine if the parse is empty or not
                if (lastdelim == 0) { // if no delimeters have been parsed yet
                   currentdelim = 0; // set the value of currentdelimeter to a dummy value
                   iscont = 0; // the expression can't be a continuation so set it to 0
                } else { // if at least one delimeter has been parsed
                    currentdelim = text[lastdelim - 1]; // set the ascii value of currentdelimeter to the ascii value of the most recent delimeter
                    iscont = (text[lastdelim - 1] >= ')'); // if the last delimeter was a right parenthesis then the next expression piece is a continuation of an existing expression
                }
                // parse between the last delimeter and the current one
                tempexpr = parsePiece(substr(text, lastdelim, i), &tail, startline, startindex, currentdelim, iscont);
                if (!emptyparse && tempexpr->type == TYPE_NIL) { // if the tail didn't parse into a valid expression
                    freeExpr(tempexpr); // free the memory of the nil expression
                    tempexpr = NULL; // set the expression to null to indicate it doesn't contain anything useful
                } else { // if the tail parsed into a valid expression
                    tempexpr->line = endline; // set the expression's line to the current one
                    if (situation) { // add the parsed expression to the current expression depending on the situation flag
                        (*current)->next = tempexpr;
                    } else {
                        storeChildExpression(*current, tempexpr);
                    }
                    if (emptyparse) { // if nothing was parsed
                        tempexpr->type = TYPE_EXP; // initialize the parsed expression
                        tempexpr->ev.expval = NULL;
                        tempexpr->line = endline;
                        if ((*current)->type == TYPE_EXP) { // set the current expression to the current expression value depending on its type
                            current = &((*current)->ev.expval);
                        } else {
                            current = &((*current)->ev.lazval->expval);
                        }
                    }
                }
                if (tempexpr != NULL && tail != NULL) { // if something was parsed
                    current = &tail; // set the current expression to the last one parsed
                }
                if (!emptyparse) {
                    if (ascii == '[') { // if the expression is lazy
                        // add a lazy expression inside the last one parsed for the content inside its parentheses
                        tempexpr = newExpressionOfType(TYPE_LAZ);
                        (*current)->next = tempexpr;
                    } else { // if the expression is regular or an array
                        // add an expression inside the last one parsed for the content inside its parentheses
                        (*current)->next = newExpressionOfType(TYPE_EXP);
                    }
                    current = (expression**)&((*current)->next); // set the current expression to this inner one
                    (*current)->line = endline; // set the expression's line to the current one
                }
                es->expr = *current; // add the current expression to the expression stack
                es = newExprstack(es); // push a new exprstack onto the end of the stack
                lastdelim = i + 1; // set the last delimeter to the current index
                situation = 0; // use the current expression for the next call to parsePiece
                break;
            case ')':
            case ']':
            case '}':
                if (i > lastdelim) { // if there is something to parse
                    // parse the substring into an expression
                    tempexpr = parsePiece(substr(text, lastdelim, i), &tail, startline, startindex, ascii, text[lastdelim - 1] >= ')');
                    if (tempexpr->type == TYPE_NIL) { // if the tail wasn't parsed into a valid expression
                        freeExpr(tempexpr->next); // free the nil expressions from memory
                        freeExpr(tempexpr);
                    } else { // if the tail was parsed into a valid expression
                        tempexpr->line = endline; // set the expression's line to the current one
                        if (ascii == '}') { // if the expression is an array
                            (*current)->flag = EFLAG_ARR; // mark the expression as an array expression
                        }
                        if (situation) { // add the parsed expression to the current expression depending on the situation flag
                            (*current)->next = tempexpr;
                        } else {
                            storeChildExpression(*current, tempexpr);
                        }
                    }
                } else if (situation == 0 && es != rootes) {
                    if (ascii == '}') {
                        (*current)->flag = EFLAG_ARR;
                    } else {
                        addError(newErrorlist(ERR_INVALID_NUM_ARGS, newString(strDup(text)), startline, startindex));
                    }
                }
                if (es == rootes) { // if there is an unmatched closing parenthesis
                    addError(newErrorlist(ERR_UNMATCHED_PAREN, newString(strDup(text)), startline, startindex));
                    break;
                } else if (es->next == NULL) { // if there are no more items on the stack
                    es->expr->next = parse_(text, i + 1, endline); // parse the next piece of the string and link the next piece to the current one
                    if (es->expr->next->type == TYPE_NIL) { // if the next expression is nil then free it from memory
                        freeExpr(es->expr->next);
                        es->expr->next = NULL;
                    }
                    i = size; // skip to the end of the loop so no more text is processed for this piece
                    break;
                } else { // if there are more items on the stack
                    tempes = es->next; // pop the last expression off the stack
                    free(es); // free the memory of the unused stack item
                    es = tempes;
                }
                current = &(es->expr); // set the current expression pointer to the last expression on the stack
                lastdelim = i + 1; // set the last delimeter to the current index
                situation = 1; // create a new expression assigned to current->next for the next call to parsePiece
                break;
            case '\n':
                ++endline;
                break;
            case '"':
                i = parseStringLiteral(text, i + 1, size); // skip the string literal
                if (i == size) { // if the string literal is never closed
                	situation = -1;
                    addError(newErrorlist(ERR_UNCLOSED_STR_LIT, newString(strDup(text)), startline, startindex));
                }
                break;
            case ';':
                
                break;
        }
    }
    if (es != NULL) {
        tempes = es->next;
        free(es);
        es = tempes;
        if (es != NULL) { // if there wasn't an unmatched parenthesis
            tempes = es->next; // store any lingering stack items
            free(es); // free the memory of the unused stack item
            if (tempes != NULL || situation == 0) { // if there are more items on the stack (i.e. if there is at least one unclosed parenthesis)
                addError(newErrorlist(ERR_UNCLOSED_PAREN, newString(strDup(text)), startline, startindex));
                head->type = TYPE_NIL; // set the head to nil since the expression hasn't been parsed
                while (tempes != NULL) { // free from memory the lingering stack items
                    es = tempes->next;
                    free(tempes);
                    tempes = es;
                }
            }
        }
    }
    tempexpr = head->next;
    free(head);
    head = tempexpr;
    if (head == NULL) { // if nothing was parsed
        head = newExpressionOfType(TYPE_NIL); // set the head to a dummy value
    } else if (head->ev.expval == NULL) { // if the head expression was never filled with content
        head->type = TYPE_NIL; // mark the head as nil so it isn't evaluated
    }
    return head;
}

/*! Parses the given piece of text (i.e. no delimeters) and returns a list containing the parsed expressions
    @param text         the piece of text to parse
    @param tail         a pointer to an expression pointer that stores the end of the parsed expression so the code after this function knows where to start
    @param startline    the starting line of the parsed expression for error logging purposes
    @param startindex   the starting index of the parsed expression for error logging purposes
    @param ascii        the ascii value of the expression delimeter, indicating whether the expression is regular, lazy, or an array
    @param iscont       whether or not the piece is a continuation of an expression and can consequently skip the validation process
    @return             the head of the list of expressions
    @todo               errors
*/
expression* parsePiece (char* text, expression** tail, linenum startline, uint startindex, int ascii, int iscont) {
    expression* expr = newExpressionOfType(TYPE_NIL); // create a new expression with a value of nil
    expression* head = expr; // store the head of the list so it can be returned at the end
    expression* last = NULL; // used to trim off the last nil expression after parsing the whole piece
    int size = strlen(text); // the size of the given text
    int validexpr = iscont; // whether or not the expression is valid
    int maybenum = 0; // whether or not the expression may be a number
    int lazy = (ascii == '[' || ascii == ']'); // whether or not the expression is lazy
    uint i = 0;
    i = parseWhitespace(text, i, size); // skip any whitespace
    uint index = i; // the index of the last significant character
    i = parseExprForSign(text, i, size, expr); // determine if the first argument is a sign-specified integer and set its type if it is
    for (; i < size; i++) {
        switch(text[i]) {
            case ' ':
                // if the function has been validated or the current expression is a string and therefore valid function type
                if (validexpr || ascii == '{' || ascii == '}') {
                    // if the expression is a string literal then the wrapper quotation marks need to be removed
                    if (expr->type == TYPE_STR && expr->flag == EFLAG_NONE) {
                        expr = storeExprValue(expr, text, index + 1, i - 1, &last, lazy && validexpr);
                    } else { // if no characters need to be removed
                        expr = storeExprValue(expr, text, index, i, &last, lazy && validexpr);
                    }
                    validexpr = 1; // the expression is valid (from a syntactical perspective)
                } else { // if the expression isn't a valid function type
                    addError(newErrorlist(ERR_UNDEFINED_FUN, newString(strDup(text)), startline, startindex)); // add an undefined function type error
                    i = size; // end the loop prematurely since the rest of the expression doesn't need to be evaluated
                    expr->type = TYPE_NIL; // set the current expression to nil so it's not evaluated
                    break;
                }
                i = parseWhitespace(text, i + 1, size) - 1; // skip any whitespace
                index = i + 1; // advance the index to the next unparsed position
                i = parseExprForSign(text, i, size, expr); // determine if the next argument is a sign-specified integer and set its type if it is
                break;
            case '\"':
                expr->type = TYPE_STR; // set the expression to a string literal
                i = parseStringLiteral(text, i + 1, size); // skip the string literal
                break;
            case '\'':
                expr->type = TYPE_STR; // parse the expression as a string (it's translated into a number in storeExprValue)
                expr->flag = EFLAG_SYMB; // mark the expression as a symbol
                break;
            default: // insignificant character (i.e. not a control character)
                // if the expression is currently assumed to be nil, set it to an integer if the current character is numeric
                if ((expr->type == TYPE_NIL || maybenum == 1) && text[i] >= '0' && text[i] <= '9') {
                    maybenum = 0;
                    expr->type = TYPE_INT;
                    expr->flag = EFLAG_NONE;
                    break;
                }
                // if the expression is currently assumed to be nil or an integer, set it to a float if the current character is a period
                if ((expr->type == TYPE_NIL || expr->type == TYPE_INT) && text[i] == '.') {
                    expr->type = TYPE_FLO;
                    break;
                }
                // if the expression is currently assumed to be an integer, mark it as needing a base conversion by storing the index of the colon
                if (text[i] == ':') {
                    if (expr->type == TYPE_NIL) {
                        expr->type = TYPE_STR;
                        expr->flag = EFLAG_VAR;
                    } else if (expr->type == TYPE_INT || expr->type == TYPE_FLO) {
                        expr->flag = i;
                    }
                    break;
                }
                // if the expression is currently assumed to be nil, an integer, or a float and the current character isn't numeric
                if ((expr->type == TYPE_NIL || expr->type == TYPE_INT || expr->type == TYPE_FLO)
                    && ((text[i] > ' ' && text[i] <= '-')
                        || (text[i] == '/')
                        || (text[i] > ':' && text[i] <= 'Z')
                        || (text[i] == '\\')
                        || (text[i] > ']' && text[i] <= 'z')
                        || (text[i] == '|')
                        || (text[i] == '~')
                        )) {
                            validexpr = 1; // the expression is valid (from a syntactical perspective)
                            maybenum = 1; // the expression may be a number
	                        expr->type = TYPE_STR; // set the current expression's type to a string variable
	                        expr->flag = EFLAG_VAR;
                }
                break;
        }
    }
    if (i <= size && i > index) { // if there is something to evaluate
        // if the expression is a string literal then the wrapper quotation marks need to be removed
        if (expr->type == TYPE_STR && expr->flag == EFLAG_NONE) {
            expr = storeExprValue(expr, text, index + 1, i - 1, &last, lazy);
        } else { // if no characters need to be removed
            expr = storeExprValue(expr, text, index, i, &last, lazy);
        }
    }
    if (validexpr && head != expr) { // if the parsed expression is valid
        freeExpr(expr); // remove the trailing nil expression and free it from memory
        if (last != NULL) {
            last->next = NULL;
        }
    }
    free(text); // free the substring from memory
    *tail = last; // set the tail to the end of the parsed piece
    if (last != NULL && last->next != NULL && last->next->type == TYPE_NIL) {
    	freeExpr(last->next);
    	last->next = NULL;
    }
    return head;
}

uint parseWhitespace (char* text, uint index, uint end) {
    while (index < end && text[index] == ' ') {
        ++index;
    }
    return index;
}

/*! Determines if the given character is a + or - sign, indicating a sign-specified integer, and if so sets its type to an integer
    @param text     the character array containing the character in question
    @param index    the index of the character in the text
    @param expr     the expression whose type is set to int if the given character is a + or - sign
    @return         nothing
*/
uint parseExprForSign (char* text, uint index, uint size, expression* expr) {
    // if the character at the given index is a + or - sign
    if ((text[index] == '+' || text[index] == '-')) {
        if (index + 1 < size && text[index + 1] != ' ') { // if the + or - is not an independent expression
            expr->type = TYPE_INT; // set the expression's type to an integer
            return index + 1; // return an increased index so this character does not get parsed again
        }
    }
    return index;
}

/*! Skips past all text until an end quotation is found
    @param text     the character array containing the string literal
    @param index    the index to start parsing at
    @return         the new index to starting parsing at after the function ends
*/
uint parseStringLiteral (char* text, uint index, uint end) {
    while (index < end && text[index] != '"') { // while the character isn't a "
        ++index;
    }
    return index;
}

/*! Stores the given child expression in the given parent expression, accounting for differences between regular and lazy expressions
    @param parent   the parent expression
    @param child    the child expression
    @return         nothing
*/
void storeChildExpression (expression* parent, expression* child) {
    if (parent->type == TYPE_EXP) { // if the expression is lazy
        parent->ev.expval = child; // store the child expression in the expval field of the expression
    } else {
        parent->ev.lazval->expval = child; // store the lazy expression into the lazval field of the expression
    }
}

/*! Evaluates the given list of expressions and returns the finished result
    @param head     the head of the list of expressions to be evaluated
    @return         the expression representing the result of the evaluation
*/
expression* evaluate (expression* head) {
	expression* result = NULL;
    if (head != NULL) {
        if (head->type == TYPE_EXP) {
            result = evaluateExp(head);
        } else if (head->type == TYPE_INT) {
        	result = evaluateInt(head);
        } else if (head->type == TYPE_FLO) {
        	result = evaluateFlo(head);
        } else if ((head->type == TYPE_STR && head->flag == EFLAG_VAR) || head->type == TYPE_FUN) {
        	result = evaluateFun(head);
        } else if (head->type == TYPE_ARR) {
            result = evaluateArr(head);
        } else if (head->type == TYPE_DAT) {
        	result = evaluateDat(head);
        } else if (head->type == TYPE_OBJ) {
            result = evaluateObj(head);
        } else if (head->type == TYPE_TYP) {
        	result = evaluateTyp(head);
        } else if (head->type >= TYPE_COMP_START) {
        	result = evaluateCompType(head);
        }
    }
    if (result == NULL) {
    	result = copyExpressionNR(head);
    }
    return result;
}

/* Evaluates the given container expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateExp (expression* head) {
	setEnvironment(); // setup a new environment with a blank slate
	expression* result = NULL;
	while (1) {
		expression* next = head->next; // store the next expression to be evaluated later
		result = evaluateArgument(head); // evaluate the expression's contents
		if (next == NULL) {
			break;
		} else {
			if (head != result) {
				freeExpr(result);
			}
			head = next;
		}
	}
    resetEnvironment(); // clear any variables created in the current environment and reset to the previous one
    return result;
}

/*! Evaluates the given lazy expression and returns the finished result
    @param head     the expression to be evaluated
    @return         the expression representing the result of the evaluation
*/
expression* evaluateLaz (expression* head) {
    if (head->type == TYPE_LAZ) { // if the expression is a lazy expression
        return evaluate(head->ev.lazval->expval); // evaluate the expression as a regular expression
    } else { // if the expression isn't a lazy expression then just evaluate it
        return evaluate(head);
    }
}

/* Evaluates the given integer expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateInt (expression* head) {
	return copyExpressionNR(head);
}

/* Evaluates the given float expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateFlo (expression* head) {
	return copyExpressionNR(head);
}

/* Evaluates the given variable string or function expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateFun (expression* head) {
	expression* result;
	int numargs = numArgs(head);
	expression* args[numargs];
	fillArgs(args, head);
	tap_fun_search tfs = findFunction(head, args, numargs);
	if (tfs.found) {
		if (tfs.prim) {
			result = callPrimFun(tfs.funs.prim_fun, args, numargs);
		} else {
			if (validFunCall(tfs.funs.fun, head, args, numargs)) {
				result = callFun(tfs.funs.fun, args, numargs);
			} else {
				result = newExpressionNil();
			}
		}
	} else {
        result = newExpressionNil();
	}
	int i;
	for (i = 0; i < numargs; ++i) {
        freeExprNR(args[i]);
    }
    return result;
}

/* Returns the number of expression arguments in the given list
	@param head	the list of expressions to count
	@return		the number of arguments found
*/
int numArgs (expression* head) {
	int numargs = 0;
    expression* expr = head->next;
    while (expr != NULL) {
        ++numargs;
        expr = expr->next;
    }
    return numargs;
}

/* Fills in the given arguments array with the given list of expressions
	@param args	the array of arguments to fill
	@param head	the list of expressions to pull from
	@return		the expression representing the result of the evaluation
*/
void fillArgs (expression* args[], expression* head) {
	expression* expr = head->next;
    int i = 0;
    while (expr != NULL) {
        expression* next = expr->next;
        args[i++] = evaluateArgument(expr);
        expr = next;
    }
}

/* Finds the function corresponding to the given expression head and returns its data
	@param head		the expression containing the function (string variable or tap function)
	@param args		the array of arguments to match the type signature to
	@param numargs	the number of arguments in the array
	@return			the tap_fun_search data associated with the found function
*/
tap_fun_search findFunction (expression* head, expression* args[], int numargs) {
	int cenv = cenvironment;
    int found = head->type == TYPE_FUN;
    tap_prim_fun* prim_fun = NULL;
    tap_fun* fun = NULL;
    while (!found && cenv >= 0) {
        hashlist* hl1 = lookupHashes(environments[cenv]->variables, head->ev.strval->content);
        hashlist* hl2 = hl1;
        while (hl1 != NULL) {
            typelist* types;
            int minargs;
            if (cenv == 0) { // if the current environment is the root one containing primitive functions
                prim_fun = hl1->value;
                if (prim_fun->minargs > numargs || (prim_fun->maxargs != ARGLEN_INF && prim_fun->maxargs < numargs)) {
                    hl1 = hl1->next;
                    continue;
                }
                types = prim_fun->types;
                minargs = prim_fun->minargs;
            } else {
                expression* expr = (expression*)(hl1->value);
                if (expr->type == TYPE_FUN) {
                    fun = expr->ev.funval;
                    if (fun->minargs > numargs || (fun->maxargs != ARGLEN_INF && fun->maxargs < numargs)) {
                        hl1 = hl1->next;
                        continue;
                    }
                    types = NULL;
                    minargs = fun->minargs;
                } else {
                    continue;
                }
            }
            int validtypes = 1;
            int i;
            for (i = 0; i < minargs; ++i) {
                if (cenv == 0) {
                    if (i > 0) {
                        types = types->next;
                    }
                } else {
                    types = fun->args[i]->types;
                }
                int validtype = 0;
                while (types != NULL) {
                    if (types->type == TYPE_UNK || args[i]->type == types->type) { // if an argument's type doesn't match the function's requirements
                        validtype = 1;
                        break;
                    }
                    types = types->next;
                }
                if (!validtype) {
                    validtypes = 0;
                    break;
                }
            }
            if (validtypes) {
                found = 1;
                break;
            }
            hl1 = hl1->next;
        }
        while (hl2 != NULL) { // free the memory used to store the hashlist
            hl1 = hl2->next;
            free(hl2);
            hl2 = hl1;
        }
        cenv = environments[cenv]->parent; // since no function was found, try searching in the parent environment
    }
    tap_fun_search tfs;
    tfs.found = found;
    int prim = prim_fun != NULL;
    tfs.prim = prim;
    if (prim) {
    	tfs.funs.prim_fun = prim_fun;
    } else {
    	tfs.funs.fun = fun;
    }
    return tfs;
}

/* Calls the given primitive function with the given arguments
	@param prim_fun	the primitive function to call
	@param args		the array of arguments to pass to the function
	@param num_args	the number of arguments in the array
	@return			the function call's resulting expression
*/
expression* callPrimFun (tap_prim_fun* prim_fun, expression* args[], int numargs) {
	setEnvironment(); // set up a new environment with a blank slate
    expression* result = newExpressionNil();
    datatype returntype = TYPE_NIL;
    prim_fun->address(args, numargs, &(result->ev), &returntype); // call the function, passing it the evaluated arguments and the number of arguments
    resetEnvironment();
    result->type = returntype;
    return result;
}

/* Returns whether or not the given function is being given valid arguments
	@param fun		the function whose signature to analyze
	@param head		the function's container expression
	@param args		the array of arguments being passed to the function
	@param numargs	the number of arguments in the array
	@return			1 if valid, 0 otherwise
*/
int validFunCall (tap_fun* fun, expression* head, expression* args[], int numargs) {
    if (fun->minargs > numargs || (fun->maxargs != ARGLEN_INF && fun->maxargs < numargs)) {
        addError(newErrorlist(ERR_INVALID_NUM_ARGS, newString(printExpression(head)), 1, 0));
        return 0;
    } else {
    	int i;
        for (i = 0; i < numargs; ++i) {
            typelist* types = fun->args[i]->types;
            int validtype = 0;
            while (types != NULL) {
                if (types->type == TYPE_UNK || args[i]->type == types->type) { // if an argument's type doesn't match the function's requirements
                    validtype = 1;
                    break;
                }
                types = types->next;
            }
            if (!validtype) {
                addError(newErrorlist(ERR_INVALID_ARG, newString(printExpression(head)), 1, 0));
                return 0;
            }
        }
    }
    return 1;
}

/* Calls the given function with the given arguments
	@param fun		the function to call
	@param args		the array of arguments to pass to the function
	@param num_args	the number of arguments in the array
	@return			the function call's resulting expression
*/
expression* callFun (tap_fun* fun, expression* args[], int numargs) {
	int newenv = environments[cenvironment]->numvars > 0;
    if (newenv) { // if this isn't a tail call
        setEnvironment(); // set up a new environment with a blank slate
    }
    int i;
    for (i = 0; i < numargs; ++i) { // add the arguments to the new environment's variables table
        insertUserHash(environments[cenvironment]->variables, fun->args[i]->name->content, args[i]);
    }
    expression* cfunction = newExpressionOfType(TYPE_FUN); // insert the special variable "here" that refers to the current function
    cfunction->ev.funval = fun;
    insertUserHash(environments[cenvironment]->variables, "here", cfunction);
    environments[cenvironment]->numvars += numargs; // indicate how many variables there are in the new environment
    expression* result = evaluateLaz(fun->body); // evaluate the function in the new environment
    if (newenv) {
        resetEnvironment(); // reset the environment to its previous state
    }
    return result;
}

/* Evaluates the given array expression and returns the result
	@param arg	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateArr (expression* head) {
	expression* result;
	expression* indexexpr = evaluateArgument(head->next);
    if (indexexpr->type == TYPE_INT) {
        array* arr = head->ev.arrval;
        int index = indexexpr->ev.intval;
        if (index > arr->start && index < arr->end) {
            expression* elem = copyExpression(arr->content[index]);
            result = elem;
        } else {
            addError(newErrorlist(ERR_OUT_OF_BOUNDS, newString(printExpression(indexexpr)), 1, 1));
            result = newExpressionNil();
        }
    } else {
        addError(newErrorlist(ERR_INVALID_ARG, newString(printExpression(head)), 1, 1));
        result = newExpressionNil();
    }
    return result;
}

/* Evaluates the given date expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateDat (expression* head) {
	return copyExpressionNR(head);
}

/* Evaluates the given object expression and returns the result
	@param arg	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateObj (expression* head) {
	expression* result;
	expression* propname = evaluateArgument(head->next);
    if (propname->type == TYPE_STR) {
        char* pnstr = propname->ev.strval->content;
        property* props = head->ev.objval->props;
        while (props != NULL) {
            if (strcmp(pnstr, props->name) == 0) {
                expression* prop = copyExpression(props->value);
                result = prop;
            }
            props = props->next;
        }
        addError(newErrorlist(ERR_UNDEFINED_PROP, newString(strDup(pnstr)), 1, 1));
        result = newExpressionNil();
    } else {
        addError(newErrorlist(ERR_INVALID_ARG, newString(printExpression(head)), 1, 1));
        result = newExpressionNil();
    }
    return result;
}

/* Evaluates the given type expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateTyp (expression* head) {
	return copyExpressionNR(head);
}

/* Evaluates the given composite type expression and returns the result
	@param head	the expression to be evaluated
	@return		the expression representing the result of the evaluation
*/
expression* evaluateCompType (expression* head) {
	return copyExpressionNR(head);
}

/*! Evaluates the given argument and returns the result
    @param arg  the expression argument to be evaluated
    @return     the expression representing the result of the evaluation
*/
expression* evaluateArgument (expression* arg) {
	expression* result;
    if (arg->type == TYPE_EXP) { // if the argument is an expression
        if (arg->flag == EFLAG_ARR) { // if the expression is an array expression
            int size = 0;
            expression* elem1 = arg->ev.expval;
            while (elem1 != NULL) {
                ++size;
                elem1 = elem1->next;
            }
            array* arr = newArray(size);
            elem1 = arg->ev.expval;
            expression* elem2;
            size = 0;
            while (elem1 != NULL) {
                elem2 = elem1->next;
                arr->content[size++] = evaluateArgument(elem1);
                elem1 = elem2;
            }
            result = newExpressionArr(arr);
        } else { // if the expression isn't an array expression
            result = evaluate(arg->ev.expval); // replace the expression parameter with its evaluated equivalent
        }
    } else if (arg->type == TYPE_STR && arg->flag == EFLAG_VAR) {
        string* var = arg->ev.strval;
        result = copyExpression(getVarValue(var->content));
        if (result == NULL) {
            addError(newErrorlist(ERR_UNDEFINED_VAR, copyString(var), 0, 0));
            return newExpressionNil();
        }
    } else {
    	result = copyExpression(arg);
    }
    return result;
}

/*! Prints the given expression in a user friendly format
    @param result   the expression to be printed
    @return         a string representing the printed expression
*/
char* printExpression (expression* result) {
    char* output = NULL; // the result string
    int size;
    datatype typ;
    if (result == NULL) {
        typ = TYPE_UNK;
    } else {
        typ = result->type;
    }
    switch (typ) { // depending on the expression's type
        case TYPE_NIL:
            output = allocate(6); // allocate the required memory to copy the static string (in this case "[nil]" but this comment is for each case)
            strcpy(output, "[nil]"); // assign the result string the duplicated value of the static string
            break;
        case TYPE_EXP:
            output = allocate(13);
            strcpy(output, "[expression]");
            break;
        case TYPE_LAZ:
            output = allocate(18);
            strcpy(output, "[lazy expression]");
            break;
        case TYPE_INT:
            size = snprintf(NULL, 0, "%ld", result->ev.intval); // find out how many characters the integer will take up when cast as a string
            output = allocate(size + 1);
            sprintf(output, "%ld", result->ev.intval);
            break;
        case TYPE_FLO:
            size = snprintf(NULL, 0, "%f", result->ev.floval); // find out how many characters the float will take up when cast as a string
            output = allocate(size + 1);
            sprintf(output, "%f", result->ev.floval);
            break;
        case TYPE_STR:
            output = strDup(result->ev.strval->content); // duplicate the content string to avoid accidentally deleting it
            break;
        case TYPE_ARR:
            output = allocate(8);
            strcpy(output, "[array]");
            break;
        case TYPE_DAT:
            output = printDate(result->ev.datval, "%M/%D/%Y %H:%U:%S %P");
            break;
        case TYPE_OBJ:
            output = allocate(9);
            strcpy(output, "[object]");
            break;
        case TYPE_FUN:
            output = allocate(11);
            strcpy(output, "[function]");
            break;
        case TYPE_TYP:
            output = printType(result->ev.intval);
            break;
        case TYPE_UNK:
        default:
            output = allocate(8);
            strcpy(output, "[error]");
            break;
    }
    return output;
}

/*! Prints the errors generated thus far in a user friendly format
    @return         a string representing the list of generated errors
*/
char* printErrors () {
    errorlist* error = errors;
    int errornum = 0;
    int size = 9;
    while (error != NULL) {
        size += 29 + stringSize(errornum) + errorCodeStringSize(error->code) + stringSize(error->line) + stringSize(error->index) + error->message->size;
        error = error->next;
    }
    char* errortext = (char*)allocate(size);
    strcpy(errortext, "Errors:\n");
    int pos = 8;
    error = errors;
    while (error != NULL) {
        char* ecs = errorCodeString(error->code);
        pos += sprintf(errortext + pos, "\tError %d: %s, line %d, index %d: \"%s\"\n", errornum++, ecs, error->line, error->index, error->message->content);
        free(ecs);
        error = error->next;
    }
    errortext[pos] = '\0';
    return errortext;
}

/*! Adds the given error to the currently active error list
    @param error    the errorlist pointer to add to the error list
    @return         nothing
*/
inline void addError (errorlist* error) {
    if (errors == NULL) {
        errors = error;
        cerror = errors;
    } else {
        cerror->next = error;
        cerror = cerror->next;
    }
}

/*! Returns the string description of the given error code
    @param code     the integer error code to return the string version of
    @return         a string representing the list of generated errors
*/
char* errorCodeString (uint code) {
    char* desc;
    switch (code) {
        case ERR_GENERAL:
            desc = "general error";
            break;
        case ERR_UNCLOSED_PAREN:
            desc = "unclosed parenthesis";
            break;
        case ERR_UNMATCHED_PAREN:
            desc = "unmatched parenthesis";
            break;
        case ERR_UNCLOSED_STR_LIT:
            desc = "unclosed string literal";
            break;
        case ERR_INVALID_ARG:
            desc = "invalid argument";
            break;
        case ERR_INVALID_NUM_ARGS:
            desc = "invalid number of arguments";
            break;
        case ERR_UNDEFINED_VAR:
            desc = "undefined variable";
            break;
        case ERR_UNDEFINED_FUN:
            desc = "undefined function (with given arguments)";
            break;
        case ERR_UNDEFINED_TYP:
            desc = "undefined type";
            break;
        case ERR_UNDEFINED_PROP:
            desc = "undefined property (for the given type)";
            break;
        case ERR_OUT_OF_BOUNDS:
            desc = "index out of bounds";
            break;
        default:
            desc = "unknown error";
            break;
    }
    return strDup(desc);
}

/*! Returns the size (number of bytes) of the string description of the given error code
    @param code     the integer error code to return the string version of
    @return         a string representing the list of generated errors
*/
int errorCodeStringSize (uint code) {
    switch (code) {
        case ERR_GENERAL:
            return strlen("general error");
        case ERR_UNCLOSED_PAREN:
            return strlen("unclosed paranthesis");
        case ERR_UNMATCHED_PAREN:
            return strlen("unmatched parenthesis");
        case ERR_UNCLOSED_STR_LIT:
            return strlen("unclosed string literal");
        case ERR_INVALID_ARG:
            return strlen("invalid argument");
        case ERR_INVALID_NUM_ARGS:
            return strlen("invalid number of arguments");
        case ERR_UNDEFINED_VAR:
            return strlen("undefined variable");
        case ERR_UNDEFINED_FUN:
            return strlen("undefined function (with given arguments)");
        case ERR_UNDEFINED_TYP:
            return strlen("undefined type");
        case ERR_UNDEFINED_PROP:
            return strlen("undefined property (for the given type)");
        case ERR_OUT_OF_BOUNDS:
            return strlen("index out of bounds");
        default:
            return strlen("unknown error");
    }
}

/*! Add the given value linked to the given key to the current environment
    @param key      the key to be associated with the given value
    @param value    the value associated with the given key
    @return         nothing
*/
void addToEnvironment (char* key, expression* value) {
    insertUserHash(environments[environments[cenvironment]->parent]->variables, key, value);
}

/*! Sets up a new environment with no extant variables
    @return     nothing
*/
void setEnvironment () {
    environments[cenvironment + 1]->parent = cenvironment; // set the new environment's parent to the previous environment
    ++cenvironment; // increment to a new, blank environment
}

/*! Resets the current environment for use next time and pops to the previous environment
    @return     nothing
*/
void resetEnvironment () {
    int parent = environments[cenvironment]->parent;
    clearHash(environments[cenvironment]->variables); // clear the environment
    environments[cenvironment]->parent = -1; // reset its parent index to -1, indicating no parent
    cenvironment = parent; // retreat to the previous environment
}

/*! Gets the expression value mapped to the given string name
    @param name     the string name to search with
    @return         the value mapped to the name
*/
expression* getVarValue (char* name) {
    expression* found = NULL;
    int cenv = cenvironment;
    while (cenv >= 0) { // while there are more environments to check
        hashlist* list1 = (hashlist*)lookupHashes(environments[cenv]->variables, name); // look for the value with the given name/key
        hashlist* list2;
        if (list1 != NULL) { // if a value was found
            if (list1->flag == HFLAG_USER) { // if the value is a user variable (as opposed to a primitive function)
                found = list1->value; // temporarily store the found value while the hash list memory is deleted
            }
            do { // free the memory allocated for the hast list
                list2 = list1->next; // store the next item in the hash list
                free(list1); // free the current hash list element
                list1 = list2; // advance to the next hash list element
            } while (list1 != NULL);
            return found; // return the found value
        }
        cenv = environments[cenv]->parent; // since the value wasn't found check the parent environment for it
    }
    return NULL;
}

/*! If the given expression is a regular or lazy expression, returns the expression value and otherwise returns null
    @param expr     the expression to retrieve the expression value from
    @return         the expression value or null
*/
expression* getExprValue (expression* expr) {
    if (expr->type == TYPE_EXP) { // if the expression is regular
        return expr->ev.expval; // return its expression value
    } else if (expr->type == TYPE_LAZ) { // if the expression is lazy
        return expr->ev.lazval->expval; // return its expression value
    } else { // if the expression doesn't contain an expression value
        return NULL;
    }
}

/*! Stores the given substring of text into the given expression and returns the next expression pointer
    @param expr     the expression to store the substring into
    @param text     the text to extract the substring from
    @param start    the start index of the substring
    @param end      the end index of the substring
    @return         a new expression pointer with a value of nil that the old expression links to
*/
expression* storeExprValue (expression* expr, char* text, int start, int end, expression** last, int lazy) {
    if (end - start > 0) { // if there is something to evaluate
        char* strval = substr(text, start, end); // store the expression's raw value
        if (expr->type == TYPE_INT) { // if the expression is an int
            int intval; // a temporary variable to store the integer result in
            if (expr->flag > EFLAG_CONVBASE) { // if the expression needs its base converted
                char* integer = substr(strval, 0, expr->flag - 2);
                char* base = substr(text, expr->flag + 1, end);
                intval = strtol(integer, NULL, atoi(base)); // convert the integer from ascii to numeric using the given base
                free(integer); // free the substrings from memory
                free(base);
            } else {
                intval = atoi(strval); // convert the integer from ascii to numeric
            }
            expr->ev.intval = intval; // store the integer value in the expression
            free(strval); // free the string value from memory
        } else if (expr->type == TYPE_FLO) { // if the expression is a float
            double floval = atof(strval); // convert the float from ascii to numeric
            expr->ev.floval = floval; // store the float value in the expression
            free(strval); // free the string value from memory
        } else if (expr->type == TYPE_STR) { // if the expression is a string
            if (expr->flag == EFLAG_SYMB) { // if the string is a sybol
                hashsum intval = hashWithSize(INITIAL_SYMBOL_COUNT, strval); // map the symbol's string value to its hashed integer value
                expr->ev.intval = intval; // store the integer value in the expression
                expr->type = TYPE_INT; // set the expression's type to an integer
                free(strval); // free the string value from memory
            } else {
                expr->ev.strval = newString(strval);
                if (expr->flag == EFLAG_NONE && lazy) { // if the string is a variable and is inside a lazy expression
                    //expr->expr->lazval->refs = newExprstack(expr->expr->lazval->refs); // add a reference to it to the lazy expression list
                }
            }
        } else {
            free(strval); // free the string value from memory
        }
        expr->next = newExpressionOfType(TYPE_NIL); // set the new expression to a value of nil
    }
    (*last) = expr;
    return expr->next;
}

/*! Returns the string description of the given integer type
    @param type     the type to print
    @return         the string description equivalent of the given integer type
*/
char* printType (datatype typ) {
    switch (typ) { // depending on the type
        case TYPE_NIL:
            return strDup("::nil"); // assign the result string the duplicated value of the static string
        case TYPE_EXP:
            return strDup("::expression");
        case TYPE_LAZ:
            return strDup("::lazy expression");
        case TYPE_INT:
            return strDup("::integer");
        case TYPE_FLO:
            return strDup("::float");
        case TYPE_STR:
            return strDup("::string");
        case TYPE_ARR:
            return strDup("::array");
        case TYPE_DAT:
            return strDup("::date");
        case TYPE_OBJ:
            return strDup("::object");
        case TYPE_FUN:
            return strDup("::function");
        case TYPE_TYP:
            return strDup("::type");
    }
    int cenv = cenvironment;
    while (cenv >= 0) {
        typedefs* types = environments[cenv++]->types;
        while (types != NULL) {
            if (types->type->id == typ) {
                char* typestr = allocate(strlen(types->type->name) + 3);
                strcpy(typestr, "::");
                strcpy(typestr + 2, types->type->name);
                return typestr;
            }
        }
    }
    return strDup("unknown type");
}

/*! Returns the size of the string description of the given integer type
    @param type     the type to print
    @return         the string description equivalent of the given integer type
*/
int printTypeSize (datatype typ) {
    switch (typ) { // depending on the type
        case TYPE_NIL:
            return 3;
        case TYPE_EXP:
            return 10;
        case TYPE_LAZ:
            return 15;
        case TYPE_INT:
            return 7;
        case TYPE_FLO:
            return 5;
        case TYPE_STR:
            return 6;
        case TYPE_ARR:
            return 5;
        case TYPE_DAT:
            return 4;
        case TYPE_OBJ:
            return 6;
        case TYPE_FUN:
            return 8;
        case TYPE_TYP:
            return 4;
        default:
            return 0;
    }
}

/*! Returns the type corresponding to the given type string (e.g. 'int' returns TYPE_INT) or returns TYPE_UNK if the type is not recognized
    @param type     the type's string name
    @return         the type's integer equivalent
*/
datatype typeFromString (char* type) {
    datatype id = TYPE_UNK;
    int cenv = cenvironment;
    while (cenv >= 0 && id == TYPE_UNK) {
        hashlist* hl1 = lookupHashes(environments[cenv--]->variables, type);
        hashlist* hl2;
        while (hl1 != NULL) {
            if (hl1->flag == HFLAG_USER) {
                expression* expr = (expression*)(hl1->value);
                if (expr->type == TYPE_TYP) {
                    id = expr->ev.intval;
                    break;
                }
            }
            hl2 = hl1->next;
            free(hl1);
            hl1 = hl2;
        }
    }
    return id;
}

/*! Converts the given argument into a string
    @param arg  the argument
    @return     the given argument as a string
*/
char* printArg (argument* arg) {
    int size = 4;
    typelist* at = arg->types;
    while (at != NULL) {
        size += printTypeSize(at->type);
        at = at->next;
    }
    size += arg->name->size;
    char* argstr = (char*)allocate(size);
    argstr[0] = '{';
    int pos = 1;
    at = arg->types;
    while (at != NULL) {
        char* type = printType(at->type);
        strcpy(argstr + pos, type);
        free(type);
        pos += printTypeSize(at->type);
        at = at->next;
    }
    strcpy(argstr + pos, "} ");
    pos += 2;
    strcpy(argstr + pos, arg->name->content);
    pos += arg->name->size;
    argstr[pos] = '\0';
    return argstr;
}

/*! Initializes all of the global variables (e.g. the environments, error list, etc.) needed for interpretation
    @return     nothing
*/
void initializeGlobals () {
    // initialize the random seed using the execution time
    srand(time(NULL));

    // initialize the environments
    environments[0] = newEnvironment(newHashtable(INITIAL_ROOT_ENV_SIZE), -1);
    int i;
    for (i = 1; i < INITIAL_ENV_COUNT; ++i) { // for each environment
        environments[i] = newEnvironment(newHashtable(INITIAL_ENV_SIZE), -1); // initialize the environment and set its parent to -1, indicating no parent
    }
    cenvironment = 0; // set the current environment to 0, the top-most environment

    // initialize the primitive functions
    hashtable* cenv = environments[cenvironment]->variables;

    insertPrimHash(cenv, "int", newPrimFunction(&prim_nInt, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_nInt, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_nStr, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_nStr, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_nTyp, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_nTyp, 1, 1, newTypelist(TYPE_NIL)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_nTyp, 1, 1, newTypelist(TYPE_NIL)));

    insertPrimHash(cenv, "eval", newPrimFunction(&prim_lEval, 1, 1, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "function", newPrimFunction(&prim_lFunction, 2, 2, newTypelistWithNext(TYPE_LAZ, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "lambda", newPrimFunction(&prim_lFunction, 2, 2, newTypelistWithNext(TYPE_LAZ, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "&&", newPrimFunction(&prim_lAnd, 1, ARGLEN_INF, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "||", newPrimFunction(&prim_lOr, 1, ARGLEN_INF, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "^^", newPrimFunction(&prim_lXor, 1, ARGLEN_INF, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "if", newPrimFunction(&prim_lIf, 2, ARGLEN_INF, newTypelistWithNext(TYPE_LAZ, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_lTyp, 1, 1, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_lTyp, 1, 1, newTypelist(TYPE_LAZ)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_lTyp, 1, 1, newTypelist(TYPE_LAZ)));

    insertPrimHash(cenv, "error", newPrimFunction(&prim_iError, 2, 2, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_STR))));
    insertPrimHash(cenv, "+", newPrimFunction(&prim_iAdd, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "-", newPrimFunction(&prim_iSub, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "*", newPrimFunction(&prim_iMul, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "/", newPrimFunction(&prim_iDiv, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "%", newPrimFunction(&prim_iMod, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "**", newPrimFunction(&prim_iPow, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "sqrt", newPrimFunction(&prim_iSqrt, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "log", newPrimFunction(&prim_iLog, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "abs", newPrimFunction(&prim_iAbs, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "max", newPrimFunction(&prim_iMax, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "min", newPrimFunction(&prim_iMin, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "round", newPrimFunction(&prim_iRound, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "ceil", newPrimFunction(&prim_iCeil, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "floor", newPrimFunction(&prim_iFloor, 1, 1, newTypelist(TYPE_INT)));
    ///insertPrimHash(cenv, "scientific", newPrimFunction(&prim_iScientific, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "sin", newPrimFunction(&prim_iSin, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "cos", newPrimFunction(&prim_iCos, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "tan", newPrimFunction(&prim_iTan, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "asin", newPrimFunction(&prim_iAsin, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "acos", newPrimFunction(&prim_iAcos, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "atan", newPrimFunction(&prim_iAtan, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "atan2", newPrimFunction(&prim_iAtan2, 2, 2, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "sinh", newPrimFunction(&prim_iSinh, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "cosh", newPrimFunction(&prim_iCosh, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "tanh", newPrimFunction(&prim_iTanh, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "radians", newPrimFunction(&prim_iRadians, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "degrees", newPrimFunction(&prim_iDegrees, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "~", newPrimFunction(&prim_iBnot, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "&", newPrimFunction(&prim_iBand, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "|", newPrimFunction(&prim_iBor, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "^", newPrimFunction(&prim_iBxor, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "<<", newPrimFunction(&prim_iLshift, 2, 2, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, ">>", newPrimFunction(&prim_iRashift, 2, 2, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, ">>>", newPrimFunction(&prim_iRlshift, 2, 2, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "!", newPrimFunction(&prim_iLnot, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "&&", newPrimFunction(&prim_iLand, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "||", newPrimFunction(&prim_iLor, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "^^", newPrimFunction(&prim_iLxor, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "<", newPrimFunction(&prim_iLess, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "<=", newPrimFunction(&prim_iLequal, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "==", newPrimFunction(&prim_iEqual, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "!=", newPrimFunction(&prim_iNequal, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, ">=", newPrimFunction(&prim_iMequal, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, ">", newPrimFunction(&prim_iMore, 1, ARGLEN_INF, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "if", newPrimFunction(&prim_iIf, 2, ARGLEN_INF, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "random", newPrimFunction(&prim_iRand, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "seed-random", newPrimFunction(&prim_iSrand, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "from-ascii", newPrimFunction(&prim_iFascii, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "bool", newPrimFunction(&prim_iBoo, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "ascii", newPrimFunction(&prim_iAscii, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "int", newPrimFunction(&prim_iInt, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_iInt, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "flo", newPrimFunction(&prim_iFlo, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "float", newPrimFunction(&prim_iFlo, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_iStr, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_iStr, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "arr", newPrimFunction(&prim_iArr, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "array", newPrimFunction(&prim_iArr, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_iTyp, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_iTyp, 1, 1, newTypelist(TYPE_INT)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_iTyp, 1, 1, newTypelist(TYPE_INT)));

    insertPrimHash(cenv, "+", newPrimFunction(&prim_fAdd, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "-", newPrimFunction(&prim_fSub, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "*", newPrimFunction(&prim_fMul, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "/", newPrimFunction(&prim_fDiv, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "**", newPrimFunction(&prim_fPow, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "sqrt", newPrimFunction(&prim_fSqrt, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "log", newPrimFunction(&prim_fLog, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "abs", newPrimFunction(&prim_fAbs, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "max", newPrimFunction(&prim_fMax, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "min", newPrimFunction(&prim_fMin, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "round", newPrimFunction(&prim_fRound, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "ceil", newPrimFunction(&prim_fCeil, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "floor", newPrimFunction(&prim_fFloor, 1, 1, newTypelist(TYPE_FLO)));
    ///insertPrimHash(cenv, "scientific", newPrimFunction(&prim_fScientific, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "sin", newPrimFunction(&prim_fSin, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "cos", newPrimFunction(&prim_fCos, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "tan", newPrimFunction(&prim_fTan, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "asin", newPrimFunction(&prim_fAsin, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "acos", newPrimFunction(&prim_fAcos, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "atan", newPrimFunction(&prim_fAtan, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "atan2", newPrimFunction(&prim_fAtan2, 2, 2, newTypelistWithNext(TYPE_FLO, newTypelist(TYPE_FLO))));
    insertPrimHash(cenv, "sinh", newPrimFunction(&prim_fSinh, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "cosh", newPrimFunction(&prim_fCosh, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "tanh", newPrimFunction(&prim_fTanh, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "radians", newPrimFunction(&prim_fRadians, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "degrees", newPrimFunction(&prim_fDegrees, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "<", newPrimFunction(&prim_fLess, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "<=", newPrimFunction(&prim_fLequal, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "==", newPrimFunction(&prim_fEqual, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "!=", newPrimFunction(&prim_fNequal, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, ">=", newPrimFunction(&prim_fMequal, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, ">", newPrimFunction(&prim_fMore, 1, ARGLEN_INF, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "int", newPrimFunction(&prim_fInt, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_fInt, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "flo", newPrimFunction(&prim_fFlo, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "float", newPrimFunction(&prim_fFlo, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_fStr, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_fStr, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "arr", newPrimFunction(&prim_fArr, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "array", newPrimFunction(&prim_fArr, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_fTyp, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_fTyp, 1, 1, newTypelist(TYPE_FLO)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_fTyp, 1, 1, newTypelist(TYPE_FLO)));

    insertPrimHash(cenv, "set", newPrimFunction(&prim_sSet, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "new-type", newPrimFunction(&prim_sNewtype, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "print", newPrimFunction(&prim_sPrint, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "copy", newPrimFunction(&prim_sCopy, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "size", newPrimFunction(&prim_sSize, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "char", newPrimFunction(&prim_sChar, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "substr", newPrimFunction(&prim_sSubstr, 2, 3, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT)))));
    insertPrimHash(cenv, "find", newPrimFunction(&prim_sFind, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "find-last", newPrimFunction(&prim_sFindlast, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "find-all", newPrimFunction(&prim_sFindall, 2, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "contains", newPrimFunction(&prim_sContains, 1, 2, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+", newPrimFunction(&prim_sConcat, 1, ARGLEN_INF, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "concat", newPrimFunction(&prim_sConcat, 1, ARGLEN_INF, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "replace", newPrimFunction(&prim_sReplace, 3, 3, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_STR)))));
    ///insertPrimHash(cenv, "replace-at", newPrimFunction(&prim_sReplaceat, 3, 4, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT)))));
    insertPrimHash(cenv, "insert", newPrimFunction(&prim_sInsert, 3, 3, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_STR)))));
    ///insertPrimHash(cenv, "insert-at", newPrimFunction(&prim_sInsertat, 3, 3, newTypelist(TYPE_STR, newTypelistWithNext(TYPE_STR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "-", newPrimFunction(&prim_sRemove, 3, 3, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_UNK, newTypelist(TYPE_UNK)))));
    insertPrimHash(cenv, "remove", newPrimFunction(&prim_sRemove, 3, 3, newTypelistWithNext(TYPE_STR, newTypelistWithNext(TYPE_UNK, newTypelist(TYPE_UNK)))));
    ///insertPrimHash(cenv, "remove-at", newPrimFunction(&prim_sRemoveat, 2, 3, newTypelist(TYPE_STR, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "reverse", newPrimFunction(&prim_sReverse, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "upper-case", newPrimFunction(&prim_sUpper, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "lower-case", newPrimFunction(&prim_sLower, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "sentence-case", newPrimFunction(&prim_sSentence, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "title-case", newPrimFunction(&prim_sTitle, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "int", newPrimFunction(&prim_sInt, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_sInt, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "flo", newPrimFunction(&prim_sFlo, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "float", newPrimFunction(&prim_sFlo, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_sStr, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_sStr, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "arr", newPrimFunction(&prim_sArr, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "array", newPrimFunction(&prim_sArr, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "dat", newPrimFunction(&prim_sDat, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "date", newPrimFunction(&prim_sDat, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_sTyp, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_sTyp, 1, 1, newTypelist(TYPE_STR)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_sTyp, 1, 1, newTypelist(TYPE_STR)));

    ///insertPrimHash(cenv, "@", newPrimFunction(&prim_aValue, 2, 3, newTypelistWithNext(TYPE_ARR, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_UNK)))));
    ///insertPrimHash(cenv, "@@", newPrimFunction(&prim_aValues, 3, 4, newTypelistWithNext(TYPE_ARR, newTypelistWithNext(TYPE_INT, newTypelistWithNext(TYPE_INT, newTypelist(TYPE_UNK)))));
    insertPrimHash(cenv, "size", newPrimFunction(&prim_aSize, 1, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+", newPrimFunction(&prim_aConcat, 1, ARGLEN_INF, newTypelist(TYPE_ARR)));
    /**insertPrimHash(cenv, "-", newPrimFunction(&prim_aRemove, 1, ARGLEN_INF, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "*", newPrimFunction(&prim_aMultiply, 2, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_UNK))));
    insertPrimHash(cenv, "trim-left", newPrimFunction(&prim_aTriml, 2, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "trim-right", newPrimFunction(&prim_aTrimr, 2, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "reverse", newPrimFunction(&prim_aReverse, 1, 1, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "sort", newPrimFunction(&prim_aSort, 1, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_FUN))));
    insertPrimHash(cenv, "map", newPrimFunction(&prim_aMap, 2, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_FUN))));
    insertPrimHash(cenv, "filter", newPrimFunction(&prim_aFilter, 2, 2, newTypelistWithNext(TYPE_ARR, newTypelist(TYPE_FUN))));
    insertPrimHash(cenv, "accum", newPrimFunction(&prim_aAccum, 3, 3, newTypelistWithNext(TYPE_ARR, newTypelistWithNext(TYPE_FUN, newTypelist(TYPE_UNK)))));*/
    insertPrimHash(cenv, "str", newPrimFunction(&prim_aStr, 1, 1, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_aStr, 1, 1, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_aTyp, 1, 1, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_aTyp, 1, 1, newTypelist(TYPE_ARR)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_aTyp, 1, 1, newTypelist(TYPE_ARR)));

    insertPrimHash(cenv, "year", newPrimFunction(&prim_dYear, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "month", newPrimFunction(&prim_dMonth, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "day", newPrimFunction(&prim_dDay, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "hour", newPrimFunction(&prim_dHour, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "minute", newPrimFunction(&prim_dMinute, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "second", newPrimFunction(&prim_dSecond, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "week-of-year", newPrimFunction(&prim_dWeekOfYear, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "week-of-month", newPrimFunction(&prim_dWeekOfMonth, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "day-of-year", newPrimFunction(&prim_dDayOfYear, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "day-of-week", newPrimFunction(&prim_dDayOfWeek, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "leap-year?", newPrimFunction(&prim_dLeapYear, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "days-in-month", newPrimFunction(&prim_dDaysInMonth, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "+years", newPrimFunction(&prim_dAddYears, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+months", newPrimFunction(&prim_dAddMonths, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+days", newPrimFunction(&prim_dAddDays, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+hours", newPrimFunction(&prim_dAddHours, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+minutes", newPrimFunction(&prim_dAddMinutes, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "+seconds", newPrimFunction(&prim_dAddSeconds, 2, 2, newTypelistWithNext(TYPE_DAT, newTypelist(TYPE_INT))));
    insertPrimHash(cenv, "int", newPrimFunction(&prim_dInt, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_dInt, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_dStr, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_dStr, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "dat", newPrimFunction(&prim_dDat, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "date", newPrimFunction(&prim_dDat, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_dTyp, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_dTyp, 1, 1, newTypelist(TYPE_DAT)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_dTyp, 1, 1, newTypelist(TYPE_DAT)));

    insertPrimHash(cenv, "obj", newPrimFunction(&prim_oObj, 1, 1, newTypelist(TYPE_OBJ)));
    insertPrimHash(cenv, "object", newPrimFunction(&prim_oObj, 1, 1, newTypelist(TYPE_OBJ)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_oTyp, 1, 1, newTypelist(TYPE_OBJ)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_oTyp, 1, 1, newTypelist(TYPE_OBJ)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_oTyp, 1, 1, newTypelist(TYPE_OBJ)));

    insertPrimHash(cenv, "str", newPrimFunction(&prim_uStr, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_uStr, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "fun", newPrimFunction(&prim_uFun, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "function", newPrimFunction(&prim_uFun, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_uTyp, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_uTyp, 1, 1, newTypelist(TYPE_FUN)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_uTyp, 1, 1, newTypelist(TYPE_FUN)));

    insertPrimHash(cenv, "new", newPrimFunction(&prim_tNew, 1, 2, newTypelistWithNext(TYPE_TYP, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "obj", newPrimFunction(&prim_tNew, 1, 2, newTypelistWithNext(TYPE_TYP, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "object", newPrimFunction(&prim_tNew, 1, 2, newTypelistWithNext(TYPE_TYP, newTypelist(TYPE_LAZ))));
    insertPrimHash(cenv, "int", newPrimFunction(&prim_tInt, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "integer", newPrimFunction(&prim_tInt, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "str", newPrimFunction(&prim_tStr, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "string", newPrimFunction(&prim_tStr, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "typ", newPrimFunction(&prim_tTyp, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "type", newPrimFunction(&prim_tTyp, 1, 1, newTypelist(TYPE_TYP)));
    insertPrimHash(cenv, "::", newPrimFunction(&prim_tTyp, 1, 1, newTypelist(TYPE_TYP)));

    // initialize default variables
    expression* expr;
    expr = newExpressionOfType(TYPE_NIL);
    insertUserHash(cenv, "nil", expr);
    expr = newExpressionOfType(TYPE_INT);
    expr->ev.intval = 1;
    insertUserHash(cenv, "true", expr);
    expr = newExpressionOfType(TYPE_INT);
    expr->ev.intval = 0;
    insertUserHash(cenv, "false", expr);
    expr = newExpressionOfType(TYPE_INT);
    expr->ev.intval = ERR_GENERAL;
    insertUserHash(cenv, "general-error", expr);
    expr = newExpressionOfType(TYPE_INT);
    expr->ev.intval = ERR_INVALID_ARG;
    insertUserHash(cenv, "inval-arg-error", expr);
    expr = newExpressionOfType(TYPE_INT);
    expr->ev.intval = ERR_INVALID_NUM_ARGS;
    insertUserHash(cenv, "inval-num-args-error", expr);
    expr = newExpressionOfType(TYPE_FLO);
    expr->ev.floval = PI;
    insertUserHash(cenv, "pi", expr);
    expr = newExpressionOfType(TYPE_FLO);
    expr->ev.floval = E;
    insertUserHash(cenv, "e", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_NIL;
    insertUserHash(cenv, "nil", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_EXP;
    insertUserHash(cenv, "exp", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_EXP;
    insertUserHash(cenv, "expression", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_LAZ;
    insertUserHash(cenv, "laz", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_LAZ;
    insertUserHash(cenv, "lazy-expression", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_INT;
    insertUserHash(cenv, "int", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_INT;
    insertUserHash(cenv, "integer", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_FLO;
    insertUserHash(cenv, "flo", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_FLO;
    insertUserHash(cenv, "float", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_STR;
    insertUserHash(cenv, "str", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_STR;
    insertUserHash(cenv, "string", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_ARR;
    insertUserHash(cenv, "arr", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_ARR;
    insertUserHash(cenv, "array", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_DAT;
    insertUserHash(cenv, "dat", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_DAT;
    insertUserHash(cenv, "date", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_OBJ;
    insertUserHash(cenv, "obj", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_OBJ;
    insertUserHash(cenv, "object", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_FUN;
    insertUserHash(cenv, "fun", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_FUN;
    insertUserHash(cenv, "function", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_TYP;
    insertUserHash(cenv, "typ", expr);
    expr = newExpressionOfType(TYPE_TYP);
    expr->ev.intval = TYPE_TYP;
    insertUserHash(cenv, "type", expr);

    // initialize the first ID composite types can be assigned to
    ctypeid = TYPE_COMP_START;

    // initialize the error list
    errors = NULL;
    cerror = errors;

    // initialize the monthdays array, which contains the number of days elapsed in the year by each month
    monthdays[0] = 31;
    monthdays[1] = 59;
    monthdays[2] = 90;
    monthdays[3] = 120;
    monthdays[4] = 151;
    monthdays[5] = 181;
    monthdays[6] = 212;
    monthdays[7] = 243;
    monthdays[8] = 273;
    monthdays[9] = 304;
    monthdays[10] = 334;
    monthdays[11] = 365;
}

/*! Frees all of the memory that global variables have reserved
    @return     nothing
*/
void freeGlobals () {
    int i;
    for (i = 0; i < INITIAL_ENV_COUNT; ++i) { // for every environment
        deleteHash(environments[i]->variables); // delete the environment and its hashtable
        free(environments[i]);
    }
    while (errors != NULL) { // delete the list of errors
        errorlist* error = errors->next;
        free(errors->message->content);
        free(errors->message);
        free(errors);
        errors = error;
    }
    cerror = NULL;
}

