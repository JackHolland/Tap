/*! AppTap.org Tap Processor
    @author Jack Holland <jack@apptap.org>
    @file   casting.c
    @brief  All of the primitive casting functions used by the language
    (C) 2011 Jack Holland. All rights reserved.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "casting.h"
#include "constants.h"
#include "engine.h"
#include "constructors.h"
#include "strings.h"

/*! Tries to cast the given expression to a number
    @param expr     the expression to cast
    @param base     the base to use when casting
    @return         the integer value of the given expression
*/
expression* castToNum (string* string, int base) {
    expression* numexpr = newExpression_t(TYPE_NIL);
    char* content = string->content;
    if (base == 0) {
        base = BASE;
    }
    int ucharmin = 'A';
    int ucharmax = (base - 10) + 'A';
    int lcharmin = 'a';
    int lcharmax = (base - 10) + 'a';
    double result = 0;
    int digit = 1;
    int situation = 0;
    int dot = 0;
    int divideby = 1;
    int tempint;
    int i;
    for (i = string->size - 1; i >= 0; --i) {
        if ((content[i] >= '0' && content[i] <= '9')
            || (content[i] >= ucharmin && content[i] < ucharmax)
            || (content[i] >= lcharmin && content[i] < lcharmax)) {
                if (situation == 2) {
                    return numexpr;
                }
                if (content[i] >= lcharmin) {
                    tempint = 'a' - BASE;
                } else if (content[i] >= ucharmin) {
                    tempint = 'A' - BASE;
                } else {
                    tempint = '0';
                }
                if (content[i] - tempint > base) {
                    return numexpr;
                }
                tempint = content[i] - tempint;
                result += tempint * digit;
                digit *= base;
                situation = 1;
        } else if (content[i] == ' ') {
            if (situation == 1) {
                situation = 2;
            }
        } else if (situation == 1 && content[i] == '-') {
            situation = 2;
            result *= -1;
        } else if (situation == 1 && content[i] == '+') {
            situation = 2;
        } else if (situation == 1 && content[i] == '.') {
            if (dot == 0) {
                divideby = digit;
            } else {
                return numexpr;
            }
        } else {
            return numexpr;
        }
    }
    numexpr->type = TYPE_FLO;
    numexpr->ev.floval = result / divideby;
    return numexpr;
}

/*! Tries to cast the given expression to an integer
    @param expr     the expression to cast
    @return         the integer value of the given expression
*/
int castToInt (expression* expr) {
    if (expr->type == TYPE_INT) { // if the expression is an integer
        return expr->ev.intval; // return the integer
    } else if (expr->type == TYPE_FLO) { // if the expression is a float
        double floval = expr->ev.floval;
        if (floval < 0) { // if the float is negative then subtract 0.5 so the truncating rounding will be accurate
            return (int)(floval - 0.5);
        }
        return (int)(floval + 0.5); // if the float is positive then add 0.5 so the truncating rounding will be accurate
    } else if (expr->type == TYPE_STR) { // if the expression is a string
        if (expr->flag == EFLAG_VAR) { // if the expression is a variable
            expression* value = getVarValue(expr->ev.strval->content); // get the value the string variable is mapped to
            if (value != NULL) { // if the value was found then try to cast it to an integer
                return castToInt(value);
            }
        } else if (expr->flag == EFLAG_NONE) { // if the expression is a string literal
            return atoi(expr->ev.strval->content);
        }
    } else if (expr->type == TYPE_LAZ) { // if the expression is a lazy expression then evaluate it and try to cast it again
        return castToInt(evaluateLazy(expr));
    }
    return NIL; // if the expression can't be cast as an integer then return nil
}

/*! Evaluates the given expression, returning the result if it is a boolean, or returning nil otherwise
    @param expr     the expression to cast
    @return         the boolean value (0 or 1) of the given expression
*/
expression* castToBoo (expression* expr) {
    expression* result;
    if (expr->type == TYPE_INT) {
        result = newExpression_t(TYPE_INT);
        result->ev.intval = expr->ev.intval;
        return result;
    } else if (expr->type == TYPE_LAZ) {
        result = copyExpression(evaluateLazy(expr->ev.lazval->expval));
        expression* tempexpr = result;
        result = castToBoo(result);
        freeExpr(tempexpr);
        return result;
    } else {
        return newExpression_t(TYPE_NIL);
    }
}

/*! Returns the given float bankers'-rounded to the nearest digit of accuracy (returns an int if no accuracy specified)
    @param expr     the expression to cast
    @param digits   the number of digits of accuracy
    @return         the boolean value (0 or 1) of the given expression
*/
expression* fRound (double result, int digits) {
    int scale;
    if (digits == -1) {
        scale = 1;
        result *= BASE;
    } else {
        scale = pow(BASE, digits);
        result *= scale * BASE;
    }
    int toround = 1;
    int intval = result;
    while ((double)intval != result) {
        result *= BASE;
        intval = result;
        ++toround;
    }
    while (toround-- > 0) {
        int roundoff = intval % BASE;
        if (roundoff < BASE / 2 || (roundoff == BASE / 2 && (intval - roundoff) % (BASE * 2) == 0)) {
            result = floor(result / BASE);
        } else {
            result = ceil(result / BASE);
        }
    }
    result /= scale;
    if (digits == -1) {
        expression* intexpr = newExpression_t(TYPE_INT);
        intexpr->ev.intval = (int)result;
        return intexpr;
    } else {
        expression* fltexpr = newExpression_t(TYPE_FLO);
        fltexpr->ev.floval = result;
        return fltexpr;
    }
}

/*! Tries to cast the given expression to a float
    @param expr     the expression to cast
    @return         the floating point value of the given expression
*/
double castToFlo (expression* expr) {
    if (expr->type == TYPE_FLO) { // if the expression is a float then return it
        return expr->ev.floval;
    } else if (expr->type == TYPE_INT) { // if the expression is an integer then return it as a float
        return (double)(expr->ev.intval);
    } else if (expr->type == TYPE_STR) {
        if (expr->flag == EFLAG_VAR) { // if the expression is a variable
            expression* value = getVarValue(expr->ev.strval->content); // get the value the string variable is mapped to
            if (value != NULL) { // if the value was found then try to cast it to a float
                return castToFlo(value);
            }
        } else if (expr->flag == EFLAG_NONE) { // if the expression is a string literal
            return atof(expr->ev.strval->content);
        }
    } else if (expr->type == TYPE_LAZ) { // if the expression is a lazy expression then evaluate it and try to cast it again
        return castToFlo(evaluateLazy(expr));
    }
    return NIL; // if the expression can't be cast as a float then return nil
}

/*! Tries to cast the given expression to a string
    @param expr     the expression to cast
    @return         the floating point value of the given expression
*/
string* castToStr (expression* expr) {
    if (expr->type == TYPE_STR) { // if the expression is a string
        if (expr->flag == EFLAG_VAR) { // if the expression is a variable
            expression* value = getVarValue(expr->ev.strval->content); // get the value the string variable is mapped to
            if (value != NULL) { // if the value was found then try to cast it to a string
                return castToStr(value);
            }
        } else if (expr->flag == EFLAG_NONE) { // if the expression is a string literal then return it
            return copyString(expr->ev.strval);
        }
    } else if (expr->type == TYPE_INT) { // if the expression is an integer then return its string equivalent
        int size = snprintf(NULL, 0, "%ld", expr->ev.intval);
        char* result = allocate(size + 1);
        sprintf(result, "%ld", expr->ev.intval);
        return newString(result);
    } else if (expr->type == TYPE_FLO) { // if the expression is a flot then return its string equivalent
        int size = snprintf(NULL, 0, "%f", expr->ev.floval);
        char* result = allocate(size + 1);
        sprintf(result, "%f", expr->ev.floval);
        return newString(result);
    } else if (expr->type == TYPE_LAZ) { // if the expression is a lazy expression then evaluate it and try to cast it again
        return castToStr(evaluateLazy(expr));
    }
    return NIL; // if the expression can't be cast as a string then return nil
}

/*! Casts the given integer to a string, converting the integer to the given base in the process
    @param expr     the expression to cast
    @return         the floating point value of the given expression
*/
string* castToStrWithBase (int intval, uint base) {
    char* result; // the resulting string
    uint size; // the size of the string (minus the null terminator)
    bool sign = intval < 0; // 1 if the integer is negative, 0 if it's not
    intval = abs(intval); // make the integer positive if it's not already
    if (intval < base) { // if the given integer is less than the given base
        size = log10(intval) + 1 + sign; // calculate the size of the result string
        result = allocate(size + 1); // allocate the appropriate memory
        intval += '0'; // convert the integer to its ascii value
        sprintf(result + sign, "%c", alphaNumeric(intval)); // print the ascii value
        result[size] = '\0'; // null terminate the string
    } else { // if the given integer is more than or equal to the given base
        size = (log(intval) / log(base)) + 1 + sign; // calculate the size of the result string
        result = allocate(size + 1); // allocate the appropriate memory
        result[size] = '\0'; // null terminate the string
        int index = size - 1; // set the index to the end of the string (this algorithm creates the string backwords)
        result[index] = '0'; // initialize the current (last in this case) digit to 0
        uint cbase = base; // the current base
        uint asciibase = '0' + base; // the ascii equivalent to the given base
        uint calc = 0; // temporary variable for intermediate calculations
        int flag = 1; // a flag that determins what to do with the current digit that is reset after each digit calculation
        while (cbase <= intval) { // while the current base is less than the given integer
            calc = intval % cbase; // calculate the remainder of the integer divided by the current base
            if (calc >= base) { // add the appropriate value depending on whether or not the remainder is less than the base
                result[index] += calc / base;
            } else {
                result[index] += calc;
            }
            while (result[index] >= asciibase) { // while the current digit is more than the current base (and therefore needs to be reduced)
                result[index] = alphaNumeric('0' + (result[index] - '0') / base); // reduce the current digit
                flag = 0; // indicate that the digit doesn't need more processing
            }
            if (flag) { // if the digit needs more processing (because it never entered the previous while loop)
                result[index] = alphaNumeric(result[index]); // adjust the value to fit in one digit if it's more than 9
            } else { // reset the flag if it was set
                flag = 1;
            }
            result[index - 1] = '0'; // initialize the next digit to 0
            --index; // advance to the next digit
            intval -= calc; // reduce the integer to erase the remainder
            cbase *= base; // advance to the next base
        }
        if (intval >= cbase / base) { // if the first digit needs processing
            if (result[index] == 0) { // if the first digit hasn't been processed
                result[index] = alphaNumeric(asciibase - 1); // set it to the highest value allowed by the base
            } else { // if the first digit has been processed
                result[index] += alphaNumeric(intval / (cbase / base)); // adjust the digit to keep it below the base and to one digit
            }
        }
    }
    if (sign) { // if the given integer is negative add a negative sign
        result[0] = '-';
    }
    return newString(result);
}

/*! Converts a number to the appropriate letter needed to restrict the number to one digit
    @param number   the number to convert
    @return         the converted number
*/
uint alphaNumeric (uint number) {
    if (number > '9') { // if the ASCII value is more than 9 then skip to the capital letters in the ASCII table
        number += 7;
    }
    return number;
}

/*! Tries to cast the given expression to an array
    @param expr     the expression to cast
    @return         the array value of the given expression
*/
array* castToArr (expression* expr) {
    if (expr->type == TYPE_ARR) { // if the expression is an array then return it
        return expr->ev.arrval;
    } else if (expr->type == TYPE_STR && expr->flag == EFLAG_NONE) { // if the expression is a variable
        expression* value = getVarValue(expr->ev.strval->content); // get the value the string variable is mapped to
        if (value != NULL) { // if the value was found then try to cast it to an array
            return castToArr(value);
        }
    }
    // if the expression isn't an array
    array* result = newArray(1); // create an array with a size of 1
    result->content[0] = expr; // store the expression in the array
    return result;
}
